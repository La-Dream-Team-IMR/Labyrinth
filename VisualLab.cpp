#include "VisualLab.h"
#include "VisualCase.h"

void VisualLab::onRender(const mat4 &matP, const mat4 &matVM){
    int8_t size = lab->getSize();
    for (int8_t i = 0; i < size*size; i++)
    {
        cases[i].onRender(matP, matVM);
    }
    
}