#include "kotte/application.hpp"
#include "kotte/config.hpp"

#include <nlohmann/json.hpp>

#include <exception>
#include <fstream>
#include <stdexcept>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    try
    {
        std::ifstream config_file{ "config/config.json" };
        if (!config_file.is_open())
        {
            throw std::runtime_error{ "Could not open config/config.json" };
        }

        const quicktype::Config config = nlohmann::json::parse(config_file).get<quicktype::Config>();

        kotte::Application app{ 1280, 720, "Kotte", config };
        app.run();
    }
    catch (const std::exception& error)
    {
        TraceLog(LOG_ERROR, "%s", error.what());
        return 1;
    }
    return 0;
}
