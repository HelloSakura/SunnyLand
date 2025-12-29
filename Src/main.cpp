#include "game_app.h"

int main()
{
    engine::core::GameApp gameApp;
    gameApp.run();
    gameApp.~GameApp();
    return 0;
}
