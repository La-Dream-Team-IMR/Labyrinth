//
// Created by kevin on 13/04/2021.
//

#include <random>
#include <cstdint>
#include <iostream>

#include "Labyrinthe.h"

Labyrinthe::Labyrinthe(uint8_t size) : _size(size), _lab(size * size)
{
    generate();
    //factice_generate();
}

void Labyrinthe::factice_generate() {
    Case c;
    c.set(Direction::East);
    setPosition(c, 0, 0);
    c.set(0xA);
    setPosition(c, 1, 0);
    c.set(Direction::South);
    setPosition(c, 2, 0);

    c.set(Direction::South);
    setPosition(c, 0, 1);
    c.set(0x3);
    setPosition(c, 1, 1);
    c.set(0x3);
    setPosition(c, 2, 1);

    c.set(0x5);
    setPosition(c, 0, 2);
    c.set(0xD);
    setPosition(c, 1, 2);
    c.set(0x9);
    setPosition(c, 2, 2);
}

const struct Case &Labyrinthe::getPosition(uint8_t x, uint8_t y)
{
    uint8_t offset = x + _size * y;

    return _lab.at(offset);
}

void Labyrinthe::setPosition(const Case &c, uint8_t x, uint8_t y)
{
    uint64_t offset = x + _size * y;
    _lab.at(offset) = c;
}

struct Case& Labyrinthe::updatePosition(uint8_t x, uint8_t y)
{
    uint64_t offset = x + _size * y;

    return _lab.at(offset);
}

void Labyrinthe::generate()
{
    std::random_device os_seed;
    const uint8_t seed = os_seed();

    std::mt19937 generator(seed);
    std::uniform_int_distribution<uint8_t> distribution(0, 1);

    const uint8_t end = _size - 1;

    for (unsigned int i = 0; i < _size - 1; ++i)
    {
        updatePosition(end, i).open(Direction::South);
        updatePosition(end, i + 1).open(Direction::North);

        updatePosition(i, end).open(Direction::East);
        updatePosition(i + 1, end).open(Direction::West);
    }

    for (uint8_t i = 0; i < _size - 1; ++i)
    {
        for (uint8_t j = 0; j < _size - 1; ++j)
        {
            if (distribution(generator) == 0)
            {
                updatePosition(i, j).open(Direction::South);
                updatePosition(i, j + 1).open(Direction::North);
            }
            else
            {
                updatePosition(i, j).open(Direction::East);
                updatePosition(i + 1, j).open(Direction::West);
            }
        }
    }
}
