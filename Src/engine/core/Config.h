#pragma once

#include <string>
#include<unordered_map>
#include<vector>
#include<nlohmann/json_fwd.hpp>

namespace engine::core {

class Config final{

//默认配置 提供默认值
public:
    //窗口配置
    std::string windowTitle = "SunnyLand";
    int windowWidth = 1280;
    int windowHeight = 720;

    //图形设置
    bool vsyncEnabled = false;

    //帧率设置
    int frameRate = 60;

    //声音设置
    float soundVolume = 1.0f;
    float musicVolume = 1.0f;

    //键位设置
    std::unordered_map<std::string, std::vector<std::string>> inputBindings = {
        {"move_up", {"w", "ArrowUp"}},
        {"move_down", {"s", "ArrowDown"}},
        {"move_left", {"a", "ArrowLeft"}},
        {"move_right", {"d", "ArrowRight"}},
        {"jump", {"space"}},
        {"attack", {"K", "5"}},
        {"interact", {"e"}},
        {"pause", {"p", "escape"}},
        {"fullscreen", {"f11"}},
    };


private:
    [[nodiscard]] bool loadConfigFromJson(const nlohmann::json& json);
    void saveConfigToJson(nlohmann::json& json) const;

};
};