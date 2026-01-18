#pragma once

#include <string>
#include <optional>
#include <SDL3/SDL_rect.h>


namespace engine::render {


/**
 * @brief 精灵类
 * 只包含纹理标识符、绘制的数据和状态
 * 渲染工作交给render
 * 翻转、移动等操作交给SpriteComponent
 */
class Sprite final{
public:
    //传入id，和绘制部分数据SDL_FRect
    Sprite(const std::string& strTextureID, const std::optional<SDL_FRect>& srcRect = std::nullopt, bool isFlipX = false);
    ~Sprite();

    //--- getters ---
    const std::string& getTextureID() const;
    const std::optional<SDL_FRect>& getRect() const;
    bool isFlipX() const;

    //--- setters ---
    void setTextureID(const std::string& strTextureID);
    void setRect(const std::optional<SDL_FRect>& rect);
    void setFlipX(bool isFlipX);
    
private:
    std::string m_strTextureID;             //@brief 纹理标识符
    std::optional<SDL_FRect> m_optRect;      //@brief 绘制部分
    bool m_isFlipX = false;                 //@brief 是否水平翻转
};
}