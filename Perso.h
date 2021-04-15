#ifndef PERSO_H
#define PERSO_H

struct Perso
{
    unsigned int pos_x;
    unsigned int pos_y;

    Perso() : pos_x(0), pos_y(0){}
    Perso(unsigned int x, unsigned int y) : pos_x(x), pos_y(y){}

};


#endif