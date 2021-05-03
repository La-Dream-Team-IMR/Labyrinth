#ifndef V_LAB_H
#define V_LAB_H

#include "labyrinthe/Case.h"
#include "labyrinthe/Labyrinthe.h"
#include "VisualCase.h"
#include <gl-matrix.h>
#include <iostream>

class Mur;

class VisualLab
{
private:
    Labyrinthe *lab;
    VisualCase *cases;

public:
    VisualLab(Labyrinthe lab) : lab(&lab)
    {
        u_int8_t size = lab.getSize();
        VisualCase *cases[size * size];

        for (uint8_t i = 0; i < size; ++i)
        {
            for (uint8_t j = 0; j < size; ++j)
            {
                auto c = lab.getPosition(i, j);
                int index = i + j * size;
                std::cout << "nouvelle case : " << (int) i << "," << (int) j << std::endl;
                cases[index] = new VisualCase(c, i, j);
            }
        }
    }
    ~VisualLab()
    {
        delete cases;
    }

    void onRender(const mat4 &matP, const mat4 &matVM);
};
#endif