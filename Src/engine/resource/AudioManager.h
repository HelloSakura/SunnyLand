#pragma once
#include <string>
#include<SDL3_mixer/SDL_mixer.h>
#include<unordered_map>
#include<memory>

namespace engine::resource{

class AudioManager
{
    friend class ResourceManager;
public:
    AudioManager();
    ~AudioManager();

private:
    //处理音效
    Mix_Chunk* loadChunk(const std::string& path);
    Mix_Chunk* tryGetChunk(const std::string& path);
    void unloadChunk(const std::string& path);
    void clearAllChunks();

    //处理音乐
    Mix_Music* loadMusic(const std::string& path);
    Mix_Music* tryGetMusic(const std::string& path);
    void unloadMusic(const std::string& path);
    void clearAllMusics();

private:
    //自定义删除器
    struct MixChunkDeleter{
        void operator()(Mix_Chunk* pChunk) const{
            if(pChunk){
                Mix_FreeChunk(pChunk);
            }
        }
    };
    struct MixMusicDeleter{
        void operator()(Mix_Music* pMusic) const{
            if(pMusic){
                Mix_FreeMusic(pMusic);
            }
        }
    };

    std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, MixChunkDeleter>> m_chunkMap;
    std::unordered_map<std::string, std::unique_ptr<Mix_Music, MixMusicDeleter>> m_musicMap;
};

}


