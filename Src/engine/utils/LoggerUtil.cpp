/**
 * @file logger_util.cpp
 * @author lucida
 * @brief 日志工具类实现
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 */

#include "LoggerUtil.h"
#include <spdlog/spdlog.h>

#ifdef _WIN32
#include <windows.h>
#endif

namespace engine::utils {

void initLogger(spdlog::level::level_enum level)
{
#ifdef _WIN32
    // 设置 Windows 控制台代码页为 UTF-8，解决中文乱码问题
    SetConsoleOutputCP(65001);  // UTF-8 代码页
    SetConsoleCP(65001);        // 同时设置输入代码页
#endif
    
    // 初始化 spdlog：设置日志级别为 trace，这样所有级别的日志都会输出
    spdlog::set_level(level);
    // 设置日志格式：时间戳 + 级别 + 消息
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
}

}

