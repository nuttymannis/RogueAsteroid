#include "bullet.h"
#include "game.h"

Bullet::Bullet(Game* _game, float rootX, float rootY, float bulletSize,
			   float bulletSpeed, float rotation, bool _isPlayer)
	: Entity(_game)
{
    isPlayer = _isPlayer;

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

	// Unlike ships and stars, bullets are not wrapped around the screen. Their
	// position is checked after integration so Game can delete them from its
	// container once they have crossed the visible play area.
	const float _x = pos.x;
	const float _y = pos.y;
    if(_x > 1 || _x < -1 || _y > 1 || _y < -1)
        expired = true;
       
}

void Bullet::draw(GLuint shader)
{
	if (!expired) {
		// Advance the Bullet entity once before copying its transform to the
		// render-only CircleMesh.
		logic();

		// Bullet owns the simulation state; synchronize it into the circle before
		// drawing so the mesh remains a rendering component only.
		mesh->setPosition(pos.x, pos.y);
		mesh->setRotation(rot);
		mesh->draw(shader);
	}
}

