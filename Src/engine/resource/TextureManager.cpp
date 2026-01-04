#include "TextureManager.h"
#include "ResourceManager.h"
#include<stdexcept>
#include<SDL3/SDL_render.h>
#include<spdlog/spdlog.h>
#include<SDL3_image/SDL_image.h>

namespace engine::resource{

TextureManager::TextureManager(SDL_Renderer* pRenderer)
{
    if(!pRenderer){
        throw std::runtime_error("渲染器指针为空");
    }
    m_pRenderer = pRenderer;
}

TextureManager::~TextureManager()
{

}

SDL_Texture* TextureManager::loadTexture(const std::string& path)
{
    auto it = m_textureMap.find(path);
    if(it != m_textureMap.end()){
        return it->second.get();
    }
    SDL_Texture* pTexture = IMG_LoadTexture(m_pRenderer, path.c_str());
    if(!pTexture){
        spdlog::error("加载图片失败: {}", path);
    }
    m_textureMap.emplace(path, std::unique_ptr<SDL_Texture, SDLTextureDeleter>(pTexture));
    spdlog::debug("加载图片成功: {}", path);
    return pTexture;
}

SDL_Texture* TextureManager::tryGetTexture(const std::string& path)
{
    return nullptr;
}

void TextureManager::unloadTexture(SDL_Texture* pTexture)
{
}

void TextureManager::clearAllTextures()
{
    m_textureMap.clear();
}

glm::vec2 TextureManager::getTextureSize(SDL_Texture* pTexture)
{
    return glm::vec2(0, 0);
}

}