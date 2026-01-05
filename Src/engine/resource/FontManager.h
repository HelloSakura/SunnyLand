#pragma once

#include <string>
#include <unordered_map>

struct TTF_Font;

namespace engine::resource{

//定义字体键类型
using FontKey = std::pair<std::string, int>;    //字体路径和字体大小

//fontkey 自定义哈希函数
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


    
};

}
