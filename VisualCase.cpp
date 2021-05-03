#include "VisualCase.h"

#include "Mur.h"
#include <vector>

VisualCase::VisualCase() : VisualCase(Case(0), 0, 0)
{
}

VisualCase::VisualCase(Case c, int i, int j) : murs(4)
{
    murs.at((u_int8_t)0) = Mur(1, 8); //North
    murs.at((u_int8_t)0).setDoor(c.North);
    murs.at((u_int8_t)0).setPosition(vec2::fromValues(0 + i * 8, 4 - j * 8));
    murs.at((u_int8_t)1) = Mur(8, 1); //East
    murs.at((u_int8_t)1).setDoor(c.East);
    murs.at((u_int8_t)1).setPosition(vec2::fromValues(4 + i * 8, 0 - j * 8));
    murs.at((u_int8_t)2) = Mur(1, 8); //South
    murs.at((u_int8_t)2).setDoor(c.South);
    murs.at((u_int8_t)2).setPosition(vec2::fromValues(0 + i * 8, -4 - j * 8));
    murs.at((u_int8_t)3) = Mur(8, 1); //West
    murs.at((u_int8_t)3).setDoor(c.West);
    murs.at((u_int8_t)3).setPosition(vec2::fromValues(-4 + i * 8, 0 - j * 8));
}

VisualCase::~VisualCase()
{
}

void VisualCase::onRender(const mat4 &matP, const mat4 &matVM)
{
    murs.at((u_int8_t)0).onRender(matP, matVM);
    murs.at((u_int8_t)1).onRender(matP, matVM);
    murs.at((u_int8_t)2).onRender(matP, matVM);
    murs.at((u_int8_t)3).onRender(matP, matVM);
}