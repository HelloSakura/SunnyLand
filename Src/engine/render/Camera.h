#pragma once

#include "engine/utils/MathUtils.h"
#include <optional>


namespace engine::render {

class Camera final{
public:
    ///< @brief 初始化相机，传入视口大小和相机左上角的世界坐标，限制相机的移动范围
    Camera(const glm::vec2& v2ViewportSize, const glm::vec2& v2WorldPos = glm::vec2(0.0f, 0.0f), const std::optional<engine::utils::Rect>& optLimitBounds = std::nullopt);
    ~Camera();

    ///< @brief 更新相机位置
    void update(double deltaTime);
    ///< @brief 移动相机
    void move(const glm::vec2& v2Move);
    ///< @brief 世界坐标转屏幕坐标
    glm::vec2 worldToScreen(const glm::vec2& v2WorldPos) const;
    ///< @brief 带有视差剪辑的世界坐标转屏幕坐标
    glm::vec2 worldToScreenWithParallax(const glm::vec2& v2WorldPos, const glm::vec2& v2ParallaxFactor) const;
    ///< @brief 屏幕坐标转世界坐标
    glm::vec2 screenToWorld(const glm::vec2& v2ScreenPos) const;

    //--- setters ---
    ///< @brief 设置相机位置
    void setWorldPos(const glm::vec2& v2WorldPos);
    ///< @brief 设置限制相机的移动范围
    void setLimitBounds(const std::optional<engine::utils::Rect>& optLimitBounds);

    //--- getters ---
    ///< @brief 获取相机位置
    const glm::vec2& getWorldPos() const;
    ///< @brief 获取限制相机的移动范围
    const std::optional<engine::utils::Rect>& getLimitBounds() const;
    ///< @brief 获取视口大小
    const glm::vec2& getViewportSize() const;

    //只有一个实例，禁用拷贝构造和赋值构造
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;
    Camera(Camera&&) = delete;
    Camera& operator=(Camera&&) = delete;

private:
    ///< @brief 限制相机位置在边界内
    void clampWorldPos();

private:
    glm::vec2 m_v2ViewportSize;  ///< @brief 视口大小（屏幕大小）
    glm::vec2 m_v2WorldPos;      ///< @brief 相机左上角的世界坐标
    std::optional<engine::utils::Rect> m_optLimitBounds;///< @brief 限制相机的移动范围
};
}
