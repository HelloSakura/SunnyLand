#pragma once

#include <optional>
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "Sprite.h"
#include "engine/render/Camera.h"

struct SDL_Renderer;
struct SDL_FRect;

namespace engine::resource {
    class ResourceManager;
}

namespace engine::render {

/*
 * @brief 渲染器类
 * 封转SDL_Renderer，提供渲染接口
 * 依赖一个有效的SDL_Renderer和ResourceManager，在初始化时传入
 */
class Renderer final{
public:
    ///< @brief  构造函数，参数为空抛出异常
    Renderer(SDL_Renderer* pRenderer, engine::resource::ResourceManager* pResourceManager);
    ~Renderer();
    /*
     * brief 绘制一个Sprite
     * @param camera 相机
     * @param sprite 精灵
     * @param position 位置
     * @param scale 缩放
     * @param angle 角度
    */
    void drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale = glm::vec2(1.0f, 1.0f), double angle = 0.0);

    /*
     * @brief 绘制视差滚动的背景
     * @param camera 相机
     * @param sprite 精灵
     * @param position 位置
     * @param scrollFactor 视差滚动因子
     * @param repeat 是否重复
     * @param scale 缩放
    */
    void drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scrollFactor, 
                      const glm::bvec2& repeat = {true, true}, const glm::vec2& scale = glm::vec2(1.0f, 1.0f));

    ///< @brief 在屏幕坐标中直接渲染一个用于UI的Sprite对象
    void drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size = std::nullopt);

    void present();     ///< @brief 更新屏幕
    void clearScreen(); ///< @brief 清空屏幕
    void setDrawColorUint8(Uint8 r, Uint8 g, Uint8 b, Uint8 a);///< @brief 设置绘制的颜色 Uint8类型
    void setDrawColorFloat(float r, float g, float b, float a);///< @brief 设置绘制颜色 float类型
    SDL_Renderer* getRenderer() const;///< @brief 获取底层的SDL_renderer

    //只有一个实例，禁用拷贝构造和赋值构造
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

private:
    ///< @brief 获取Sprite的原矩形，用于具体绘制
    std::optional<SDL_FRect> getSpriteRect(const Sprite& sprite);
    ///< @brief 判断矩形是否在视口中，用于视口裁剪，AABB碰撞检测，图片与屏幕边缘有偏差就绘制
    bool isRectInViewport(const Camera& camera, const SDL_FRect& rect);

private:
    ///< @brief 指向SDL_Renderer，可以为nullptr，不负责其生命周期管理
    SDL_Renderer* m_pRenderer = nullptr;
    ///< @brief 指向ResourceManager，可以为nullptr，不负责其生命周期管理
    engine::resource::ResourceManager* m_pResourceManager = nullptr;
};
}
