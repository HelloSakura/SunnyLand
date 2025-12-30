#pragma once

#include <SDL3/SDL_stdinc.h>


namespace engine::core{

class XTime{
public:
    XTime();
    ~XTime();

    //更新
    void update();
    //获取时间间隔
    double getDeltaTime() const;
    //获取未经缩放的时间间隔
    double getUnscaledDeltaTime() const;
    //设置缩放因子
    void setTimeScaleFactor(double factor);
    double getTimeScaleFactor();
    //设置目标帧率
    void setTargetFps(int fps);
    

private:
    //限制帧率
    void limitFrameRate(double cur_delta_time);

private:
    Uint64 m_u64LastTime;           //上一帧时间戳
    Uint64 m_u64FrameStartTime;     //帧开始时间戳
    double m_deltaTime;             //帧间隔
    double m_timeScaleFactor;       //缩放因子

    //帧率限制相关
    int m_targetFps;                //目标帧率
    double m_targetFrameTime;       //目标帧时间    
};

}