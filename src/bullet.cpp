#include "bullet.h"
#include "game.h"
#include "entity_hitbox.h"

Bullet::Bullet(Game* _game, float rootX, float rootY, float bulletSize,
			   float bulletSpeed, float rotation, bool _isPlayer)
	: Entity(_game)
{
	setID(EntityID::Bullet);
    isPlayer = _isPlayer;
	shader = game->getShader();

	// Bullet owns the circle used to display it. CircleMesh remains responsible
	// for generic geometry, movement, and OpenGL drawing operations.
	mesh = new CircleMesh(game, rootX, rootY, bulletSize);
	setPosition(rootX, rootY);
	setSize(bulletSize);
	setSpeed(bulletSpeed);
	setRotation(rotation);
	accelerate(getSpeed());
	mesh->setPosition(pos.x, pos.y);
	mesh->setRotation(rot);

    
}

Bullet::~Bullet()
{
	// Bullet owns the mesh it creates, so release the mesh with the bullet.
    delete mesh;
}

void Bullet::logic(){
	// Bullets use the shared acceleration and braking math but do not wrap at
	// the screen edge; leaving the normalized bounds marks them for removal.
	integrateMotion();

	lifeTime -= game->deltaTime();

	if(lifeTime <= 0.0f)
		expired = true;

	// World entities wrap around the normalized screen bounds by default.
    if (pos.x > 1.0f || pos.x < -1.0f) {
        pos.x *= -1.0f;
    }
    if (pos.y > 1.0f || pos.y < -1.0f) {
        pos.y *= -1.0f;
    }
	
    if(!expired && hitbox != nullptr){
        //hitbox->setRect({pos.x - size + 20.0f, pos.y - size * 4.0f, size * 8.0f, size * 8.0f});
        hitbox->logic();
    }
}

void Bullet::onCollision(Entity *target)
{
	printf("Bullet HAHASHDASDIHA\n");
	if(target != nullptr && target != this && expired == false && target->isDead() == false && target != game->getPlayer()){
		printf("[!] Bullet *%p collided with Entity *%p\n", this, target);
		expired = true;
		if (hitbox != nullptr)
			hitbox->setDraw(false); // Disable debug rendering for expired bullets.
		
		game->addScore(100);
		printf("Score: %d\n", game->getScore());
		target->kill();
	}
}

void Bullet::draw()
{
	if (!expired) {
		// Advance the Bullet entity once before copying its transform to the
		// render-only CircleMesh.
		logic();

		// Bullet owns the simulation state; synchronize it into the circle before
		// drawing so the mesh remains a rendering component only.
		mesh->setPosition(pos.x, pos.y);
		mesh->setRotation(rot);
		mesh->draw();
	} else {
		kill();
	}
}

