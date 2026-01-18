#include "Camera.h"


namespace engine::render {

Camera::Camera(const glm::vec2& v2ViewportSize, const glm::vec2& v2WorldPos, const std::optional<engine::utils::Rect>& optLimitBounds)
    : m_v2ViewportSize(v2ViewportSize), m_v2WorldPos(v2WorldPos), m_optLimitBounds(optLimitBounds)
{

}

Camera::~Camera()
{}

void Camera::update(double deltaTime)
{
    // 自动跟随目标
}

void Camera::move(const glm::vec2& offset)
{
    m_v2WorldPos += offset;
    clampWorldPos();
}

glm::vec2 Camera::worldToScreen(const glm::vec2& v2WorldPos) const
{
    return v2WorldPos - m_v2WorldPos;
}

glm::vec2 Camera::worldToScreenWithParallax(const glm::vec2& v2WorldPos, const glm::vec2& v2ParallaxFactor) const
{
    return v2WorldPos - m_v2WorldPos * v2ParallaxFactor;
}

glm::vec2 Camera::screenToWorld(const glm::vec2& v2ScreenPos) const
{
    return v2ScreenPos + m_v2WorldPos;
}

void Camera::setWorldPos(const glm::vec2& v2WorldPos)
{
    m_v2WorldPos = v2WorldPos;
    clampWorldPos();    //修改后需要重新限制相机位置
}

void Camera::setLimitBounds(const std::optional<engine::utils::Rect>& optLimitBounds)
{
    m_optLimitBounds = optLimitBounds;
    clampWorldPos();    //修改后需要重新限制相机位置
}

const glm::vec2& Camera::getWorldPos() const
{
    return m_v2WorldPos;
}

const std::optional<engine::utils::Rect>& Camera::getLimitBounds() const
{
    return m_optLimitBounds;
}

const glm::vec2& Camera::getViewportSize() const
{
    return m_v2ViewportSize;
}


void Camera::clampWorldPos()
{
    //边界检查确保相机视图在限制范围内
    if(m_optLimitBounds.has_value() && m_optLimitBounds->size.x > 0 && m_optLimitBounds->size.y > 0){
        // 计算允许的相机位置范围
        glm::vec2 minPos = m_optLimitBounds->position;   //左上角位置
        glm::vec2 maxPos = minPos + m_optLimitBounds->size - m_v2ViewportSize;       //左上角位置 + 限制范围大小 - 视口大小 = 右下角位置

        // 保护，确保maxPos至少大于minPos，如果世界区域比视口区域小，按以上方式计算，最后会导致maxPos小于minPos，需要修正
        maxPos.x = std::max(minPos.x, maxPos.x);
        maxPos.y = std::max(minPos.y, maxPos.y);
        m_v2WorldPos = glm::clamp(m_v2WorldPos, minPos, maxPos);
    }

    //如果限制范围为空，则不进行限制
}

}
