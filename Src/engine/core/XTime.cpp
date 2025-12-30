#include "XTime.h"
#include "SDL3/SDL_timer.h"
#include<spdlog/spdlog.h>

namespace engine::core{

XTime::XTime()
{
    m_u64LastTime = SDL_GetTicksNS();
    m_u64FrameStartTime = m_u64LastTime;
    spdlog::trace("XTime 初始化, last time: {}", m_u64LastTime);
}

XTime::~XTime()
{
    spdlog::trace("XTime 销毁");
}

void XTime::update()
{
    m_u64FrameStartTime = SDL_GetTicksNS();
    auto curDeltaTime = static_cast<double>(m_u64FrameStartTime - m_u64LastTime) / 1000000.0;
    spdlog::trace("XTime 更新, curDeltaTime: {} ms", curDeltaTime);
    if(m_targetFrameTime > 0){   //如果设置了限制帧率
        //获取当前帧实际的运行时间，处理器跑太快了，实际帧的时间可能比较短，需要等待，保证每台机器的帧率一致
        limitFrameRate(curDeltaTime);
    }
    else{
        m_deltaTime = curDeltaTime;
    }

    m_u64LastTime = SDL_GetTicksNS();
}




//限制帧率
void XTime::limitFrameRate(double curDeltaTime)
{
    //每帧都会处理
    if(curDeltaTime < m_targetFrameTime){
        double sleepTime = m_targetFrameTime - curDeltaTime;
        spdlog::trace("XTime 等待, sleepTime: {} ms", sleepTime);
        SDL_DelayNS(static_cast<Uint64>(sleepTime * 1000000.0));   //调整睡眠时间，使得帧率符合目标帧率
        m_deltaTime = static_cast<double>(SDL_GetTicksNS() - m_u64LastTime) / 1000000.0;
        spdlog::trace("XTime 限制帧率, m_deltaTime: {} ms", m_deltaTime);
    }
}


double XTime::getDeltaTime() const
{
    return m_deltaTime * m_timeScaleFactor;
}

double XTime::getUnscaledDeltaTime() const
{
    return m_deltaTime;
}

void XTime::setTargetFps(int fps)
{
    m_targetFps = fps;
    m_targetFrameTime = static_cast<double>(1000.0 / m_targetFps);

    spdlog::trace("XTime 设置目标帧率: {} fps, 目标帧时间: {} ms", m_targetFps, m_targetFrameTime);
}


}
