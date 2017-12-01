#ifndef MAP_HPP
#define MAP_HPP

#include "point.hpp"
#include "constants.hpp"

// char** MAP_Get(void);
MapTiles MAP_GetData(Point* _p);
void MAP_SetData(Point* _p, MapTiles _d);
bool MAP_CheckValidLocation(Point *p);
void MAP_Gen(void);
void MAP_Print(void);

#endif /* end of include guard: MAP_HPP */