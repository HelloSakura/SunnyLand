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
 #include<stdexcept>
 #include<spdlog/spdlog.h>


 namespace engine::input{

InputManager::InputManager(SDL_Renderer* sdl_renderer, const engine::core::Config& config)
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

}


bool InputManager::isActionDown(const std::string& actionName) const
{
    return m_actionStates.at(actionName) == ActionState::PRESSED_THIS_FRAME;
}

bool InputManager::isActionPressed(const std::string& actionName) const
{
    return m_actionStates.at(actionName) == ActionState::PRESSED_THIS_FRAME;
}

bool InputManager::isActionReleased(const std::string& actionName) const
{
    return m_actionStates.at(actionName) == ActionState::RELEASED_THIS_FRAME;
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
    
}


void InputManager::processEvent(const SDL_Event& event)
{
    
}

void InputManager::initializeMappings(const engine::core::Config& config)
{
    //初始化映射

    //获取配置中的映射

    //clear cache

    //添加默认的鼠标映射

    //遍历动作映射
}
}