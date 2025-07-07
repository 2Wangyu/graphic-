//
// Created by 分子轨道mo法 on 25-4-16.
//

#ifndef CG_E2_INPUTHANDLER_H
#define CG_E2_INPUTHANDLER_H

#include <GLFW/glfw3.h>
#include <functional>
#include "Timer.h"


enum class KeyAction {
    Pressed,
    Released,
    Hold
};


class InputHandler {
    int Key;
    //判断按下多久算长按
    float HoldTime;
    //如果没有绑定Hold事件则为空,unique_ptr不会造成额外性能开销
    std::unique_ptr<Timer> timer;

    std::function<void()> onPressed;
    std::function<void()> onReleased;
    std::function<void()> onHold;


public:
    void ActionBind(int key, KeyAction Action, std::function<void()> callback);

};

//每一帧获取的输入事件
struct InputEvent {
    int key;
    int KeyAction;
};

//处理每一帧的输入事件，判断其是否为按下，
class InputProcess {
    //按键注册的事件列表
    std::unordered_map<int, InputHandler> KeyHold;
    //每一帧收到的事件
    std::vector<InputEvent> KeyEventNeedProcessed;

    void ProcessEvent();

};


#endif //CG_E2_INPUTHANDLER_H
