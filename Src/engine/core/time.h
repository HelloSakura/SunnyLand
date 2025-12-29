#pragma once
#include<SDL3/SDL_stdinc.h>

namespace engine::core{


    /**
     * @brief 时间类
     * @details 用于获取时间相关的信息
     */
class Time{
private:
    Uint64 m_u64LastTime;         //上一帧结束时间
    Uint64 m_u64frameStartTime;   //帧开始时间
    double m_deltaTime;    //时间差
    double m_timeScale;    //时间缩放因子
};

}