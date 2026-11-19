#pragma once

#include <cstdint>
#include <nlohmann/json.hpp>
#include <string>

namespace kotte
{
    struct KeyBindings
    {
        std::string move_up;
        std::string move_down;
        std::string move_left;
        std::string move_right;
        std::string place_bomb;
        std::string quit;
    };

    struct Config
    {
        std::int64_t tile_map_width = 0;
        std::int64_t tile_map_height = 0;
        std::int64_t seed = 0;
        KeyBindings input;
    };

    void from_json(const nlohmann::json& json, KeyBindings& bindings);
    void to_json(nlohmann::json& json, const KeyBindings& bindings);

    void from_json(const nlohmann::json& json, Config& config);
    void to_json(nlohmann::json& json, const Config& config);

    Config default_config();

    Config load_config(const std::string& config_path, const std::string& schema_path);
}
