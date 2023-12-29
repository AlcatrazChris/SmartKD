#ifndef __ASTAR_H
#define __ASTAR_H

#include "main.h"

#define MAP_LENGTH 8
#define MAP_WIDTH  5


void Astar_Init();
void update(int x, int y);
void getNext(int x, int y, int* nx, int* ny, int dirc, int distance);
void findWay(int x, int y, int dx, int dy, int* dirc, int* distance);
	
#define TURN_COST 1

#endif
