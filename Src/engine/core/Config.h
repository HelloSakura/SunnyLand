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

    explicit Config(const std::string& configPath);
    ~Config();

    //禁止拷贝和赋值
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    Config(Config&&) = delete;
    Config& operator=(Config&&) = delete;

    //获取配置
    [[nodiscard]] const std::string& getWindowTitle() const;
    [[nodiscard]] int getWindowWidth() const;
    [[nodiscard]] int getWindowHeight() const;
    [[nodiscard]] bool isVsyncEnabled() const;
    [[nodiscard]] int getFrameRate() const;
    [[nodiscard]] float getSoundVolume() const;
    [[nodiscard]] float getMusicVolume() const;
    [[nodiscard]] const std::unordered_map<std::string, std::vector<std::string>>& getInputBindings() const;

    //设置配置
    void setWindowTitle(const std::string& title);
    void setWindowWidth(int width);
    void setWindowHeight(int height);
    void setVsyncEnabled(bool enabled);

    bool loadFromFile(const std::string& filePath);
    [[nodiscard]] bool saveToFile(const std::string& filePath) const;

private:
    void fromJson(const nlohmann::json& json);      //反序列化，从JSON配置加载
    nlohmann::ordered_json toJson() const;          //序列化，将当前配置转换为JSON格式
};
};