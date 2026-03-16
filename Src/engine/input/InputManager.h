/**
 * @file InputManager.h
 * @author lucida
 * @brief 输入管理器
 * @version 0.1
 * @date 2026-03-16
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

//related headers



// related namespaces
namespace engine::core{
    class Config;
}


namespace engine::input{

// key action state
enum class ActionState{
    INACTIVE,
    PRESSED_THIS_FRAME,
    HOLD_DOWN,      //持续按下
    RELEASED_THIS_FRAME,
};

//main class
class InputManager final{

}


}



