#include "FontManager.h"

namespace engine::resource{

FontManager::FontManager()
{
    
}

FontManager::~FontManager()
{
}

TTF_Font* FontManager::loadFont(const std::string& path, int size)
{
    return nullptr;
}

TTF_Font* FontManager::tryGetFont(const std::string& path, int size)
{
    return nullptr;
}

void FontManager::unloadFont(const std::string& path, int size)
{
    
}

void FontManager::clearAllFonts()
{

}
};
