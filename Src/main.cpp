#include "spdlog/common.h"
#include <spdlog/spdlog.h>

int main()
{
    spdlog::set_level(spdlog::level::err);
    spdlog::info("Hello, World!");
    spdlog::error("Error message");
    spdlog::warn("Warning message");
    spdlog::critical("Critical message");
    spdlog::debug("Debug message");
    spdlog::trace("Trace message");


    spdlog::warn("Warning message");
    spdlog::critical("Critical message");
    spdlog::debug("Debug message");
    spdlog::trace("Trace message");

    return 0;
}
