#include "kotte/input.hpp"

#include <raymath.h>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
//tommi's code
// InputContext {
// actions[action] = key;
// }

// InputContext {
// axes = AxisBinding{positive, negative};
// }

// InputContext {
// auto it = actions.find(action);
// return it != actions.end() &&IsKeDown( (*it).second);
// }


// InputContext {
// auto it = actions.find(action);
// return it != actions.end() &&IsKeyPressed( it -> second);
// }

// InputContext {
// auto it = axis.find(axis);
// if it == axes.end
// return 0.0f;
// 
// const auto & b = it ->second;
// float value = float (IsKeyDown(b.postive)) - b.negative;
// return value
// }

// InputContext {
// auto it = directions.find(direction);
// if it == directions.end
// return 0.0f;
// 
// const auto & b = it ->second;
// Vector2 value {
// .x = float (IsKeyDown(right))-float (IsKeyDown(left)),
// .y
// };
// normalized;
// return Vector2
// }


namespace kotte
{
    int key_from_name(const std::string& name)
    {
        static const std::unordered_map<std::string, int> keys{
            { "A", KEY_A },{ "B", KEY_B },{ "C", KEY_C },{ "D", KEY_D },{ "E", KEY_E },
            { "F", KEY_F },{ "G", KEY_G },{ "H", KEY_H },{ "I", KEY_I },{ "J", KEY_J },
            { "K", KEY_K },{ "L", KEY_L },{ "M", KEY_M },{ "N", KEY_N },{ "O", KEY_O },
            { "P", KEY_P },{ "Q", KEY_Q },{ "R", KEY_R },{ "S", KEY_S },{ "T", KEY_T },
            { "U", KEY_U },{ "V", KEY_V },{ "W", KEY_W },{ "X", KEY_X },{ "Y", KEY_Y },
            { "Z", KEY_Z },
            { "ZERO", KEY_ZERO },{ "ONE", KEY_ONE },{ "TWO", KEY_TWO },{ "THREE", KEY_THREE },
            { "FOUR", KEY_FOUR },{ "FIVE", KEY_FIVE },{ "SIX", KEY_SIX },{ "SEVEN", KEY_SEVEN },
            { "EIGHT", KEY_EIGHT },{ "NINE", KEY_NINE },
            { "UP", KEY_UP },{ "DOWN", KEY_DOWN },{ "LEFT", KEY_LEFT },{ "RIGHT", KEY_RIGHT },
            { "SPACE", KEY_SPACE },{ "ESCAPE", KEY_ESCAPE },{ "ENTER", KEY_ENTER },{ "TAB", KEY_TAB },
            { "BACKSPACE", KEY_BACKSPACE },
            { "LEFT_SHIFT", KEY_LEFT_SHIFT },{ "RIGHT_SHIFT", KEY_RIGHT_SHIFT },
            { "LEFT_CONTROL", KEY_LEFT_CONTROL },{ "RIGHT_CONTROL", KEY_RIGHT_CONTROL },
            { "LEFT_ALT", KEY_LEFT_ALT },{ "RIGHT_ALT", KEY_RIGHT_ALT },
        };

        const auto it = keys.find(name);
        if (it == keys.end())
        {
            throw std::runtime_error{ "Unknown key binding: \"" + name + "\"" };
        }
        return it->second;
    }

    Input::Input(const Config& config)
    {
        bind(Action::place_bomb, { key_from_name(config.input.place_bomb) });
        // TODO: bind quit and Direction::move from config.input the same way,
        // once Application stops binding them itself.
    }

    void Input::bind(Action action, std::vector<int> keys)
    {
        actions_[action] = std::move(keys);
    }

    void Input::bind(
        Direction direction,
        std::vector<int> up,
        std::vector<int> down,
        std::vector<int> left,
        std::vector<int> right)
    {
        directions_[direction] = DirectionBinding{
            std::move(up),
            std::move(down),
            std::move(left),
            std::move(right)
        };
    }

    bool Input::down(Action action) const noexcept
    {
        const auto it = actions_.find(action);
        return it != actions_.end() && any_down(it->second);
    }

    bool Input::pressed(Action action) const noexcept
    {
        const auto it = actions_.find(action);
        return it != actions_.end() && any_pressed(it->second);
    }

    Vector2 Input::get(Direction direction) const noexcept
    {
        const auto it = directions_.find(direction);
        if (it == directions_.end())
        {
            return Vector2{ 0.0f, 0.0f };
        }

        const auto& binding = it->second;
        Vector2 value{
            static_cast<float>(any_down(binding.right)) - static_cast<float>(any_down(binding.left)),
            static_cast<float>(any_down(binding.down)) - static_cast<float>(any_down(binding.up))
        };

        if (value.x != 0.0f || value.y != 0.0f)
        {
            value = Vector2Normalize(value);
        }

        return value;
    }

    bool Input::any_down(const std::vector<int>& keys) noexcept
    {
        for (const int key : keys)
        {
            if (IsKeyDown(key))
            {
                return true;
            }
        }
        return false;
    }

    bool Input::any_pressed(const std::vector<int>& keys) noexcept
    {
        for (const int key : keys)
        {
            if (IsKeyPressed(key))
            {
                return true;
            }
        }
        return false;
    }
}
