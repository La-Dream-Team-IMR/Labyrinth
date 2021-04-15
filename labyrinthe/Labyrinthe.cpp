//
// Created by kevin on 13/04/2021.
//

#include "Labyrinthe.h"

Labyrinthe::Labyrinthe(unsigned short size) : _size(size), _lab(size)
{
}

const struct Case& Labyrinthe::getPosition(unsigned short x, unsigned short y)
{
    unsigned short offset = x + _size * y;

    return _lab.at(offset);
}
