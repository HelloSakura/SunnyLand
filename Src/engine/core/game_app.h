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

#include<unique_ptr>

//前向声明，减少头文件依赖，提高编译速度
struct SDL_Window;
struct SDL_Renderer;


namespace engine::core {  //命名空间：保持路径结构清晰，避免命名冲突

class XTime;

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
    void update(float deltaTime);
    void render();


private:
    SDL_Window* m_pWindow = nullptr;
    SDL_Renderer* m_pRenderer = nullptr;
    bool m_IsRunning = false;

    //引擎组件
    std::unique_ptr<XTime> m_upTimeComponent;
};
}
