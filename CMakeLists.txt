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

# 收集所有源文件
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.h"
)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/engine/core/game_app.cpp
    src/engine/core/game_app.h
    src/engine/utils/logger_util.cpp
    src/engine/utils/logger_util.h
)

# 设置目标 C++ 标准
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

# 设置 include 目录
# 1. 添加 src 目录，支持 "engine/xxx/xxx.h" 这样的路径
# 2. 添加各个子目录，支持直接使用文件名（如 "game_app.h"）
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/utils
)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} SDL3::SDL3)

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