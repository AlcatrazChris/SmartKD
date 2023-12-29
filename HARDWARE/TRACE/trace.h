#ifndef __TRACE_H
#define __TRACE_H

#include "main.h"

#define DIRC_FORWARD 0
#define DIRC_BACK    1
#define DIRC_LEFT    2
#define DIRC_RIGHT   3

void Move(int speed, int dirc, int distance, int dx, int dy ,int antidx ,int antidy);



#define TRACE_BACK_FORCE 1500

#endif
