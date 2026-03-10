/**
 * @file PathUtils.cpp
 * @author Lucida
 * @brief 路径工具类实现
 * @version 0.1
 * @date 2025-01-03
 */

#include "PathUtils.h"
#include <spdlog/spdlog.h>

#ifdef _WIN32
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
    #include <limits.h>
#else
    #include <unistd.h>
    #include <limits.h>
    #include <sys/stat.h>
#endif

#include <filesystem>

namespace engine::utils {

std::string PathUtils::s_projectRoot;
bool PathUtils::s_rootInitialized = false;

std::string PathUtils::getExecutableDirectory() {
    std::string exePath;
    
#ifdef _WIN32
    char buffer[MAX_PATH];
    DWORD length = GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    if (length > 0) {
        exePath = buffer;
        // 移除文件名，只保留目录
        size_t lastSlash = exePath.find_last_of("\\/");
        if (lastSlash != std::string::npos) {
            exePath = exePath.substr(0, lastSlash + 1);
        }
    }
#elif defined(__APPLE__)
    char buffer[PATH_MAX];
    uint32_t size = PATH_MAX;
    if (_NSGetExecutablePath(buffer, &size) == 0) {
        char resolved[PATH_MAX];
        if (realpath(buffer, resolved) != nullptr) {
            exePath = resolved;
            size_t lastSlash = exePath.find_last_of("/");
            if (lastSlash != std::string::npos) {
                exePath = exePath.substr(0, lastSlash + 1);
            }
        }
    }
#else
    char buffer[PATH_MAX];
    ssize_t length = readlink("/proc/self/exe", buffer, PATH_MAX);
    if (length > 0) {
        buffer[length] = '\0';
        exePath = buffer;
        // 移除文件名，只保留目录
        size_t lastSlash = exePath.find_last_of("/");
        if (lastSlash != std::string::npos) {
            exePath = exePath.substr(0, lastSlash + 1);
        }
    }
#endif

    return normalizePath(exePath);
}

std::string PathUtils::getProjectRootDirectory() {
    if (s_rootInitialized) {
        return s_projectRoot;
    }

    std::string exeDir = getExecutableDirectory();
    std::filesystem::path currentPath(exeDir);

    // 向上查找，直到找到包含 assets 目录的目录
    int maxDepth = 10;  // 最多向上查找10层，防止无限循环
    for (int i = 0; i < maxDepth; ++i) {
        std::filesystem::path assetsPath = currentPath / "assets";
        if (std::filesystem::exists(assetsPath) && std::filesystem::is_directory(assetsPath)) {
            s_projectRoot = normalizePath(currentPath.string());
            s_rootInitialized = true;
            spdlog::debug("找到项目根目录: {}", s_projectRoot);
            return s_projectRoot;
        }

        // 向上移动一层
        if (currentPath.has_parent_path() && currentPath != currentPath.parent_path()) {
            currentPath = currentPath.parent_path();
        } else {
            break;
        }
    }

    // 如果找不到，使用可执行文件目录
    spdlog::warn("未找到 assets 目录，使用可执行文件目录作为项目根目录: {}", exeDir);
    s_projectRoot = exeDir;
    s_rootInitialized = true;
    return s_projectRoot;
}

std::string PathUtils::getResourcePath(const std::string& relativePath) {
    std::string root = getProjectRootDirectory();
    std::string normalizedRelative = normalizePath(relativePath);
    
    // 移除相对路径开头的 "./" 或 "../"
    while (normalizedRelative.find("../") == 0) {
        normalizedRelative = normalizedRelative.substr(3);
    }
    while (normalizedRelative.find("./") == 0) {
        normalizedRelative = normalizedRelative.substr(2);
    }

    std::filesystem::path fullPath = std::filesystem::path(root) / normalizedRelative;
    std::string result = normalizePath(fullPath.string());
    
    return result;
}

std::string PathUtils::normalizePath(const std::string& path) {
    if (path.empty()) {
        return path;
    }

    std::filesystem::path fsPath(path);
    std::string normalized = fsPath.make_preferred().string();

    // 确保目录路径以分隔符结尾
    if (std::filesystem::is_directory(normalized) || normalized.back() == '\\' || normalized.back() == '/') {
        if (normalized.back() != '\\' && normalized.back() != '/') {
            normalized += std::filesystem::path::preferred_separator;
        }
    }

    return normalized;
}

bool PathUtils::fileExists(const std::string& path) {
    return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
}

} // namespace engine::utils
