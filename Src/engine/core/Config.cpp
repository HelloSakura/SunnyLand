#include "Config.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <spdlog/spdlog.h>

namespace engine::core {

Config::Config(const std::string& configPath)
{
    loadFromFile(configPath);
}

Config::~Config()
{
    //保存配置到文件
}

// -------------------------
// Getter / Setter
// -------------------------

const std::string& Config::getWindowTitle() const
{
    return windowTitle;
}

int Config::getWindowWidth() const
{
    return windowWidth;
}

int Config::getWindowHeight() const
{
    return windowHeight;
}

bool Config::isVsyncEnabled() const
{
    return vsyncEnabled;
}

int Config::getFrameRate() const
{
    return frameRate;
}

float Config::getSoundVolume() const
{
    return soundVolume;
}

float Config::getMusicVolume() const
{
    return musicVolume;
}

const std::unordered_map<std::string, std::vector<std::string>>& Config::getInputBindings() const
{
    return inputBindings;
}

void Config::setWindowTitle(const std::string& title)
{
    windowTitle = title;
}

void Config::setWindowWidth(int width)
{
    windowWidth = width;
}

void Config::setWindowHeight(int height)
{
    windowHeight = height;
}

void Config::setVsyncEnabled(bool enabled)
{
    vsyncEnabled = enabled;
}

bool Config::loadFromFile(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        spdlog::error("Failed to open config file: {}", filePath);
        //try to create the file & save default config
        if(!saveToFile(filePath)){
            spdlog::error("Failed to create default config file: {}", filePath);
            return false;
        }
        return false;
    }

    try{
        nlohmann::json json;
        file >> json;
        fromJson(json);
        return true;
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to parse config file: {}", e.what());
    }
    return false;
}

bool Config::saveToFile(const std::string& filePath) const
{
    std::ofstream file(filePath);
    if (!file.is_open()){
        spdlog::error("Failed to open config file: {}", filePath);
        return false;
    }
   
    try{
        nlohmann::ordered_json json = toJson();
        file << json.dump(4);
        spdlog::trace("Config file saved successfully: {}", filePath);
        return true;
    }
    catch (const std::exception& e)    {
        spdlog::error("Failed to save config file: {}", e.what());
    }
    return false;
}

void Config::fromJson(const nlohmann::json& json)
{
    //逐键值队拆解
    if(json.contains("window")){
        const auto& windowCfg = json["window"];
        windowTitle = windowCfg.value("title", windowTitle);
        windowWidth = windowCfg.value("width", windowWidth);
        windowHeight = windowCfg.value("height", windowHeight);
        windowResizable = windowCfg.value("resizable", windowResizable);
    }

    if(json.contains("graphics")){
        const auto& graphicsCfg = json["graphics"];
        vsyncEnabled = graphicsCfg.value("vsync", vsyncEnabled);
        frameRate = graphicsCfg.value("frame_rate", frameRate);
        if(frameRate < 0){
            spdlog::warn("Invalid frame rate: {}, setting to 0 (no limits)", frameRate);
            frameRate = 0;
        }
    }

    if(json.contains("audio")){
        const auto& audioCfg = json["audio"];
        soundVolume = audioCfg.value("sound_volume", soundVolume);
        musicVolume = audioCfg.value("music_volume", musicVolume);
    }

    if(json.contains("input_mapping") && json["input_mapping"].is_object()){
        const auto& mappingJson = json["input_mapping"];
        try{
            auto inputMappings = mappingJson.get<std::unordered_map<std::string, std::vector<std::string>>>();
            inputBindings = std::move(inputMappings);
            spdlog::trace("Input mappings loaded successfully");
        }
        catch(const std::exception& e){
            spdlog::error("Failed to parse input mappings: {}", e.what());
        }
    }
    else{
        spdlog::warn("Input mappings are not properly formatted, using default bindings");
    }
}

nlohmann::ordered_json Config::toJson() const
{
    return nlohmann::ordered_json{
        {"window", {
            {"title", windowTitle},
            {"width", windowWidth},
            {"height", windowHeight},
            {"resizable", windowResizable},
        }},
        {"graphics", {
            {"vsync", vsyncEnabled},
            {"frame_rate", frameRate},
        }},
        {"audio", {
            {"sound_volume", soundVolume},
            {"music_volume", musicVolume},
        }},
        {"input_mapping", inputBindings},
    };
}   
}