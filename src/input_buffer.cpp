#include "config.h"
#include "input_buffer.h"

InputBuffer::InputBuffer(Game* _game){
    game = nullptr;
    if(_game != nullptr) {
        game = _game;
    } else{
        printf("[Warning] InputBuffer::InputBuffer(Game* _game) _game points to null\n");
    }
    

    for(int i = 0; i < 512; i++){
        keyActions[{i, false}] = [](){};
    }
}

void InputBuffer::bindKey(InputKey _key, std::function<void()> _action){
    keyActions[_key] = _action;
}

void InputBuffer::bindKey(int _key, std::function<void()> _action){
    keyActions[{_key, false}] = _action;
}

void InputBuffer::handleKey(int _key, int _action){
    if (_action == GLFW_PRESS) {
        if (heldKeys.insert(_key).second)
            buffer.push({_key, false});
    } else if (_action == GLFW_RELEASE) {
        heldKeys.erase(_key);
    }
}

void InputBuffer::actionBuffer(){
    while (!buffer.empty()) {
        const InputKey key = buffer.front();
        buffer.pop();

        const auto action = keyActions.find(key);
        if (action != keyActions.end())
            action->second();
    }

    for (const int keyCode : heldKeys) {
        const auto action = keyActions.find({keyCode, true});
        if (action != keyActions.end())
            action->second();
    }
}