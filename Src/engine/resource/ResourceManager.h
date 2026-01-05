/**
 * @file resource_manager.h
 * @author Lucida
 * @date 2026-01-03
 * @brief 资源管理器 代理模式
 */

#pragma once

#include <memory>
#include <string>
#include<glm/glm.hpp>


struct SDL_Renderer;
struct SDL_Texture;
struct TTF_Font;


namespace engine::resource{

class TextureManager;
class FontManager;
class AudioManager;
class Mix_Chunk;
class Mix_Music;


class ResourceManager{
public:
    explicit ResourceManager(SDL_Renderer* pRenderer);
    ~ResourceManager();     //使用前向声明时，使用智能指针时必须知道类的析构函数，如果省略编译器会自动生成内联版本析构函数，但是由于是前向声明，没有类的完整定义，所以会报错

    void clearAll();  //清空所有资源

    //去除其它的构造函数，只需要一个ResourceManager对象
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;

    // ******* 统一资源访问接口 ******
#pragma region texture
    // *** 图片 ***
    SDL_Texture* loadTexture(const std::string& path);     ///< @brief 载入图片纹理
    SDL_Texture* tryGetTexture(const std::string& path);///< @brief 尝试获取已加载的图片纹理，未加载则尝试加载
    void unloadTexture(SDL_Texture* pTexture);///< @brief 卸载图片纹理
    glm::vec2 getTextureSize(const std::string& path);///< @brief 获取图片大小尺寸
    void clearAllTextures();    ///< @brief 清空所有的图片纹理
#pragma endregion texture


#pragma region font
    // *** 字体 ***
    TTF_Font* loadFont(const std::string& path, int size);     ///< @brief 载入字体
    TTF_Font* tryGetFont(const std::string& path, int size);    ///< @brief 尝试获取已加载的字体，未加载则尝试加载
    void unloadFont(TTF_Font* pFont);    ///< @brief 卸载字体
    void clearAllFonts();    ///< @brief 清空所有的字体

#pragma endregion font

#pragma region Sound Effect
    // *** 音效 ***
    Mix_Chunk* loadChunk(const std::string& path);     ///< @brief 载入音效
    Mix_Chunk* tryGetChunk(const std::string& path);///< @brief 尝试获取已加载的音频，未加载则尝试加载
    void unloadChunk(Mix_Chunk* pChunk);///< @brief 卸载音效
    void clearAllChunks();    ///< @brief 清空所有的音效
#pragma endregion Sound Effect

#pragma region music
    // *** 音乐 ***
    Mix_Music* loadMusic(const std::string& path);     ///< @brief 载入音乐
    Mix_Music* tryGetMusic(const std::string& path);///< @brief 尝试获取已加载的音乐，未加载则尝试加载
    void unloadMusic(Mix_Music* pMusic);    ///< @brief 卸载音乐
    void clearAllMusics();                  ///< @brief 清空所有的音乐
#pragma endregion music

private:
    std::unique_ptr<TextureManager> m_upTextureManager;
    std::unique_ptr<FontManager> m_upFontManager;
    std::unique_ptr<AudioManager> m_upAudioManager;
};
};
