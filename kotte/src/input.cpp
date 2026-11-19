#include "kotte/input.hpp"
#include <raymath.h>
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
