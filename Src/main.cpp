#include<fstream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

int main()
{
    try{
        std::ifstream configFile("../../Src/test.json");
        nlohmann::json config = nlohmann::json::parse(configFile);
        configFile.close();
        spdlog::info("json load success");

        std::string gameName = config["game"]["name"].get<std::string>();
        spdlog::info("gameName: {}", gameName);
    }catch(const std::exception& e){
        spdlog::error("Error: {}", e.what());
        return -1;
    }
    return 0;
}
