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
	bool isExpired() const { return expired; }
	CircleMesh* getMesh() { return mesh; }

private:
	CircleMesh* mesh;
	// New bullets must be drawable until their bounds check marks them expired.
	bool expired = false;
	bool isPlayer = false;
};

