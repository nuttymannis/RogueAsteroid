#include "config.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "input_buffer.h"

Player::Player(Game* _game, GLuint _shader) : Entity(_game){
    setID(EntityID::Player);
    shader = _shader;
    inputBuffer = game->getInputBuffer();

    lastTime = game->getCurrentTime();

    weaponCooldown = 0.15f; // 0.1 second cooldown

    hp = 3;
    playerHue = 0.0f;

    ship = new TriangleMesh(game);
    playerColor = {1.0f, 0.5f, 0.0f};

    inputBuffer->bindKey({GLFW_KEY_W, true}, [this]() { accelerate(getSpeed() * game->deltaTime()); });
    inputBuffer->bindKey({GLFW_KEY_S, true}, [this]() { accelerate(-getSpeed() * game->deltaTime()); });
    inputBuffer->bindKey({GLFW_KEY_A, true}, [this]() { rotate(getSpeed() * 10 * game->deltaTime()); });
    inputBuffer->bindKey({GLFW_KEY_D, true}, [this]() { rotate(getSpeed() * -10 * game->deltaTime()); });
    inputBuffer->bindKey({GLFW_KEY_SPACE, true}, [this]() { setBrake(true);});
    inputBuffer->bindKey({GLFW_KEY_J, false}, [this]() { fireWeapon(); });
}

void Player::draw()
{
    // Player owns the simulation state now that TriangleMesh is render-only.
    Entity::logic();

    if(rainbow){
        if(rainbowIncreasing){
            playerHue += hueIncrement * game->deltaTime();
            if(playerHue >= 1.0f)
                rainbowIncreasing = false;
        } else {
            playerHue -= hueIncrement * game->deltaTime();
            if(playerHue <= 0.0f)
                rainbowIncreasing = true;
        }

        /* ship->setColor({fmod(playerColor.x + playerHue, 1.0f),
                        fmod(playerColor.y + playerHue, 1.0f),
                        fmod(playerColor.z + playerHue, 1.0f)}); */

        /* ship->setColor({
            std::sin(playerHue), 
            std::sin(playerHue + 0.3334f), 
            std::sin(playerHue + 0.6667f)
        }); */

        ship->setColor({
            playerHue,
            fmod(playerHue + 0.34f, 1.0f),
            fmod(playerHue + 0.67f, 1.0f)
        });
                        
    }

    ship->setPosition(pos.x, pos.y);
    ship->setRotation(rot);
    ship->draw(shader);
}

void Player::fireWeapon()
{
    if(game->getCurrentTime() - lastTime > weaponCooldown)
    {
        printf("Firing weapon!\n");

        const float bulletSize = Game::randomFloat(0.004f, 0.01f);

        // Carry some of the player's current velocity into the projectile so
        // firing while moving gives the bullet a different initial speed.
        const float bulletSpeed = 1.5f + (1.0f + getVelocity() * 2.0f);
        Bullet* bullet = new Bullet(game, pos.x + forward().x * size, pos.y + forward().y * size, bulletSize,
                bulletSpeed, rot);

        game->spawnBullet(bullet);
        printf("Bullet count: %zu\n", game->getBullets().size());

        lastTime = game->getCurrentTime();
    }
}

void Player::input()
{
    setBrake(false); // Reset brake state at the start of each input cycle.

    /* if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) accelerate(getSpeed() * game->deltaTime());
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) accelerate(-getSpeed() * game->deltaTime());
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) rotate(getSpeed() * 10 * game->deltaTime()); 
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) rotate(getSpeed() * -10 * game->deltaTime());

    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || getVelocity() >= 0.007f) setBrake(true);
    else setBrake(false);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) fireWeapon();

    if(glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS) game->generateStars(48);

    if(glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS && currentTime - lastTime > 0.5f) {
        game->toggleDebugStatus();
        printf("Debug status: %s\n", game->getDebugStatus() ? "ON" : "OFF");
        lastTime = currentTime;
    } */
}

void Player::onCollision(Entity *target)
{
    /* if(game->getBullets().size()) {
        if(target != game->getBullets().front())
            setAcceleration(target->forward() * -1.0f * game->deltaTime());
    } else 
        setAcceleration(target->forward() * -1.0f * game->deltaTime()); */

    if(target != nullptr && target->getID() != EntityID::Bullet){
        // Build a vector from the target's center to the player's center.
        Vec2 offset = getPosition() - target->getPosition();

        // The distance is used to normalize offset into a direction vector.
        float distance = offset.magnitude();

        // Avoid dividing by zero if both entities occupy the same position.
        if(distance == 0.0f)
            return;

        // normal points from the target toward the player and has length 1.
        Vec2 normal = offset / distance;

        // Relative velocity describes how the player moves compared with the
        // target, which is what determines whether they approach each other.
        Vec2 relativeVelocity = getVelocityVector() - target->getVelocityVector();

        // The dot product keeps only the relative velocity along the collision
        // normal; sideways motion does not push the objects apart.
        float speedAlongNormal = relativeVelocity.x * normal.x + relativeVelocity.y * normal.y;

        // A non-negative value means the objects are separating already.
        if(speedAlongNormal >= 0.0f)
            return;

        // Reverse the inward normal speed to create a simple bounce impulse.
        float impulse = -speedAlongNormal;
        // Add the impulse to the player and apply the opposite impulse to the
        // target so momentum is transferred in opposite directions.
        setVelocityVector(getVelocityVector() + normal * impulse * 1.2f); 
        target->setVelocityVector(target->getVelocityVector() - normal * impulse);
    }
}
