cmake_minimum_required(VERSION 3.10)

project(SunnyLand)

# 设置 C++ 标准为 C++17（支持嵌套命名空间定义等特性）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 生成 compile_commands.json 用于 IDE 支持
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
    # 添加UTF-8编码支持（spdlog需要）
    if(MSVC)
        add_compile_options(/utf-8)
    endif()
    #定义spdlog需要的宏
    add_compile_definitions(
        SPDLOG_WCHAR_TO_UTF8_SUPPORT #将wchar_t转换为UTF-8编码
    )
endif()


find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(glm REQUIRED)

# 收集所有源文件
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

add_executable(${PROJECT_NAME} 
    src/main.cpp

    src/engine/core/GameApp.cpp
    src/engine/core/GameApp.h
    src/engine/core/XTime.cpp
    src/engine/core/XTime.h
    src/engine/core/Config.cpp
    src/engine/core/Config.h

    src/engine/utils/LoggerUtil.cpp 
    src/engine/utils/LoggerUtil.h
    src/engine/utils/RandomUtils.cpp
    src/engine/utils/RandomUtils.h
    src/engine/utils/MathUtils.h
    src/engine/utils/PathUtils.cpp
    src/engine/utils/PathUtils.h

    src/engine/resource/ResourceManager.cpp
    src/engine/resource/ResourceManager.h
    src/engine/resource/TextureManager.cpp
    src/engine/resource/TextureManager.h
    src/engine/resource/FontManager.cpp
    src/engine/resource/FontManager.h
    src/engine/resource/AudioManager.cpp
    src/engine/resource/AudioManager.h
    src/engine/render/Renderer.cpp
    src/engine/render/Renderer.h
    src/engine/render/Camera.cpp
    src/engine/render/Camera.h
    src/engine/render/Sprite.cpp
    src/engine/render/Sprite.h
)

# 设置目标 C++ 标准
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 设置 Visual Studio 启动项目（避免调试时选择 ALL_BUILD 导致"拒绝访问"错误）
# 设置 Visual Studio 调试工作目录为项目根目录（确保相对路径资源文件能正确加载）
if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# 设置 include 目录
# 1. 添加 src 目录，支持 "engine/xxx/xxx.h" 这样的路径
# 2. 添加各个子目录，支持直接使用文件名（如 "game_app.h"）
# 3. 显式添加 glm 的 include 目录（确保 compile_commands.json 中包含）
#    原因：glm::glm 通过 INTERFACE_LINK_LIBRARIES 间接依赖 glm::glm-header-only，
#    但 CMake 在生成 compile_commands.json 时可能无法正确解析这种间接依赖
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/resource
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/render
)

# 确保 resource 目录被包含在所有编译命令中（包括 compile_commands.json）
# 这对于 IDE 和 clangd 等工具很重要
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE 
        /I"${CMAKE_CURRENT_SOURCE_DIR}/src/engine/resource"
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE 
        -I"${CMAKE_CURRENT_SOURCE_DIR}/src/engine/resource"
    )
endif()

target_link_libraries(${PROJECT_NAME} 
                    spdlog::spdlog
                    nlohmann_json::nlohmann_json
                    SDL3::SDL3
                    SDL3_image::SDL3_image
                    SDL3_ttf::SDL3_ttf
                    SDL3_mixer::SDL3_mixer
                    glm::glm
                    glm::glm-header-only
)

# 链接 filesystem 库（C++17 std::filesystem）
# 注意：MSVC 和 GCC >= 9.0 不需要额外链接，GCC < 9.0 需要链接 stdc++fs
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS "9.0")
    target_link_libraries(${PROJECT_NAME} PRIVATE stdc++fs)
endif()


# 显式添加 glm 的 include 目录（确保 compile_commands.json 中包含）
# 原因：glm::glm 通过 INTERFACE_LINK_LIBRARIES 间接依赖 glm::glm-header-only，
# 但 CMake 在生成 compile_commands.json 时可能无法正确解析这种间接依赖
# 使用 PUBLIC 作用域确保所有依赖此目标的目标也能看到这个路径
if(WIN32)
    target_include_directories(${PROJECT_NAME} PUBLIC "C:/Library/glm/include")
elseif(APPLE)
    target_include_directories(${PROJECT_NAME} PUBLIC "/opt/homebrew/include/glm")
elseif(UNIX)
    target_include_directories(${PROJECT_NAME} PUBLIC "/usr/include/glm")
endif()


# 文件末尾添加
if(NOT TARGET clean-all)
    add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
        COMMENT "删除整个构建目录"
    )
endif()

# 生成外部清理脚本（跨平台支持）
if(WIN32)
    # Windows 系统生成 .bat 脚本
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cleanup.bat.in
        ${CMAKE_BINARY_DIR}/cleanup.bat
        @ONLY
    )
    set(CLEANUP_SCRIPT "${CMAKE_BINARY_DIR}/cleanup.bat")
else()
    # Unix/Linux 系统生成 .sh 脚本
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/cleanup.in
        ${CMAKE_BINARY_DIR}/cleanup.sh
        @ONLY
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
    )
    set(CLEANUP_SCRIPT "${CMAKE_BINARY_DIR}/cleanup.sh")
endif()

# 添加到清理目标
add_custom_target(generate-clean-script
    COMMAND ${CMAKE_COMMAND} -E echo "清理脚本已生成到 ${CLEANUP_SCRIPT}"
    DEPENDS ${CLEANUP_SCRIPT}
    COMMENT "生成清理脚本"
) 