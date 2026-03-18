/**
 * @file game_app.h
 * @author lucida
 * @brief 游戏应用类 负责游戏的主循环和事件处理 唯一单例
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 */

#pragma once

#include "Config.h"
#include<memory>



//前向声明，减少头文件依赖，提高编译速度
struct SDL_Window;
struct SDL_Renderer;

namespace engine::resource{
    class ResourceManager;
}

namespace engine::render{
    class Renderer;
    class Camera;
}

namespace engine::input{
    class InputManager;
}

namespace engine::core {  //命名空间：保持路径结构清晰，避免命名冲突

class XTime;
class Config;

/**
 * @brief 游戏应用类
 * @details 负责游戏的主循环和事件处理 唯一单例
 */
class GameApp final{
public:
    GameApp();
    ~GameApp();

    void run();

    /**
     * @brief 禁止拷贝构造和赋值
     */
    GameApp(const GameApp&) = delete;
    GameApp& operator=(const GameApp&) = delete;
    GameApp(GameApp&&) = delete;
    GameApp& operator=(GameApp&&) = delete;

private:
    [[nodiscard]] bool init();   //初始化游戏应用， nodiscard：避免返回值被忽略
    void close();
    void handleEvents();
    void update(double deltaTime);
    void render();

    //引擎组件初始化
    [[nodiscard]] bool initConfig();
    [[nodiscard]] bool initSDL();
    [[nodiscard]] bool initTime();
    [[nodiscard]] bool initResourceManager();
    [[nodiscard]] bool initRenderer();
    [[nodiscard]] bool initCamera();
    [[nodiscard]] bool initInputManager();


    //测试函数
    void testResourceManager();
    void testRenderer();
    void testCamera();
    void testInputManager();


private:
    SDL_Window* m_pWindow = nullptr;
    SDL_Renderer* m_pRenderer = nullptr;
    bool m_IsRunning = false;
    int m_frameIndex = 0;

    //引擎组件
    std::unique_ptr<Config> m_upConfig;
    std::unique_ptr<XTime> m_upTimeComponent;
    std::unique_ptr<engine::resource::ResourceManager> m_upResourceManager;
    std::unique_ptr<engine::render::Renderer> m_upRenderer;
    std::unique_ptr<engine::render::Camera> m_upCamera;
    std::unique_ptr<engine::input::InputManager> m_upInputManager;
};
}
