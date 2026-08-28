#include "config.h"
#include "game.h"
#include "entity.h"
#include "entity_hitbox.h"

Hitbox::Hitbox(Game* _game, Entity* _owner){
    game = _game;
    owner = _owner;

    color = {1.0f, 0.0f, 0.0f};

    pos = owner->getPosition();
    size = owner->getSize();
    
    worldBox = {
        pos.x - size,
        pos.y - size,
        size * 2.0f,
        size * 2.0f
    };

    // The hitboxMesh uses a local rectangle centered at its origin. The owner
    // position is supplied separately through the hitboxMesh transform uniform.
    hitboxMesh = new QuadMesh(_game, {
        -size,
        -size,
        size * 2.0f,
        size * 2.0f
    });

    accelMesh = new QuadMesh(_game, {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    });

    forwardMesh = new QuadMesh(_game, {
        1.0f,
        1.0f,
        1.0f,
        1.0f
    });

    uploadHitbox();
}

Hitbox::Hitbox(Game *_game, Entity* _owner, Rect _r)  : Hitbox(_game, _owner)
{
    setRect(_r);
    hitboxMesh->setBox(_r);
}

Hitbox::~Hitbox(){
    if (game != nullptr) {
        game->removeHitbox(this);
    }
    delete hitboxMesh;
    delete forwardMesh;
    delete accelMesh;
}

static void projectVertices(const std::array<Vec2, 4>& vertices,
                            const Vec2& axis,
                            float& minimum,
                            float& maximum)
{
    // Project the first corner onto the axis to initialize the interval. A
    // projection is the dot product of a point and the direction axis.
    minimum = maximum = vertices[0].x * axis.x + vertices[0].y * axis.y;

    // Project every remaining corner and expand the interval to contain it.
    // The final [minimum, maximum] range is the rectangle's shadow on the
    // selected axis.
    for (size_t i = 1; i < vertices.size(); ++i) {
        float projection = vertices[i].x * axis.x + vertices[i].y * axis.y;
        minimum = std::min(minimum, projection);
        maximum = std::max(maximum, projection);
    }
}

static bool projectionsOverlap(const std::array<Vec2, 4>& first,
                               const std::array<Vec2, 4>& second,
                               const Vec2& axis)
{
    // Find each rectangle's minimum and maximum projection on the same axis.
    float firstMin, firstMax;
    float secondMin, secondMax;

    projectVertices(first, axis, firstMin, firstMax);
    projectVertices(second, axis, secondMin, secondMax);

    // The intervals overlap when neither rectangle ends before the other
    // begins. A gap means this axis separates the rectangles.
    return firstMax >= secondMin && secondMax >= firstMin;
}

bool Hitbox::isColliding(Hitbox* _target)
{
    // A missing target cannot overlap this hitbox.
    if (_target == nullptr)
        return false;

    // Convert both axis-aligned Rect values into their four world-space
    // corners. The rotation is applied around each rectangle's center.
    const std::array<Vec2, 4> first = worldBox.vertices(rot);
    const std::array<Vec2, 4> second =
        _target->getRect()->vertices(_target->getRotation());

    // These are the two local axes for each rectangle. A rectangle rotated by
    // angle r has axes (cos(r), sin(r)) and (-sin(r), cos(r)).
    const Vec2 axes[] = {
        {std::cos(rot), std::sin(rot)},
        {-std::sin(rot), std::cos(rot)},
        {std::cos(_target->getRotation()), std::sin(_target->getRotation())},
        {-std::sin(_target->getRotation()), std::cos(_target->getRotation())}
    };

    // Project both rectangles onto every candidate axis. If one pair of
    // projections has a gap, that axis separates the rectangles and collision
    // is impossible.
    for (const Vec2& axis : axes) {
        if (!projectionsOverlap(first, second, axis))
            return false;
    }

    // No separating axis was found, so the rotated rectangles overlap.
    return true;
}

void Hitbox::logic(){
    // The hitbox can only update if both its render hitboxMesh and owning entity
    // still exist. This also protects against drawing after either object has
    // been destroyed or not initialized.
    if(hitboxMesh != nullptr && owner != nullptr && game != nullptr){
        drawMesh = game->getDebugStatus();

        // Copy the owner's current transform into the hitbox state so the
        // collision rectangle follows the entity as it moves and rotates.
        pos = owner->getPosition();
        const float ownerSize = owner->getSize();

        // Store the collision rectangle in world coordinates. The rectangle's
        // origin is its upper-left corner, while w and h describe its extent.
        worldBox = {
            (pos.x - ownerSize),
            (pos.y - ownerSize),
            ownerSize * 2.0f,
            ownerSize * 2.0f
        };

        // Draw only the visualization when debug mode is enabled. The actual
        // collision rectangle above is maintained regardless of this flag.
        if(drawMesh){
            // QuadMesh stores local vertex coordinates, so give it a rectangle
            // centered around its own origin. The hitboxMesh is translated below using
            // the owner's world position.

            Rect localBox = {-ownerSize, -ownerSize, worldBox.w, worldBox.h}; // Stores world hitbox coordinates in local coordinates
            hitboxMesh->setBox(localBox);
            hitboxMesh->setColor(color);
            
            float tanX = std::sin(owner->getVelocityVector().x);
            float tanY = std::cos(owner->getVelocityVector().y);

            // Keep the debug quad aligned with the owner's position and rotation.
            hitboxMesh->setPosition(pos.x, pos.y);
            hitboxMesh->setRotation(owner->getRotation());
            hitboxMesh->setSize(owner->getSize());

            Rect linebox = {
                -ownerSize + (worldBox.w / 2.0f) - MESH_WIDTH,
                -ownerSize + (worldBox.h / 2.0f),
                MESH_WIDTH * 2.0f,
                0.1f
            };

            forwardMesh->setBox(linebox);
            forwardMesh->setColor(Vec3{1.0f,1.0f,1.0f});

            forwardMesh->setPosition(worldBox.getPos().x, worldBox.getPos().y);

            /* forwardMesh->setRotation(std::acos(
                owner->getPosition().dot(Vec2{tanX, tanY}) /
                (owner->getPosition().magnitude() * Vec2{tanX, tanY}.magnitude())
            )); // -std::atan2(tanY, tanX) */
            forwardMesh->setRotation(owner->getRotation());
            forwardMesh->setSize(owner->getSize());

            linebox.h = 0.01f + ownerSize * 2.0f * owner->getVelocity();
            accelMesh->setBox(linebox);
            accelMesh->setColor(Vec3{0.0f,1.0f,0.0f});

            accelMesh->setPosition(worldBox.getPos().x, worldBox.getPos().y);

            /* accelMesh->setRotation(std::acos(
                owner->getPosition().dot(Vec2{tanX, tanY}) /
                (owner->getPosition().magnitude() * Vec2{tanX, tanY}.magnitude())
            )); // -std::atan2(tanY, tanX) */
            accelMesh->setRotation(forwardMeshRot);
            accelMesh->setSize(owner->getSize());
            draw();
        }
    } else {
        // If the hitbox is no longer valid, disable its debug rendering.
        drawMesh = false;
    }
}

void Hitbox::draw(){
    hitboxMesh->draw();
    forwardMesh->draw();
    accelMesh->draw();
}