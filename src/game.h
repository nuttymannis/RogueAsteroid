#pragma once

#include "config.h"
#include "bullet.h"
#include "star.h"
#include "asteroid.h"

class Player;

class Random;

class Star;

class Entity;

class Game {
    public:
    Game();
    Game(GLFWwindow* _window, GLuint _shader);
    GLFWwindow* window;
    GLFWwindow* getWindow() { return window; }
    void spawnBullet(Bullet* bullet) { bulletObjects.push_back(bullet); }
    std::vector<Bullet*>& getBullets() { return bulletObjects; }
    std::vector<Hitbox*>& getHitboxList() {return hitboxObjects;}
    double getCurrentTime() { return currentTime; }
    GLuint getShader() { return shader; }
    static float randomFloat(float minimum, float maximum);
    float deltaTime();
    void generateStars(int count = 8, float _brightness = 1.0f);
    void generateAsteroids(int count = 5);
    void AABECollisionLogic();
    void calculateFrames();
    void draw();
    void logic();

    private:
    uint16_t score;
    std::vector<Bullet*> bulletObjects = std::vector<Bullet*>();
    std::vector<Star*> starObjects = std::vector<Star*>();
    std::vector<Asteroid*> asteroidObjects = std::vector<Asteroid*>();
    std::vector<Entity*> entityList = std::vector<Entity*>();
    std::vector<Hitbox*> hitboxObjects = std::vector<Hitbox*>();
    GLuint shader;
    double currentTime, fpsLast, lastTime;
    float _deltaTime;
    int frameCount;
    Player* player;
};