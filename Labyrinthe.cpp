//
// Created by kevin on 13/04/2021.
//
#include <iostream>
#include "Labyrinthe.h"
using namespace std;

Labyrinthe::Labyrinthe(unsigned short size) : _size(size), _lab(size*size)
{
    _lab[0] = Case(6);
    _lab[1] = Case(10);
    _lab[2] = Case(5);
    _lab[3] = Case(9);
}

const struct Case& Labyrinthe::getPosition(unsigned short x, unsigned short y)
{
    unsigned short offset = x + _size * y;
    cout << offset << endl;
    return _lab.at(offset);
}
