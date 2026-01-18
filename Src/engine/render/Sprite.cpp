#include "Sprite.h"


namespace engine::render {

Sprite::Sprite(const std::string& strTextureID, const std::optional<SDL_FRect>& srcRect, bool isFlipX)
    : m_strTextureID(strTextureID), m_optRect(srcRect), m_isFlipX(isFlipX)
{
}

Sprite::~Sprite()
{}

const std::string& Sprite::getTextureID() const
{
    return m_strTextureID;
}

const std::optional<SDL_FRect>& Sprite::getRect() const
{
    return m_optRect;
}

bool Sprite::isFlipX() const
{
    return m_isFlipX;
}

void Sprite::setTextureID(const std::string& strTextureID)
{
    m_strTextureID = strTextureID;
}

void Sprite::setRect(const std::optional<SDL_FRect>& rect)
{
    m_optRect = rect;
}

void Sprite::setFlipX(bool isFlipX)
{
    m_isFlipX = isFlipX;
}

};
