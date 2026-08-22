#pragma once
#include <functional>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "game.h"
#include "player.h"

struct ActionKey {
    int key;
    std::function<void()> action;
};

struct InputKey{
    int key;
    bool pressed = false;

    bool operator==(const InputKey& other) const {
        return key == other.key && pressed == other.pressed;
    }
};

struct InputKeyHash {
    std::size_t operator()(const InputKey& inputKey) const noexcept {
        return std::hash<int>{}(inputKey.key) ^
            (std::hash<bool>{}(inputKey.pressed) << 1);
    }
};

class InputBuffer {
    public:
    InputBuffer(Game* _game = nullptr); 

    void setGame(Game* _game) {game = _game;}
    
    void bindKey(InputKey _key, std::function<void()> _action);
    void bindKey(int _key, std::function<void()> _action);
    
    void actionBuffer();

    void handleKey(int _key, int _action);

    private:
    std::unordered_map<InputKey, std::function<void()>, InputKeyHash> keyActions;
    std::queue<InputKey> buffer;
    std::unordered_set<int> heldKeys;
    Game* game;

};