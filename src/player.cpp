#include "config.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "input_buffer.h"

Player::Player(Game* _game, GLuint _shader) : Entity(_game){
    shader = _shader;
    inputBuffer = game->getInputBuffer();

    lastTime = game->getCurrentTime();

    weaponCooldown = 0.25f; // 0.5 seconds cooldown

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
        const float bulletSpeed = 1.5f + (1.0f + getVelocity() * 200.0f);
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
    setAcceleration(target->forward() * -1.0f * game->deltaTime());
}
