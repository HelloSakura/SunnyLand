/**
 * @file InputManager.h
 * @author lucida
 * @brief 输入管理器 完成抽象动作和实际输入之间的映射 需要注意的是关于按键之间的状态
 * @version 0.1
 * @date 2026-03-16
 * 
 * @copyright Copyright (c) 2026
 */

#pragma once

//related headers
#include<string>
#include<glm/glm.hpp>
#include<unordered_map>
#include<variant>
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"


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

public:
    InputManager(SDL_Renderer* sdl_renderer, const engine::core::Config* config);
    
    void update();  ///< @brief 更新输入状态 每轮循环最先调用

    //动作状态检查
    bool isActionDown(const std::string& actionName) const;     ///< @brief 动作是否当前触发
    bool isActionPressed(const std::string& actionName) const;   ///< @brief 动作是否在当前帧触发
    bool isActionReleased(const std::string& actionName) const;   ///< @brief 动作是否在当前帧释放

    bool shouldQuit() const;                ///< @brief 查询退出状态
    void setShouldQuit(bool shouldQuit);    ///< @brief 设置退出状态

    glm::vec2 getScreenMousePosition() const;    ///< @brief 获取屏幕鼠标位置
    glm::vec2 getLogicalMousePosition() const;   ///< @brief 获取逻辑鼠标位置


private:
    void initializeMappings(const engine::core::Config* config);    ///< @brief 根据config初始化动作映射
    void processEvent(const SDL_Event& event);                      ///< @brief 处理 SDL 事件（将按键转换为动作状态）
    void updateActionState(const std::string& action_name, bool is_input_active, bool is_repeat_event);    ///< @brief 更新动作状态
    SDL_Scancode scancodeFromString(const std::string& key_name);          ///< @brief 将字符串转换为扫描码
    Uint8 mouseButtonUint8FromString(const std::string& button_name);      ///< @brief 将字符串转换为鼠标按钮码


private:
    SDL_Renderer* m_pRenderer;  //获取逻辑坐标的SDL_Renderer指针
    std::unordered_map<std::string, std::vector<std::string>> m_actionToKeyMappings; //存储动作名称到按键名称列表的映射
    std::unordered_map<std::variant<SDL_Scancode, Uint8>, std::vector<std::string>> m_inputToActionMap; //从输入到关联动作名称的映射
    std::unordered_map<std::string, ActionState> m_actionStates; //存储动作当前状态

    bool m_shouldQuit = false; //退出标志 默认false
    glm::vec2 m_mousePosition; //鼠标位置
};
}
