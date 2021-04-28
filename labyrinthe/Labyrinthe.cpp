//
// Created by kevin on 13/04/2021.
//

#include <random>
#include <cstdint>

#include "Labyrinthe.h"

Labyrinthe::Labyrinthe(unsigned short size) : _size(size), _lab(size*size)
{
    generate();
}

const struct Case& Labyrinthe::getPosition(unsigned short x, unsigned short y)
{
    unsigned short offset = x + _size * y;

    return _lab.at(offset);
}

void Labyrinthe::setPosition(const Case &c, unsigned int x, unsigned int y)
{
    unsigned short offset = x + _size * y;
    _lab.at(offset) = c;
}

void Labyrinthe::generate()
{
    std::random_device os_seed;
    const unsigned short seed = os_seed();

    std::mt19937 generator(seed);
    std::uniform_int_distribution<unsigned short> distribution(0, 1);

    for(unsigned int i = 0; i < _size - 1; ++i)
    {
        struct Case c;

        c = getPosition(_size - 1, i);
        c.East = true;
        setPosition(c, _size - 1, i);

        c = getPosition(_size - 1, i + 1);
        c.West = true;
        setPosition(c, _size - 1, i + 1);

        c = getPosition(i, _size - 1);
        c.South = true;
        setPosition(c, i, _size - 1);

        c = getPosition(i + 1, _size - 1);
        c.North = true;
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
