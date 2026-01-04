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

}
