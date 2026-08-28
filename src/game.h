#pragma once

#include "config.h"
#include "bullet.h"
#include "star.h"
#include "asteroid.h"
#include "input_buffer.h"
#include "entity.h"

#include "ft2build.h"
#include FT_FREETYPE_H

class Player;

class Random;

class Star;

class Entity;

class InputBuffer;
class TextRenderer;

#define STAR_COUNT 512 

class Game {
    public:
    Game();
    Game(GLFWwindow* _window, GLuint _shader, InputBuffer* _iBuffer = nullptr, TextRenderer* _t = nullptr);
    GLFWwindow* window;
    GLFWwindow* getWindow() { return window; }
    void spawnBullet(Bullet* bullet) { entityList.push_back(bullet); }
    std::vector<Bullet*> getBullets() {
        std::vector<Bullet*> bullets;
        for (Entity* entity : entityList) {
            if (Bullet* bullet = dynamic_cast<Bullet*>(entity)) {
                bullets.push_back(bullet);
            }
        }
        return bullets;
    } // return array of bullets
    std::vector<Hitbox*>& getHitboxList() {return hitboxObjects;}
    void removeHitbox(Hitbox* hitbox);
    Hitbox* getTargetHitbox() {return targetHitbox;}
    double getCurrentTime() { return currentTime; }
    uint16_t getScore() { return score; }
    void setScore(uint16_t _score) { score = _score;}
    void addScore(uint16_t _score) { score += _score;}
    bool isPaused(){ return pause;}
    void pauseGame() {
        if(isPaused())
            pause = false;
        else 
            pause = true;
        
        printf("Pausing game: %d", isPaused());
    }
    GLuint getShader() { return shader; }
    bool getDebugStatus() {return debug;}
    void toggleDebugStatus() {debug = !debug;}
    static float randomFloat(float minimum, float maximum);
    float deltaTime();
    void generateStars(int count = 8, float _brightness = 1.0f);
    void generateAsteroids(int count = 5);
    void AABECollisionLogic();
    void calculateFrames();
    Player* getPlayer() { return player; }
    void draw();
    void logic();
    InputBuffer* getInputBuffer() {return inputBuffer;}

    bool findEntityByID(EntityID _id) {
        return std::any_of(entityList.begin(), entityList.end(),
        [_id](Entity* entity) {
            return entity != nullptr && entity->getID() == _id;
        });
    }

    private:
    bool debug = true;                  // Enables collision-box/debug rendering.
    bool pause = false;                 // Pauses game if true.
    uint16_t score;                     // Current player score.
    std::vector<Entity*> entityList;    // General entity registry, if populated.
    std::vector<Entity*> backEntityList;// Less important background objects
    std::vector<Hitbox*> hitboxObjects; // Registered hitboxes used for collision checks.
    GLuint shader;                      // Linked OpenGL shader program for the scene.
    double currentTime;                 // Current GLFW clock time.
    double fpsLast;                     // Clock time when the FPS interval began.
    double lastTime;                    // Previous frame time used for deltaTime.
    double AABELast;                    // Previous AABB collision update time.
    float _deltaTime;                   // Seconds elapsed since the previous frame.
    int frameCount;                     // Frames counted during the FPS interval.
    Player* player;                     // Player object owned by the game.
    InputBuffer* inputBuffer;           // Input buffer for handling player input.
    Hitbox* targetHitbox;               // Temporary hitbox used for collision checks.
    TextRenderer* textRenderer;
};