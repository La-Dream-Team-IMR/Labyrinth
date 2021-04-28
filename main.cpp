#include <iostream>

#include "labyrinthe/Labyrinthe.h"


int main(){
    std::cout << "Hello World !\n";

    Labyrinthe lab(10);

    const auto size = lab.getSize();

    std::cout << "__________" << std::endl;

    for(uint8_t i = 0; i < size; ++i)
    {
        for(uint8_t j = 0; j < size; ++j)
        {
            auto c = lab.getPosition(i, j);

            if(!c.South)
                std::cout << "_";
            else
                std::cout << " ";

            if(!c.East)
                std::cout << "|";
            else
                std::cout << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}