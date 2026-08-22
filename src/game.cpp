#pragma once
#include "config.h"
#include "game.h"
#include "player.h"
#include "asteroid.h"
#include "entity_hitbox.h"
#include <algorithm>

void Game::removeHitbox(Hitbox* hitbox)
{
    if(hitbox == nullptr)
        return;
    auto hitboxIt = std::remove(hitboxObjects.begin(), hitboxObjects.end(), hitbox);
    hitboxObjects.erase(hitboxIt, hitboxObjects.end());
}

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
	targetHitbox = nullptr;
    setScore(0);
    
    // Initialize all timing values from the same clock sample so the first
    // frame has a valid delta time instead of using uninitialized data.
    Game::lastTime = Game::currentTime = Game::fpsLast = glfwGetTime();
    Game::frameCount = 0;
    Game::_deltaTime = 0.0f;

    
}

Game::Game(GLFWwindow* _window, GLuint _shader, InputBuffer* _iBuffer) : Game(){
    window = _window;
    shader = _shader;

    if(_iBuffer != nullptr)
        inputBuffer = _iBuffer;
    else
        inputBuffer = new InputBuffer(this);
      
    player = new Player(this, shader);

    inputBuffer->bindKey({GLFW_KEY_ESCAPE}, [this](){pauseGame();});
    inputBuffer->bindKey({GLFW_KEY_F8, false}, [this](){generateStars(512);});
    inputBuffer->bindKey({GLFW_KEY_F7, false}, [this](){toggleDebugStatus();});

    inputBuffer->bindKey(GLFW_KEY_F6, [this](){
        for(size_t i = 0; i < hitboxObjects.size(); ++i){
            Hitbox* hitbox = hitboxObjects[i];
            if(hitbox != nullptr){
                printf("Hitbox %zu: Owner %p, Owner Type: %s, Pos [%f, %f], Box [%f, %f, %f, %f]\n", 
                    i,
                    hitbox->getOwner(),
                    typeid(*(hitbox->getOwner())).name(),
                    hitbox->getPos().x,
                    hitbox->getPos().y,
                    hitbox->getRect()->x,
                    hitbox->getRect()->y,
                    hitbox->getRect()->w, 
                    hitbox->getRect()->h
                );
            }
        }
    });

    inputBuffer->bindKey({GLFW_KEY_F5, true}, [this](){
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        float newX = float(1.0 - ((x / 800.0) * 2.0));
        float newY = float(1.0 - ((y / 600.0) * 2.0));
        player->setPosition(newX, newY);
    });

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow* callbackWindow, int key, int scancode, int action, int mods){
        Game* game = static_cast<Game*>(glfwGetWindowUserPointer(callbackWindow));

        if(game == nullptr)
            return;

        game->getInputBuffer()->handleKey(key, action);
    });

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
        asteroid->accelerate(2.0f * deltaTime()); //Game::randomFloat(0.001f,0.0013f)
        asteroidObjects.push_back(asteroid);
    }
}

void Game::AABECollisionLogic()
{   // TODO: Change to sweep and prune or spatial partitioning to reduce the number of collision checks.

    // Copy the registered pointers before checking collisions. Collision
    // callbacks can mark entities for deletion, so iterating over a stable
    // snapshot prevents the loop from being invalidated by later cleanup.
    const std::vector<Hitbox*> collisionSnapshot = hitboxObjects;

    // Check each hitbox against every other hitbox. This is a simple
    // all-pairs collision test with O(n^2) time complexity.
    for (Hitbox* host : collisionSnapshot) {
        if (host == nullptr)
            continue;

        // Reset the previous target before searching for a collision for this
        // host. Only the first overlapping hitbox is handled this frame.
        targetHitbox = nullptr;

        bool colliding = false;
        for (Hitbox* check : collisionSnapshot) {
            // Do not compare a hitbox with itself. isColliding() performs the
            // axis-aligned bounding-box test against the candidate target.
            if (check != nullptr && host != check && host->isColliding(check)) {
                colliding = true;
                targetHitbox = check;
                break;
            }
        }

        // Convert the hitboxes back into the entities that own them. The
        // collision callback belongs to the host entity, while target is the
        // other entity involved in the collision.
        Entity* hostOwner = host->getOwner();
        Entity* target = targetHitbox != nullptr
            ? targetHitbox->getOwner()
            : nullptr;

        // Dispatch only when both owners are valid. Objects are marked for
        // removal by callbacks, but are deleted later by Game's cleanup code.
        if (colliding && hostOwner != nullptr && target != nullptr) {
            hostOwner->onCollision(target);
        }

        // Red identifies overlapping hitboxes and yellow identifies clear
        // hitboxes. This affects only the optional debug visualization; the
        // collision result itself is determined by isColliding().
        host->setColor(colliding
            ? Vec3{1.0f, 0.0f, 0.0f}
            : Vec3{1.0f, 1.0f, 0.5f});  // When not colliding
    }
}

float Game::deltaTime() {
    return _deltaTime;
}

void Game::logic(){ // This function runs first
    player->input();
    // if(hitboxObjects.size() > 0)
    // printf("[%d](%p) %p\n", 0, hitboxObjects.at(0)->getOwner(), this);

    if(asteroidObjects.size() == 0)
        generateAsteroids();
}

void Game::draw(){ // Handles all drawing of game objects after logic() is called
    for(size_t i = 0; i < starObjects.size(); ++i){
        Star* star = starObjects[i];
        star->draw(Game::shader);
    }

    for (size_t i = 0; i < bulletObjects.size() && pause == false;) {
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
    
    for(size_t i = 0; i < asteroidObjects.size() && pause == false; ) {
        if(asteroidObjects[i]->isDead()){
            delete asteroidObjects[i];
            asteroidObjects.erase(asteroidObjects.begin() + i);
        } else {
            asteroidObjects[i]->draw();
            ++i;
        }
    }

    if(pause == false)
        player->draw();

    // All entities have now updated their hitboxes for this frame.
    AABECollisionLogic();
}



void Game::calculateFrames(){
    currentTime = glfwGetTime();
    double seconds = 5.0;

    // Assign deltaTime
    _deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Fps Counter
    frameCount++;
    //printf("Frame Count: %i\n", frameCount);
    if(currentTime - fpsLast >= seconds){
        fpsLast = currentTime;
        printf("%f fps\n", double(frameCount/seconds));
        frameCount = 0;
    }


}
