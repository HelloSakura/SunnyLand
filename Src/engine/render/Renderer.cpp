#include "Renderer.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "engine/resource/ResourceManager.h"

#include <stdexcept>

#include <SDL3/SDL.h>
#include <spdlog/spdlog.h>


namespace engine::render {

Renderer::Renderer(SDL_Renderer* pRenderer, engine::resource::ResourceManager* pResourceManager)
    : m_pRenderer(pRenderer), m_pResourceManager(pResourceManager)
{
    spdlog::trace("构造 Render");
    if(pRenderer == nullptr){
        throw std::runtime_error("Renderer 构造失败，SDL_Renderer 为空");   
    }
    if(pResourceManager == nullptr){
        throw std::runtime_error("Renderer 构造失败，ResourceManager 为空");
    }
    setDrawColorUint8(0, 0, 0, 255);
    spdlog::trace("Renderer 构造成功");
}

Renderer::~Renderer()
{
    m_pRenderer = nullptr;
    m_pResourceManager = nullptr;
    spdlog::trace("Renderer 销毁");
}


void Renderer::drawSprite(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scale, double angle)
{
    //获取纹理
    SDL_Texture* pTexture = m_pResourceManager->tryGetTexture(sprite.getTextureID());
    if(pTexture == nullptr){
        spdlog::error("绘制精灵失败，纹理未加载: {}", sprite.getTextureID());
        return;
    }

    //获取矩形区域
    std::optional<SDL_FRect> src_rect = getSpriteRect(sprite);
    if(!src_rect.has_value()){
        spdlog::error("绘制精灵失败，矩形区域未获取: {}", sprite.getTextureID());
        return;
    }

    //应用相机变换
    glm::vec2 world_pos = camera.worldToScreen(position);

    //计算模板矩形
    SDL_FRect dst_rect = {
        world_pos.x,
        world_pos.y,
        src_rect.value().w * scale.x,
        src_rect.value().h * scale.y
    };

    //视口裁剪，不在视口内不绘制
    if(!isRectInViewport(dst_rect)){
        return;
    }

    //绘制，默认旋转点为中心点
    if(!SDL_RenderTextureRotated(m_pRenderer, pTexture, &src_rect.value(), &dst_rect, angle, nullptr, sprite.isFlipX() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)){
        spdlog::error("绘制精灵失败: ID:{}, 错误: {}", sprite.getTextureID(), SDL_GetError());
    }
    spdlog::debug("绘制精灵成功: ID:{}, 位置: ({}, {}), 缩放: ({}, {}), 旋转: {}", sprite.getTextureID(), world_pos.x, world_pos.y, scale.x, scale.y, angle);
}

void Renderer::drawParallax(const Camera& camera, const Sprite& sprite, const glm::vec2& position, const glm::vec2& scaleFactor, const glm::bvec2& repeat, const glm::vec2& scale)
{
    SDL_Texture* pTexture = m_pResourceManager->tryGetTexture(sprite.getTextureID());
    if(pTexture == nullptr){
        spdlog::error("绘制精灵失败，纹理未加载: {}", sprite.getTextureID());
        return;
    }
    std::optional<SDL_FRect> src_rect = getSpriteRect(sprite);
    if(!src_rect.has_value()){
        spdlog::error("绘制精灵失败，矩形区域未获取: {}", sprite.getTextureID());
        return;
    }
    spdlog::debug("获取矩形区域成功: ID:{}, 矩形区域: ({}, {}, {}, {})", sprite.getTextureID(), src_rect.value().x, src_rect.value().y, src_rect.value().w, src_rect.value().h);
    //应用相机变换，需要考虑视差滚动因子
    //得到的是什么？？
    glm::vec2 pos_screen = camera.worldToScreenWithParallax(position, scaleFactor);

    //计算缩放后的纹理尺寸
    float tex_scale_w = src_rect.value().w * scale.x;
    float tex_scale_h = src_rect.value().h * scale.y;

    //滚动时需要考虑图片重复循环的情况
    glm::vec2 start_pos, stop_pos;
    glm::vec2 view_size = camera.getViewportSize();

    if(repeat.x){
        //取模计算出需要重复的次数
        start_pos.x = glm::mod(pos_screen.x, view_size.x) - tex_scale_w;
        stop_pos.x = view_size.x;
    }
    else{
        start_pos.x = pos_screen.x;
        stop_pos.x = glm::min(pos_screen.x + tex_scale_w, view_size.x);
    }

    if(repeat.y){
        start_pos.y = glm::mod(pos_screen.y, view_size.y) - tex_scale_h;
        stop_pos.y = view_size.y;
    }
    else{
        start_pos.y = pos_screen.y;
        stop_pos.y = glm::min(pos_screen.y + tex_scale_h, view_size.y);
    }
    
    for(float y = start_pos.y; y < stop_pos.y; y += tex_scale_h){
        for(float x = start_pos.x; x < stop_pos.x; x += tex_scale_w){
            SDL_FRect dst_rect = {x, y, tex_scale_w, tex_scale_h};
            if(!isRectInViewport(dst_rect)){
                continue;
            }
            if(!SDL_RenderTextureRotated(m_pRenderer, pTexture, &src_rect.value(), &dst_rect, 0.0, nullptr, sprite.isFlipX() ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)){
                spdlog::error("绘制视差滚动背景失败: ID:{}, 错误: {}", sprite.getTextureID(), SDL_GetError());
            }
        }
    }
    spdlog::debug("绘制视差滚动背景成功: ID:{}, 位置: ({}, {}), 缩放: ({}, {})", sprite.getTextureID(), start_pos.x, start_pos.y, scaleFactor.x, scaleFactor.y);
}

//画UI，不需要进行精灵图转换，UI始终在屏幕中固定的位置上绘制
void Renderer::drawUISprite(const Sprite& sprite, const glm::vec2& position, const std::optional<glm::vec2>& size)
{
    SDL_Texture* pTexture = m_pResourceManager->tryGetTexture(sprite.getTextureID());
    if(pTexture == nullptr){
        spdlog::error("绘制精灵失败，纹理未加载: {}", sprite.getTextureID());
        return;
    }
    
    std::optional<SDL_FRect> src_rect = getSpriteRect(sprite);
    if(!src_rect.has_value()){
        spdlog::error("绘制精灵失败，矩形区域未获取: {}", sprite.getTextureID());
        return;
    }

    SDL_FRect dst_rect = {position.x, position.y, 0, 0};
    if(size.has_value()){
        dst_rect.w = size.value().x;
        dst_rect.h = size.value().y;
    }
    else{
        dst_rect.w = src_rect.value().w;
        dst_rect.h = src_rect.value().h;
    }
    if(!SDL_RenderTexture(m_pRenderer, pTexture, &src_rect.value(), &dst_rect)){
        spdlog::error("绘制精灵失败: ID:{}, 错误: {}", sprite.getTextureID(), SDL_GetError());
    }
    spdlog::debug("绘制UI精灵成功: ID:{}, 位置: ({}, {})", sprite.getTextureID(), position.x, position.y);
}

void Renderer::present()
{
    SDL_RenderPresent(m_pRenderer);
}


void Renderer::clearScreen()
{
    if(!SDL_RenderClear(m_pRenderer)){
        spdlog::error("清空屏幕失败: {}", SDL_GetError());
    }
}



void Renderer::setDrawColorUint8(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    if(!SDL_SetRenderDrawColor(m_pRenderer, r, g, b, a)){
        spdlog::error("设置绘制颜色失败: {}", SDL_GetError());
    }
}

void Renderer::setDrawColorFloat(float r, float g, float b, float a)
{
    if(!SDL_SetRenderDrawColorFloat(m_pRenderer, r, g, b, a)){
        spdlog::error("设置绘制颜色失败: {}", SDL_GetError());
    }
}


SDL_Renderer* Renderer::getRenderer() const
{
    return m_pRenderer;
}


std::optional<SDL_FRect> Renderer::getSpriteRect(const Sprite& sprite)
{
    SDL_Texture* pTexture = m_pResourceManager->tryGetTexture(sprite.getTextureID());
    if(pTexture == nullptr){
        spdlog::error("绘制精灵失败，纹理未加载: {}", sprite.getTextureID());
        return std::nullopt;
    }

    std::optional<SDL_FRect> src_rect = sprite.getRect();
    if(src_rect.has_value()){   //存在尺寸，则判断尺寸是否有效
        if(src_rect.value().w <= 0 || src_rect.value().h <= 0){
            spdlog::error("绘制精灵失败，矩形区域尺寸无效: {}", sprite.getTextureID());
            return std::nullopt;
        }
    }

    //否则获取纹理尺寸并返回整个纹理大小
    SDL_FRect res_rect = {0.0f, 0.0f, 0.0f, 0.0f};
    if(!SDL_GetTextureSize(pTexture, &res_rect.w, &res_rect.h)){
        spdlog::error("绘制精灵失败，获取纹理尺寸失败: {}", sprite.getTextureID());
        return std::nullopt;
    }
    return res_rect;
  
}

bool Renderer::isRectInViewport(const SDL_FRect& rect)
{
    return true;
}

}
