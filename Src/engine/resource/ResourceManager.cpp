#include<memory>
#include "ResourceManager.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "AudioManager.h"
#include<spdlog/spdlog.h>

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

TextureManager* ResourceManager::loadTexture(const std::string& path)
{
    return m_upTextureManager->loadTexture(path);
}

TextureManager* ResourceManager::tryGetTexture(const std::string& path)
{
    return m_upTextureManager->tryGetTexture(path);
}

void ResourceManager::unloadTexture(TextureManager* pTexture)
{
    m_upTextureManager->unloadTexture(pTexture);
}

void ResourceManager::clearAllTextures()
{
    m_upTextureManager->clearAll();
}

FontManager* ResourceManager::loadFont(const std::string& path, int size)
{
    return m_upFontManager->loadFont(path, size);
}

FontManager* ResourceManager::tryGetFont(const std::string& path, int size)
{
    return m_upFontManager->tryGetFont(path, size);
}

void ResourceManager::unloadFont(FontManager* pFont)
{
    m_upFontManager->unloadFont(pFont);
}

void ResourceManager::clearAllFonts()
{
    m_upFontManager->clearAll();
}

AudioManager* ResourceManager::loadChunk(const std::string& path)
{
    return m_upAudioManager->loadChunk(path);
}

AudioManager* ResourceManager::tryGetChunk(const std::string& path)
{
    return m_upAudioManager->tryGetChunk(path);
}

void ResourceManager::unloadChunk(AudioManager* pChunk)
{
    m_upAudioManager->unloadChunk(pChunk);
}

void ResourceManager::clearAllChunks()
{
    m_upAudioManager->clearAllChunks();
}

AudioManager* ResourceManager::loadMusic(const std::string& path)
{
    return m_upAudioManager->loadMusic(path);
}

AudioManager* ResourceManager::tryGetMusic(const std::string& path)
{
    return m_upAudioManager->tryGetMusic(path);
}

void ResourceManager::unloadMusic(AudioManager* pMusic)
{
    m_upAudioManager->unloadMusic(pMusic);
}

void ResourceManager::clearAllMusics()
{
    m_upAudioManager->clearAllMusics();
}


void ResourceManager::clearAll()
{
    m_upTextureManager->clearAll();
    m_upFontManager->clearAll();
    m_upAudioManager->clearAll();

}
}
