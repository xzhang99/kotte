#include "kotte/application.hpp"
#include "kotte/config.hpp"

#include <exception>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    try
    {
        const kotte::Config config =
            kotte::load_config("config/config.json", "config/config_schema.json");

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
