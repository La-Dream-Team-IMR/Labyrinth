//
// Created by kevin on 13/04/2021.
//

#ifndef LABYRINTH_CASE_H
#define LABYRINTH_CASE_H

struct Case
{
    bool North, South, East, West;

    Case() : North(false), South(false), East(false), West(false) {}

    Case(unsigned short bitmap)
    {
        North = bitmap & 1;
        South = bitmap & (1 << 1);
        East = bitmap & (1 << 2);
        West = bitmap & (1 << 3);
    }

    bool isFalse()
    {
        return !North && !South && !East && !West;
    }

    bool operator !()
    {
        return isFalse();
    }
};

#endif //LABYRINTH_CASE_H
