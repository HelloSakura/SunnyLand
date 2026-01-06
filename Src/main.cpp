#include "GameApp.h"
#include "LoggerUtil.h"
#include "spdlog/common.h"

int main()  
{
    engine::utils::initLogger(spdlog::level::trace);
    engine::core::GameApp gameApp;
    gameApp.run();
    return 0;
}
