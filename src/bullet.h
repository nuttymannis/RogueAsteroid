#pragma once

#include "config.h"
#include "circle_mesh.h"
#include "entity.h"

class Game;

class Bullet : public Entity {
public:
	Bullet(Game* game, float rootX, float rootY, float bulletSize,
		   float bulletSpeed, float rotation, bool isPlayer = false);
	~Bullet();
	void draw(GLuint shader);
	void logic() override;
	void onCollision(Entity* target) override;
	bool isExpired() const { return expired; }
	CircleMesh* getMesh() { return mesh; }

private:
	CircleMesh* mesh; // Circle geometry owned and rendered by this bullet.
	// New bullets must be drawable until their bounds check marks them expired.
	bool expired = false;
	bool isPlayer = false; // Identifies which side created the projectile.
};

