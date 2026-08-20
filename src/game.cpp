#pragma once
#include "config.h"
#include "game.h"
#include "player.h"
#include "asteroid.h"
#include "entity_hitbox.h"

float Game::randomFloat(float minimum, float maximum)
{
    // The generator is static so it is seeded once and reused. Re-seeding on
    // every call could produce repeated values when bullets or stars are
    // created close together in time.
    static std::random_device seedSource;
    static std::mt19937 generator(seedSource());
    std::uniform_real_distribution<float> distribution(minimum, maximum);
    return distribution(generator);
}

Game::Game(){
    window = NULL;
    score = 0;
    
    // Initialize all timing values from the same clock sample so the first
    // frame has a valid delta time instead of using uninitialized data.
    Game::lastTime = Game::currentTime = Game::fpsLast = glfwGetTime();
    Game::frameCount = 0;
    Game::_deltaTime = 0.0f;

    
}

Game::Game(GLFWwindow* _window, GLuint _shader) : Game(){
    window = _window;
    shader = _shader;
    player = new Player(this, shader);

    generateStars(512);
}

void Game::generateStars(int count, float _brightness){
    // Reserve the final pointer count up front so adding stars does not cause
    // repeated vector reallocations as the collection grows.
    starObjects.clear();
    starObjects.reserve(count);

    for(int i = 0; i < count; ++i){
        Star* star = new Star(this, Game::randomFloat(0.0005f, 0.001f));
        starObjects.push_back(star);
    }

    for(int i = 0; i < count; ++i){
        Star* star = new Star(this, Game::randomFloat(0.0005f, 0.001f), Game::randomFloat(0.1f,0.8f));
        starObjects.push_back(star);
    }
}

void Game::generateAsteroids(int count){
    for(int i = 0; i < count; i++){
        Asteroid* asteroid = new Asteroid(this);
        asteroid->rotate(Game::randomFloat(0, M_PI*2));
        asteroid->accelerate(Game::randomFloat(0.01f,0.1f));
        asteroidObjects.push_back(asteroid);
    }
}

float Game::deltaTime() {
    return _deltaTime;
}

void Game::logic(){ // This function runs first
    player->input();
    if(hitboxObjects.size() > 0)
    printf("[%d](%p) %p\n", 0, hitboxObjects.at(0)->getOwner(), this);

    if(asteroidObjects.size() == 0)
        generateAsteroids();
}

void Game::draw(){ // Handles all drawing of game objects after logic() is called
    for(size_t i = 0; i < starObjects.size(); ++i){
        Star* star = starObjects[i];
        star->draw(Game::shader);
    }

    for (size_t i = 0; i < bulletObjects.size();) {
        Bullet* bullet = bulletObjects[i];
        bullet->draw(Game::shader);

        if (bullet->isExpired()) {
            // Erasing invalidates the current vector element and shifts later
            // bullets left. Keeping i unchanged ensures the shifted bullet is
            // processed instead of skipped.
            // Delete the object first, then remove its pointer from the list.
            // The next bullet shifts into this index, so do not increment i.
            delete bullet;
            bulletObjects.erase(bulletObjects.begin() + i);
        } else {
            ++i; // Only increment i if the bullet was not removed, to avoid skipping the next bullet in the list.
        }
    }
    
    for(size_t i = 0; i < asteroidObjects.size(); i++){
            asteroidObjects[i]->draw();
    }

    player->draw();
}



void Game::calculateFrames(){
    currentTime = glfwGetTime();
    double seconds = 5.0;

    // Assign deltaTime
    _deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Fps Counter
    frameCount++;
    if(currentTime - fpsLast >= seconds){
        fpsLast = currentTime;
        printf("%f fps\n", double(frameCount/seconds));
        frameCount = 0;
    }


}
