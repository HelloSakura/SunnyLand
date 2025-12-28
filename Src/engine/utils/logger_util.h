/**
 * @file logger_util.h
 * @author lucida
 * @brief 日志工具类，负责初始化 spdlog 和控制台编码设置
 * @version 0.1
 * @date 2025-12-28
 * 
 * @copyright Copyright (c) 2025
 */

#pragma once

namespace engine::utils {

/**
 * @brief 初始化日志系统
 * @details 设置 spdlog 日志级别和格式，并配置 Windows 控制台 UTF-8 编码
 */
void initLogger();

}

