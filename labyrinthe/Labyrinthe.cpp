//
// Created by kevin on 13/04/2021.
//

#include <random>
#include <cstdint>

#include "Labyrinthe.h"

Labyrinthe::Labyrinthe(uint8_t size) : _size(size), _lab(size*size)
{
    generate();
}

const struct Case& Labyrinthe::getPosition(uint8_t x, uint8_t y)
{
    uint8_t offset = x + _size * y;

    return _lab.at(offset);
}

void Labyrinthe::setPosition(const Case &c, uint8_t x, uint8_t y)
{
    uint64_t offset = x + _size * y;
    _lab.at(offset) = c;
}

void Labyrinthe::generate()
{
    std::random_device os_seed;
    const uint8_t seed = os_seed();

    std::mt19937 generator(seed);
    std::uniform_int_distribution<uint8_t> distribution(0, 1);

    for(unsigned int i = 0; i < _size - 1; ++i)
    {
        struct Case c;

        c.set(Direction::East);
        setPosition(c, _size - 1, i);

        c.set(Direction::West);
        setPosition(c, _size - 1, i + 1);

        c.set(Direction::South);
        setPosition(c, i, _size - 1);

        c.set(Direction::North);
        setPosition(c, i + 1, _size - 1);
    }

    for(uint8_t i = 0; i < _size - 1; ++i)
    {
        for(uint8_t j = 0; j < _size - 1; ++j)
        {
            struct Case une = getPosition(i, j);
            struct Case deux = getPosition(i + 1, j);

            if(distribution(generator) == 0)
            {
                une.South = true;
                deux.North = true;
            }
            else
            {
                une.East = true;
                deux.West = true;
            }

            setPosition(une, i, j);
            setPosition(deux, i + 1, j);
        }
    }
}
