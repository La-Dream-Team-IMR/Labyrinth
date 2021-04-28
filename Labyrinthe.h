//
// Created by kevin on 13/04/2021.
//

#ifndef LABYRINTH_LABYRINTHE_H
#define LABYRINTH_LABYRINTHE_H

#include "Case.h"
#include <vector>
#include <cstdint>

class Labyrinthe
{
public:
    Labyrinthe() : Labyrinthe(5) {}
    explicit Labyrinthe(uint8_t size);
    ~Labyrinthe() = default;

    const struct Case& getPosition(uint8_t x, uint8_t y);

    const uint8_t& getSize() const { return _size; }

private:
    uint8_t _size;
    std::vector<struct Case> _lab;

    void generate();

    void setPosition(const struct Case &c, uint8_t x, uint8_t y);
};


#endif //LABYRINTH_LABYRINTHE_H
