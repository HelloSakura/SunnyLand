/**
 * @file game_app.cpp
 * @author lucida
 * @brief 游戏应用类实现
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 */



#include "GameApp.h"
#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <memory>
#include "LoggerUtil.h"
#include "XTime.h"
#include "RandomUtils.h"
#include "ResourceManager.h"


namespace engine::core {

GameApp::GameApp()
{

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
    //初始化时间
    SDL_srand(SDL_GetTicks());
    //主循环
    while(m_IsRunning){
        m_upTimeComponent->update();
        double deltaTime = m_upTimeComponent->getDeltaTime();

        handleEvents();
        update(deltaTime);;
        render();
        SDL_DelayNS(static_cast<Uint64>(engine::utils::RandomUtils::getInstance().randomDoubleInRange(0, 300) * 1000000.0));
        spdlog::trace("frame_index:{} delta_time:{}", m_frameIndex, deltaTime);
        m_frameIndex++;
    }

    close();
}

bool GameApp::init()
{
    // 初始化日志系统（必须在其他日志输出之前调用）
    initLogger();
    initSDL();
    initTime();
    initResourceManager();
    
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

void GameApp::update(double deltaTime)
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

void GameApp::initSDL()
{
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        spdlog::error("SDL 初始化失败: {}", SDL_GetError());
        throw std::runtime_error("SDL 初始化失败");
    }
    spdlog::trace("SDL 初始化成功");
}

void GameApp::initLogger()
{
    engine::utils::initLogger();
    spdlog::trace("Logger 初始化成功");
}

void GameApp::initTime()
{
    m_upTimeComponent = std::make_unique<XTime>();
    if(nullptr == m_upTimeComponent){
        spdlog::error("XTime 初始化失败");
        throw std::runtime_error("XTime 初始化失败");
    }
    m_upTimeComponent->setTargetFps(10);
    m_upTimeComponent->setTimeScaleFactor(1.0);
}

void GameApp::initResourceManager()
{
    m_upResourceManager = std::make_unique<engine::resource::ResourceManager>(m_pRenderer);
    if(nullptr == m_upResourceManager){
        spdlog::error("ResourceManager 初始化失败");
        throw std::runtime_error("ResourceManager 初始化失败");
    }
    spdlog::trace("ResourceManager 初始化成功");
}
}