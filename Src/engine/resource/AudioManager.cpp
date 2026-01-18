#include "AudioManager.h"
#include "SDL3_mixer/SDL_mixer.h"
#include<stdexcept>
#include<spdlog/spdlog.h>

namespace engine::resource{

AudioManager::AudioManager()
{
    //初始化格式
    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0){
        spdlog::error("初始化音频系统失败: {}", SDL_GetError());
        throw std::runtime_error("初始化音频系统失败");
    }

    //打开音频设备，使用默认参数，44100Hz，16位，2通道，2048样本
    if(!Mix_OpenAudio(0, nullptr)){
        spdlog::error("打开音频设备失败: {}", SDL_GetError());
        throw std::runtime_error("打开音频设备失败");
    }

    
    spdlog::trace("AudioManager 初始化成功");
}

AudioManager::~AudioManager()
{
    //停止所有音效和音乐
    Mix_HaltChannel(-1);
    Mix_HaltMusic();

    //清空音效和音乐
    clearAllChunks();
    clearAllMusics();

    //关闭音频设备
    Mix_CloseAudio();

    //退出音频系统
    Mix_Quit();
    spdlog::trace("AudioManager 退出成功");
}


Mix_Chunk* AudioManager::loadChunk(const std::string& path)
{
    auto it = m_chunkMap.find(path);
    if(it != m_chunkMap.end()){
        spdlog::warn("音效已存在: {}", path);
        return it->second.get();
    }
    Mix_Chunk* pChunk = Mix_LoadWAV(path.c_str());
    if(!pChunk){
        spdlog::error("加载音效失败: {}", path);
        throw std::runtime_error("加载音效失败: " + path + " 错误信息: " + std::string(SDL_GetError()));
        return nullptr;
    }
    m_chunkMap.emplace(path, std::unique_ptr<Mix_Chunk, MixChunkDeleter>(pChunk));
    spdlog::debug("加载音效成功: {}", path);
    return pChunk;   
}

Mix_Chunk* AudioManager::tryGetChunk(const std::string& path)
{
    auto it = m_chunkMap.find(path);
    if(it != m_chunkMap.end()){
        spdlog::debug("获取音效成功: {}", path);
        return it->second.get();
    }
    return loadChunk(path);
}

void AudioManager::unloadChunk(const std::string& path)
{
    auto it = m_chunkMap.find(path);
    if(it != m_chunkMap.end()){
        m_chunkMap.erase(it);
        spdlog::debug("卸载音效成功: {}", path);
    }
    else{
        spdlog::error("卸载音效失败: {}", path + " 不存在");
    }
}

void AudioManager::clearAllChunks()
{
    m_chunkMap.clear();
}

Mix_Music* AudioManager::loadMusic(const std::string& path)
{
    auto it = m_musicMap.find(path);
    if(it != m_musicMap.end()){
        spdlog::warn("音乐已存在: {}", path);
        return it->second.get();
    }
    Mix_Music* pMusic = Mix_LoadMUS(path.c_str());
    if(!pMusic){
        spdlog::error("加载音乐失败: {}", path);
        throw std::runtime_error("加载音乐失败: " + path + " 错误信息: " + std::string(SDL_GetError()));
        return nullptr;
    }
    m_musicMap.emplace(path, std::unique_ptr<Mix_Music, MixMusicDeleter>(pMusic));
    spdlog::debug("加载音乐成功: {}", path);
    return pMusic;   
}

Mix_Music* AudioManager::tryGetMusic(const std::string& path)
{
    auto it = m_musicMap.find(path);
    if(it != m_musicMap.end()){
        spdlog::debug("获取音乐成功: {}", path);
        return it->second.get();
    }
    return loadMusic(path);
}


void AudioManager::unloadMusic(const std::string& path)
{
    auto it = m_musicMap.find(path);
    if(it != m_musicMap.end()){
        m_musicMap.erase(it);
        spdlog::debug("卸载音乐成功: {}", path);
    }
    else{
        spdlog::error("卸载音乐失败: {}", path + " 不存在");
    }
}

void AudioManager::clearAllMusics()
{
    m_musicMap.clear();
}
};

