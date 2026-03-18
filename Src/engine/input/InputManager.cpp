/**
 * @file InputManager.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-03-17
 * 
 * @copyright Copyright (c) 2026
 * 
 */



 #include "InputManager.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
 #include<stdexcept>
 #include<spdlog/spdlog.h>
 #include"engine/core/Config.h"


 namespace engine::input{

InputManager::InputManager(SDL_Renderer* sdl_renderer, const engine::core::Config* config)
{
    if(!sdl_renderer){
        throw std::runtime_error("SDL_Renderer 为空");
    }
    m_pRenderer = sdl_renderer;
    initializeMappings(config);
    spdlog::trace("InputManager 初始化成功");
}


void InputManager::update()
{
    //根据上一帧的值 更新默认的按键状态
    for(const auto& [action, state] : m_actionStates){
        if(state == ActionState::PRESSED_THIS_FRAME || state == ActionState::HOLD_DOWN){
            m_actionStates[action] = ActionState::HOLD_DOWN;
        }
        else if(state == ActionState::RELEASED_THIS_FRAME){
            m_actionStates[action] = ActionState::INACTIVE;
        }
    }

    //处理所有SDL事件， 将SDL事件转换为动作状态
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        processEvent(event);
    }

}


bool InputManager::isActionDown(const std::string& actionName) const
{
    if(auto it = m_actionStates.find(actionName); it != m_actionStates.end()){
        return it->second == ActionState::PRESSED_THIS_FRAME || it->second == ActionState::HOLD_DOWN;
    }
    return false;
}


bool InputManager::isActionPressed(const std::string& actionName) const
{
    if(auto it = m_actionStates.find(actionName); it != m_actionStates.end()){
        return it->second == ActionState::PRESSED_THIS_FRAME;
    }
    return false;
}


bool InputManager::isActionReleased(const std::string& actionName) const
{
    if(auto it = m_actionStates.find(actionName); it != m_actionStates.end()){
        return it->second == ActionState::RELEASED_THIS_FRAME;
    }
    return false;
}


bool InputManager::shouldQuit() const
{
    return m_shouldQuit;
}

void InputManager::setShouldQuit(bool shouldQuit)
{
    m_shouldQuit = shouldQuit;
}


glm::vec2 InputManager::getScreenMousePosition() const
{
    return m_mousePosition;
}


glm::vec2 InputManager::getLogicalMousePosition() const
{
    glm::vec2 logic_pos;
    //通过“窗口坐标获取渲染坐标”（逻辑坐标）
    SDL_RenderCoordinatesFromWindow(m_pRenderer, m_mousePosition.x, m_mousePosition.y, &logic_pos.x, &logic_pos.y);
    return logic_pos;
}

SDL_Scancode InputManager::scancodeFromString(const std::string& key_name)
{
    return SDL_GetScancodeFromName(key_name.c_str());
}


Uint8 InputManager::mouseButtonUint8FromString(const std::string& button_name)
{
    if(button_name == "MouseLeft"){
        return SDL_BUTTON_LEFT;
    }
    if(button_name == "MouseRight"){
        return SDL_BUTTON_RIGHT;
    }
    if(button_name == "MouseMiddle"){
        return SDL_BUTTON_MIDDLE;
    }
    return 0;
}

void InputManager::initializeMappings(const engine::core::Config* config)
{
    if(config == nullptr){
        spdlog::error("config is null");
        throw std::runtime_error("config is null");
    }
    //获取配置中的映射
    m_actionToKeyMappings = config->getInputBindings();

    //clear cache
    m_inputToActionMap.clear();
    m_actionStates.clear();

    //添加默认的鼠标映射
    if(m_actionToKeyMappings.find("mouse_left_click") == m_actionToKeyMappings.end()){
        spdlog::warn("mouse_left_click is not found in config");
        m_actionToKeyMappings["mouse_left_click"] = {"MouseLeft"};
    }
    if(m_actionToKeyMappings.find("mouse_right_click") == m_actionToKeyMappings.end()){
        spdlog::warn("mouse_right_click is not found in config");
        m_actionToKeyMappings["mouse_right_click"] = {"MouseRight"};
    }
    if(m_actionToKeyMappings.find("mouse_middle_click") == m_actionToKeyMappings.end()){
        spdlog::warn("mouse_middle_click is not found in config");
        m_actionToKeyMappings["mouse_middle_click"] = {"MouseMiddle"};
    }
    //遍历 actionToKeyMappings 映射，填充其它映射
    for(const auto& [action, keys] : m_actionToKeyMappings){
        m_actionStates[action] = ActionState::INACTIVE;
        for(const auto& key : keys){
            SDL_Scancode scancode = scancodeFromString(key);
            Uint8 mouseButton = mouseButtonUint8FromString(key);

            if(scancode != SDL_SCANCODE_UNKNOWN){
                m_inputToActionMap[scancode].push_back(action);
                spdlog::trace("added scancode mapping for action: {} key: {}", action, key);
            }
            else if(mouseButton != 0){
                m_inputToActionMap[mouseButton].push_back(action);
                spdlog::trace("added mouse button mapping for action: {} key: {}", action, key);
            }
            else{
                spdlog::warn("invalid key: {} for action: {}", key, action);
            }
        }
    }
}


void InputManager::processEvent(const SDL_Event& event)
{
    switch(event.type){
        //处理按键点击
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP:
        {
            SDL_Scancode scancode = event.key.scancode;
            bool is_down = event.key.down;
            bool is_repeat = event.key.repeat;

            if(auto it = m_inputToActionMap.find(scancode); it != m_inputToActionMap.end()){
                const std::vector<std::string>& actions = it->second;
                for(const std::string& action : actions){
                    updateActionState(action, is_down, is_repeat);
                }
            }
        }
        //处理鼠标点击
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
        case SDL_EVENT_MOUSE_BUTTON_UP:
        {
            Uint8 button = event.button.button;
            bool is_down = event.button.down;
            if(auto it = m_inputToActionMap.find(button); it != m_inputToActionMap.end()){
                const std::vector<std::string>& actions = it->second;
                for(const std::string& action : actions){
                    updateActionState(action, is_down, false);
                }   
            }
            //更新鼠标位置
            m_mousePosition = {event.button.x, event.button.y};
            break;
        }
        //处理鼠标移动
        case SDL_EVENT_MOUSE_MOTION:
        {
            m_mousePosition = {event.motion.x, event.motion.y};
            break;
        }
        case SDL_EVENT_QUIT:
            m_shouldQuit = true;
            break;
        default:
            break;
    }
}

void InputManager::updateActionState(const std::string& action_name, bool is_input_active, bool is_repeat_event)
{
    auto it = m_actionStates.find(action_name);
    if(it == m_actionStates.end()){
        spdlog::warn("action: {} not found in actionStates", action_name);
        return;
    }
    ActionState& state = it->second;
    if(is_input_active){
        if(is_repeat_event){
            state = ActionState::HOLD_DOWN;
        }
        else{
            state = ActionState::PRESSED_THIS_FRAME;
        }
    }
    else{
        state = ActionState::RELEASED_THIS_FRAME;
    }
    spdlog::trace("updated action: {} state: {}", action_name, static_cast<int>(state));
}

}