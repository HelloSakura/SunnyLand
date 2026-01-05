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
    spdlog::trace("TextureManager 初始化成功");
}

TextureManager::~TextureManager()
{
    clearAllTextures();
    m_pRenderer = nullptr;
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
    auto it = m_textureMap.find(path);
    if(it != m_textureMap.end()){
        return it->second.get();
    }
    
    auto pTexture = loadTexture(path);
    return pTexture;
}

void TextureManager::unloadTexture(const std::string& path)
{
    auto it = m_textureMap.find(path);
    if(it != m_textureMap.end()){
        m_textureMap.erase(it);
        spdlog::debug("卸载图片成功: {}", path);
    }
    else{
        spdlog::error("卸载图片失败: {}", path + " 不存在");
    }
}

glm::vec2 TextureManager::getTextureSize(const std::string& path)
{
    auto pTexture = tryGetTexture(path);
    if(!pTexture){
        spdlog::error("获取图片大小失败: {}", path + " 不存在");
        return glm::vec2(0, 0);
    }

    glm::vec2 size;
    if(!SDL_GetTextureSize(pTexture, &size.x, &size.y)){
        spdlog::error("获取图片大小失败: {}", path);
        return glm::vec2(0, 0);
    }
    return size;
}

void TextureManager::clearAllTextures()
{
    m_textureMap.clear();
}



}