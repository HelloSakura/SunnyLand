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
#include "Config.h"

#include "SDL3/SDL_render.h"
#include "engine/core/XTime.h"
#include "engine/utils/RandomUtils.h"
#include "engine/utils/PathUtils.h"
#include "engine/resource/ResourceManager.h"
#include "engine/render/Renderer.h"
#include "engine/render/Camera.h"

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>
#include <memory>


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

    testResourceManager();

    //主循环
    while(m_IsRunning){
        m_upTimeComponent->update();
        double deltaTime = m_upTimeComponent->getDeltaTime();

        handleEvents();
        update(deltaTime);;
        render();
        m_frameIndex++;
    }

    close();
}

bool GameApp::init()
{
    // 初始化日志系统（必须在其他日志输出之前调用）
    spdlog::trace("GameApp 初始化开始");
    if(!initConfig()) return false;
    if(!initSDL()) return false;
    if(!initTime()) return false;
    if(!initResourceManager()) return false;
    if(!initRenderer()) return false;
    if(!initCamera()) return false;
    spdlog::trace("GameApp 初始化成功");

    m_IsRunning = true;
    return true;
}

void GameApp::update(double deltaTime)
{
    testCamera();
}


void GameApp::render()
{
    m_upRenderer->clearScreen();
    testRenderer();
    m_upRenderer->present();
}

void GameApp::handleEvents()
{
    SDL_Event event;
    // 处理所有待处理的事件
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                // 用户点击关闭按钮
                spdlog::trace("收到退出事件，关闭游戏");
                m_IsRunning = false;
                break;
            
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                // 窗口关闭请求（Alt+F4 等）
                spdlog::trace("收到窗口关闭请求，关闭游戏");
                m_IsRunning = false;
                break;
            
            case SDL_EVENT_WINDOW_MINIMIZED:
                // 窗口最小化
                spdlog::trace("窗口已最小化");
                break;
            
            case SDL_EVENT_WINDOW_RESTORED:
                // 窗口恢复
                spdlog::trace("窗口已恢复");
                break;
            
            default:
                // 其他事件可以在这里处理
                break;
        }
    }
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


bool GameApp::initConfig()
{
    try{
        m_upConfig = std::make_unique<Config>(engine::utils::PathUtils::getResourcePath("assets/config.json"));
    }
    catch(const std::exception& e){
        spdlog::error("Config 初始化失败: {}", e.what());
        return false;
    }
    spdlog::trace("Config 初始化成功");
    return true;
}

bool GameApp::initSDL() 
{
    //初始化SDL
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
        spdlog::error("SDL 初始化失败: {}", SDL_GetError());
        return false;
    }
    
    //创建窗口
    m_pWindow = SDL_CreateWindow(m_upConfig->windowTitle.c_str(), m_upConfig->windowWidth, m_upConfig->windowHeight, SDL_WINDOW_RESIZABLE);
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

    //设置VSync模式
    int vsync_mode = m_upConfig->vsyncEnabled ? SDL_RENDERER_VSYNC_ADAPTIVE : SDL_RENDERER_VSYNC_DISABLED;
    SDL_SetRenderVSync(m_pRenderer, vsync_mode);
    spdlog::trace("VSync 设置为: {}", m_upConfig->vsyncEnabled ? "Enabled" : "Disabled");

    //设置逻辑分辨率
    SDL_SetRenderLogicalPresentation(m_pRenderer, m_upConfig->windowWidth/2, m_upConfig->windowHeight/2, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    spdlog::trace("SDL 初始化成功");
    return true;
}

bool GameApp::initTime()
{
    try{
        m_upTimeComponent = std::make_unique<XTime>();
    }
    catch(const std::exception& e){
        spdlog::error("XTime 初始化失败: {}", e.what());
        return false;
    }

    m_upTimeComponent->setTargetFps(m_upConfig->frameRate);
    spdlog::trace("XTime 初始化成功");
    return true;
}

bool GameApp::initResourceManager()
{
    try{
        m_upResourceManager = std::make_unique<engine::resource::ResourceManager>(m_pRenderer);
    }catch(const std::exception& e){
        spdlog::error("ResourceManager 初始化失败: {}", e.what());
        return false;
    }
    spdlog::trace("ResourceManager 初始化成功");
    return true;
}

bool GameApp::initRenderer()
{
    try{
        m_upRenderer = std::make_unique<engine::render::Renderer>(m_pRenderer, m_upResourceManager.get());
    }catch(const std::exception& e){
        spdlog::error("Renderer 初始化失败: {}", e.what());
        return false;
    }
    spdlog::trace("Renderer 初始化成功");
    return true;
}

bool GameApp::initCamera()
{
    try{
        m_upCamera = std::make_unique<engine::render::Camera>(glm::vec2(1280, 720), glm::vec2(0, 0));
    }catch(const std::exception& e){
        spdlog::error("Camera 初始化失败: {}", e.what());
        return false;
    }
    spdlog::trace("Camera 初始化成功");
    return true;
}



void GameApp::testResourceManager()
{
    spdlog::debug("测试ResourceManager");
    // 使用 PathUtils 获取资源路径，确保跨平台兼容性
    std::string texturePath = engine::utils::PathUtils::getResourcePath("assets/textures/Actors/frog.png");
    std::string musicPath = engine::utils::PathUtils::getResourcePath("assets/audio/monster.mp3");
    std::string fontPath = engine::utils::PathUtils::getResourcePath("assets/fonts/VonwaonBitmap-16px.ttf");

    m_upResourceManager->loadTexture(texturePath);
    m_upResourceManager->loadMusic(musicPath);
    m_upResourceManager->loadFont(fontPath, 16);

    m_upResourceManager->unloadTexture(texturePath);
    m_upResourceManager->unloadMusic(musicPath);
    m_upResourceManager->unloadFont(fontPath, 16);
};

void GameApp::testRenderer()
{
    // 使用 PathUtils 获取资源路径，确保跨平台兼容性
    engine::render::Sprite sprite_frog(engine::utils::PathUtils::getResourcePath("assets/textures/Actors/frog.png"));
    engine::render::Sprite sprite_ui(engine::utils::PathUtils::getResourcePath("assets/textures/UI/buttons/Start1.png"));
    engine::render::Sprite sprite_background(engine::utils::PathUtils::getResourcePath("assets/textures/Layers/back.png"));

    static float rotation = 0.0f;
    rotation += 1.0f;

    m_upRenderer->drawParallax(*m_upCamera, sprite_background, glm::vec2(100.0f, 100.0f), glm::vec2(1.0f, 1.0f), {true, false});
    m_upRenderer->drawSprite(*m_upCamera, sprite_frog, glm::vec2(100.0f, 100.0f), glm::vec2(1.0f, 1.0f), rotation);
    m_upRenderer->drawUISprite(sprite_ui,  glm::vec2(100.0f, 100.0f));
}

void GameApp::testCamera()
{
    auto keyState = SDL_GetKeyboardState(nullptr);
    if(keyState[SDL_SCANCODE_UP]){
        m_upCamera->move(glm::vec2(0.0f, -10.0f));
    }
    if(keyState[SDL_SCANCODE_DOWN]){
        m_upCamera->move(glm::vec2(0.0f, 10.0f));
    }
    if(keyState[SDL_SCANCODE_LEFT]){
        m_upCamera->move(glm::vec2(-10.0f, 0.0f));
    }
    if(keyState[SDL_SCANCODE_RIGHT]){
        m_upCamera->move(glm::vec2(10.0f, 0.0f));
    }
}

}