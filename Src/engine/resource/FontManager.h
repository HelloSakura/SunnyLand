#pragma once

#include <string>
#include <functional>
#include <utility>
#include <memory>
#include <unordered_map>
#include <SDL3_ttf/SDL_ttf.h>

struct TTF_Font;

namespace engine::resource{

//定义字体键类型
using FontKey = std::pair<std::string, int>;    //字体路径和字体大小

//fontkey 自定义哈希函数 根据字体路径和字体大小生成哈希值
struct FontKeyHash{
    size_t operator()(const FontKey& key) const{
        return std::hash<std::string>()(key.first) ^ std::hash<int>()(key.second);
    }
};

class FontManager
{
    friend class ResourceManager;
public:
    FontManager();
    ~FontManager();

private:
    TTF_Font* loadFont(const std::string& path, int size);
    TTF_Font* tryGetFont(const std::string& path, int size);
    void unloadFont(const std::string& path, int size);
    void clearAllFonts();

private:
    struct FontDeleter{
        void operator()(TTF_Font* font) const{
            if(font){
                TTF_CloseFont(font);
                font = nullptr;
            }
        }
    };

    //使用智能指针管理字体资源，配合自定义删除器实现RAII机制
    //使用unordered_map管理字体资源，根据字体路径和字体大小生成哈希值，使用自定义的类型需要自定义哈希函数
    std::unordered_map<FontKey, std::unique_ptr<TTF_Font, FontDeleter>, FontKeyHash> m_fontMap;
    
};

}
