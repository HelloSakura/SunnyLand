/**
 * @file game_app.cpp
 * @author lucida
 * @brief 游戏应用类实现
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 */



#include "game_app.h"
#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include "logger_util.h"
#include "XTime.h"


namespace engine::core {

GameApp::GameApp()
{
    m_upTimeComponent = std::make_unique<XTime>();
}

GameApp::~GameApp()
{
    if(m_IsRunning){
        spdlog::warn("GameApp 销毁时没有显示关闭，将强制关闭");
        close();
    }
}


void GameApp::run()
{
    if(!init()){
        spdlog::error("GameApp 初始化失败");
        return;
    }

    //主循环
    while(m_IsRunning){
        float deltaTime = 0.01f;
        handleEvents();
        update(deltaTime);;
        render();
    }

    close();
}

bool GameApp::init()
{
    // 初始化日志系统（必须在其他日志输出之前调用）
    engine::utils::initLogger();
    
    spdlog::trace("GameApp 初始化开始");

    //初始化SDL
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        spdlog::error("SDL 初始化失败: {}", SDL_GetError());
        return false;
    }

    //创建窗口
    m_pWindow = SDL_CreateWindow("SunnyLand", 1280, 720, SDL_WINDOW_RESIZABLE);
    if(nullptr == m_pWindow){
        spdlog::error("窗口创建失败: {}", SDL_GetError());
        return false;
    }
    spdlog::trace("窗口创建成功");

    //创建渲染器
    m_pRenderer = SDL_CreateRenderer(m_pWindow, nullptr);
    if(nullptr == m_pRenderer){
        spdlog::error("渲染器创建失败: {}", SDL_GetError());
        return false;
    }
    spdlog::trace("渲染器创建成功");
    spdlog::trace("SDL 初始化成功");
    
    m_IsRunning = true;
    return true;
}

void GameApp::update(float deltaTime)
{

}


void GameApp::render()
{

}

void GameApp::handleEvents()
{

}

void GameApp::close()
{
    spdlog::trace("GameApp 关闭开始");
    
    // 销毁渲染器
    if(m_pRenderer){
        SDL_DestroyRenderer(m_pRenderer);
        m_pRenderer = nullptr;
        spdlog::trace("渲染器已销毁");
    }
    
    // 销毁窗口
    if(m_pWindow){
        SDL_DestroyWindow(m_pWindow);
        m_pWindow = nullptr;
        spdlog::trace("窗口已销毁");
    }
    
    // 退出SDL
    SDL_Quit();
    spdlog::trace("SDL 已退出");
    
    m_IsRunning = false;
    spdlog::trace("GameApp 关闭完成");
}


}