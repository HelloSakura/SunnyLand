#include "ResourceManager.h"
#include"TextureManager.h"
#include"FontManager.h"
#include"AudioManager.h"
#include<spdlog/spdlog.h>
#include<SDL3/SDL_render.h>

namespace engine::resource{
ResourceManager::ResourceManager(SDL_Renderer* pRenderer)
{
    //构造函数通过抛出异常来报告错误
    m_upTextureManager = std::make_unique<TextureManager>(pRenderer);
    m_upFontManager = std::make_unique<FontManager>();
    m_upAudioManager = std::make_unique<AudioManager>();

    spdlog::trace("ResourceManager 初始化成功");
}

ResourceManager::~ResourceManager()
{
    
}

SDL_Texture* ResourceManager::loadTexture(const std::string& path)
{
    return m_upTextureManager->loadTexture(path);
}

SDL_Texture* ResourceManager::tryGetTexture(const std::string& path)
{
    return m_upTextureManager->tryGetTexture(path);
}

void ResourceManager::unloadTexture(const std::string& path)
{
    m_upTextureManager->unloadTexture(path);
}

glm::vec2 ResourceManager::getTextureSize(const std::string& path)
{
    return m_upTextureManager->getTextureSize(path);
}

void ResourceManager::clearAllTextures()
{
    m_upTextureManager->clearAllTextures();
}

TTF_Font* ResourceManager::loadFont(const std::string& path, int size)
{
    return m_upFontManager->loadFont(path, size);
}

TTF_Font* ResourceManager::tryGetFont(const std::string& path, int size)
{
    return m_upFontManager->tryGetFont(path, size);
}

void ResourceManager::unloadFont(const std::string& path, int size)
{
    m_upFontManager->unloadFont(path, size);
}

void ResourceManager::clearAllFonts()
{
    m_upFontManager->clearAllFonts();
}

Mix_Chunk* ResourceManager::loadChunk(const std::string& path)
{
    return m_upAudioManager->loadChunk(path);
}

Mix_Chunk* ResourceManager::tryGetChunk(const std::string& path)
{
    return m_upAudioManager->tryGetChunk(path);
}

void ResourceManager::unloadChunk(const std::string& path)
{
    m_upAudioManager->unloadChunk(path);
}

void ResourceManager::clearAllChunks()
{
    m_upAudioManager->clearAllChunks();
}

Mix_Music* ResourceManager::loadMusic(const std::string& path)
{
    return m_upAudioManager->loadMusic(path);
}

Mix_Music* ResourceManager::tryGetMusic(const std::string& path)
{
    return m_upAudioManager->tryGetMusic(path);
}

void ResourceManager::unloadMusic(const std::string& path)
{
    m_upAudioManager->unloadMusic(path);
}

void ResourceManager::clearAllMusics()
{
    m_upAudioManager->clearAllMusics();
}


void ResourceManager::clearAll()
{
    m_upTextureManager->clearAllTextures();
    m_upFontManager->clearAllFonts();
    m_upAudioManager->clearAllChunks();
    m_upAudioManager->clearAllMusics();
    spdlog::trace("ResourceManager 清空所有资源成功");
};
}
