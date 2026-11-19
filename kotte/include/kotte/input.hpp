#pragma once

#include "kotte/config.hpp"

#include <raylib.h>
#include <string>
#include <unordered_map>
#include <vector>
//tommi's codes
//enum the Action q. what is enum class? say enmu class Action{}

//enum the Axis

//enum the Direction
// 
//struct InputContext
//{
// void bind(Action action, int key
// void bind(Axis, int positive, int negative
// void bind(direction, left, right, up, down
// 
// bool down(Action action) const;
// bool pressed(Action action) const;
// 
// get Axis
// Vector2 direction
// }

//private: AXISbIDING 
//STRUCT directionalBinding
//then use the unordered_map to store Action, iny, Axis, AxisBinding, Direction, DirectionBinding

namespace kotte
{
    [[nodiscard]] int key_from_name(const std::string& name);
    enum class Action
    {
        quit,
        place_bomb
    };

    enum class Direction
    {
        move
    };

    class Input final
    {
    public:
        explicit Input(const Config& config);

        void bind(Action action, std::vector<int> keys);
        void bind(
            Direction direction,
            std::vector<int> up,
            std::vector<int> down,
            std::vector<int> left,
            std::vector<int> right);

        [[nodiscard]] bool down(Action action) const noexcept;
        [[nodiscard]] bool pressed(Action action) const noexcept;
        [[nodiscard]] Vector2 get(Direction direction) const noexcept;

    private:
        struct DirectionBinding final
        {
            std::vector<int> up;
            std::vector<int> down;
            std::vector<int> left;
            std::vector<int> right;
        };

        [[nodiscard]] static bool any_down(const std::vector<int>& keys) noexcept;
        [[nodiscard]] static bool any_pressed(const std::vector<int>& keys) noexcept;

        std::unordered_map<Action, std::vector<int>> actions_;
        std::unordered_map<Direction, DirectionBinding> directions_;
    };
}
