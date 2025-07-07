//
// Created by 分子轨道mo法 on 25-4-16.
//

#include "InputHandler.h"

#include <utility>

void InputHandler::ActionBind(int key, KeyAction Action, std::function<void()> callback) {

    switch (Action) {
        case KeyAction::Pressed:
            onPressed = std::move(callback);
            break;
        case KeyAction::Released:
            onReleased = std::move(callback);
            break;
        case KeyAction::Hold:
            break;
    }
}

void InputProcess::ProcessEvent() {
    for (auto event: KeyEventNeedProcessed) {


    }

}
