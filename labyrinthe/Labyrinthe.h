//
// Created by kevin on 13/04/2021.
//

#ifndef LABYRINTH_LABYRINTHE_H
#define LABYRINTH_LABYRINTHE_H

#include "Case.h"
#include <vector>

class Labyrinthe
{
public:
    Labyrinthe() : Labyrinthe(5) {}
    explicit Labyrinthe(unsigned short size);
    ~Labyrinthe() = default;

    const struct Case& getPosition(unsigned short x, unsigned short y);

private:
    unsigned short _size;
    std::vector<struct Case> _lab;
};


#endif //LABYRINTH_LABYRINTHE_H
