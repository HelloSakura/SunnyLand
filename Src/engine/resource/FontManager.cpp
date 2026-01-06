#include "FontManager.h"
#include "SDL3_ttf/SDL_ttf.h"
#include <stdexcept>
#include <SDL3/SDL_error.h>
#include <spdlog/spdlog.h>

namespace engine::resource{

FontManager::FontManager()
{
    if(!TTF_WasInit() && !TTF_Init()){
        throw std::runtime_error("字体系统初始化失败,错误信息: " + std::string(SDL_GetError()));
    }
    spdlog::trace("FontManager 初始化成功");
}

FontManager::~FontManager()
{
    clearAllFonts();
    TTF_Quit();
    spdlog::trace("FontManager 退出成功");
}

TTF_Font* FontManager::loadFont(const std::string& path, int size)
{
    if(size <= 0){
        throw std::invalid_argument("字体大小不能小于等于0");
    }

    FontKey key = std::make_pair(path, size);   //根据字体路径和字体大小生成哈希值
    auto it = m_fontMap.find(key);
    if(it != m_fontMap.end()){
        spdlog::warn("字体已存在: {} 字体大小: {}", path, size);
        return it->second.get();
    }

    TTF_Font* pFont = TTF_OpenFont(path.c_str(), static_cast<float>(size));
    if(!pFont){
        spdlog::error("加载字体失败: {} 错误信息: {}", path, SDL_GetError());
        throw std::runtime_error("加载字体失败: " + path + " 错误信息: " + std::string(SDL_GetError()));
        return nullptr;
    }
    m_fontMap.emplace(key, std::unique_ptr<TTF_Font, FontDeleter>(pFont));
    spdlog::debug("加载字体成功: {} 字体大小: {}", path, size);
    return pFont;
}

TTF_Font* FontManager::tryGetFont(const std::string& path, int size)
{
    if(size <= 0){
        throw std::invalid_argument("字体大小不能小于等于0");
    }
    FontKey key = std::make_pair(path, size);   //根据字体路径和字体大小生成哈希值
    auto it = m_fontMap.find(key);
    if(it != m_fontMap.end()){
        spdlog::debug("获取字体成功: {} 字体大小: {}", path, size);
        return it->second.get();
    }
    return loadFont(path, size);    //如果字体不存在，则加载字体
}

void FontManager::unloadFont(const std::string& path, int size)
{
    if(size <= 0){
        throw std::invalid_argument("字体大小不能小于等于0");
    }
    FontKey key = std::make_pair(path, size);   //根据字体路径和字体大小生成哈希值
    auto it = m_fontMap.find(key);
    if(it != m_fontMap.end()){
        spdlog::debug("卸载字体成功: {} 字体大小: {}", path, size);
        m_fontMap.erase(it);
    }
    else{
        spdlog::error("卸载字体失败: {} 字体大小: {}", path, size + " 不存在");
    }
}

void FontManager::clearAllFonts()
{
    if(!m_fontMap.empty()){
        m_fontMap.clear();
        spdlog::trace("FontManager 清除所有字体成功");
    }
}
};
