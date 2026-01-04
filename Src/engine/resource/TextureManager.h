#include<memory>
#include <string>
#include <unordered_map>
#include<SDL3/SDL_render.h>
#include<glm/glm.hpp>

namespace engine::resource{

class TextureManager{

    friend class ResourceManager;       //声明ResourceManager为友元类，可以访问TextureManager的私有成员

private:
    struct SDLTextureDeleter{   //自定义删除器，配合智能指针实现RAII机制
        void operator()(SDL_Texture* pTexture) const{
            if(pTexture){
                SDL_DestroyTexture(pTexture);
            }
        }
    };


public:
    /**
    * @brief 构造函数
    * @param pRenderer 渲染器指针，必须由ResourceManager创建并传递
    * @throw std::runtime_error 如果渲染器指针为空
    */
    explicit TextureManager(SDL_Renderer* pRenderer);
    ~TextureManager();

    //当前设计中，只需要一个TextureManager实例，因此禁用拷贝构造函数和赋值运算符，防止资源管理错误
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager&&) = delete;

private:
    SDL_Texture* loadTexture(const std::string& path);
    SDL_Texture* tryGetTexture(const std::string& path);
    void unloadTexture(SDL_Texture* pTexture);
    void clearAllTextures();
    glm::vec2 getTextureSize(SDL_Texture* pTexture);


private:
    std::unordered_map<std::string, std::unique_ptr<SDL_Texture, SDLTextureDeleter>> m_textureMap;
    SDL_Renderer* m_pRenderer = nullptr;    //指向渲染器指针
};
}