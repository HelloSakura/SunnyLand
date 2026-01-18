/**
 * @file PathUtils.h
 * @author Lucida
 * @brief 路径工具类，用于获取可执行文件目录和构建资源路径
 * @version 0.1
 * @date 2025-01-03
 */

#pragma once

#include <string>

namespace engine::utils {

/**
 * @brief 路径工具类
 * 提供跨平台的路径解析功能，确保资源文件路径在不同环境下都能正确访问
 */
class PathUtils {
public:
    /**
     * @brief 获取可执行文件所在目录
     * @return 可执行文件所在目录的绝对路径（以路径分隔符结尾）
     */
    static std::string getExecutableDirectory();

    /**
     * @brief 获取项目根目录（assets 目录的父目录）
     * 通过向上查找 assets 目录来确定项目根目录
     * @return 项目根目录的绝对路径（以路径分隔符结尾）
     */
    static std::string getProjectRootDirectory();

    /**
     * @brief 获取资源文件路径
     * 将相对路径（如 "assets/textures/xxx.png"）转换为绝对路径
     * @param relativePath 相对于项目根目录的资源路径（如 "assets/textures/xxx.png"）
     * @return 资源文件的绝对路径
     */
    static std::string getResourcePath(const std::string& relativePath);

    /**
     * @brief 规范化路径分隔符
     * 将路径中的分隔符统一为当前平台的标准格式
     * @param path 原始路径
     * @return 规范化后的路径
     */
    static std::string normalizePath(const std::string& path);

    /**
     * @brief 检查文件是否存在
     * @param path 文件路径
     * @return 文件是否存在
     */
    static bool fileExists(const std::string& path);

private:
    static std::string s_projectRoot;  // 缓存的项目根目录
    static bool s_rootInitialized;      // 是否已初始化项目根目录
};

} // namespace engine::utils
