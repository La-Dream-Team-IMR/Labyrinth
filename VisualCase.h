#ifndef V_CASE_H
#define V_CASE_H

#include "labyrinthe/Case.h"
#include <gl-matrix.h>
#include <vector>

class Mur;

class VisualCase
{
private:
    std::vector<Mur> murs;
public:
    VisualCase(Case c, int i, int j);
    VisualCase();
    ~VisualCase();

    void onRender(const mat4& matP, const mat4& matVM);
};
#endif