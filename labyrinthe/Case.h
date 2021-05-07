//
// Created by kevin on 13/04/2021.
//

#ifndef LABYRINTH_CASE_H
#define LABYRINTH_CASE_H

#include <cstdint>

enum Direction : uint8_t
{
    North   = 0x1,
    South   = 0x2,
    East    = 0x4,
    West    = 0x8
};

struct Case
{
    bool North, South, East, West;

    Case() : North(false), South(false), East(false), West(false) {}

    explicit Case(uint8_t bitmap) : Case()
    {
        set(bitmap);
    }

    void set(uint8_t value)
    {
        this->North = value & Direction::North;
        this->South = value & Direction::South;
        this->East = value & Direction::East;
        this->West = value & Direction::West;
    }

    void open(uint8_t value)
    {
        this->North = (value & Direction::North) != 0 || this->North;
        this->South = (value & Direction::South) != 0 || this->South;
        this->East = (value & Direction::East) != 0 || this->East;
        this->West = (value & Direction::West) != 0 || this->West;
    }

    bool isFalse() const
    {
        return !this->North && !this->South && !this->East && !this->West;
    }

    bool operator !() const
    {
        return isFalse();
    }
};

#endif //LABYRINTH_CASE_H
