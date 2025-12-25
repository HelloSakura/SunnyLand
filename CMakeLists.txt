cmake_minimum_required(VERSION 3.10)

project(SunnyLand)

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

add_executable(${PROJECT_NAME} Src/main.cpp)

target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

