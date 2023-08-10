#ifndef RESOURCE_H
#define RESOURCE_H

#define PROGRAM_NAME "순식간에Fight"
#define PROGRAM_VERSION "vD.1.8"
#define PROGRAM_AUTHOR "배근준(짱구슬동자)"

#define MAX_X 20
#define MAX_Y 35
#define MAX_PLAYER 8
#define MAX_ENEMY 100
#define MAX_CHARACTER 1000
#define MAX_FOCUS 10
#define MAX_STARTLOCATION 20
#define MAX_ITEM 10
#define MAX_TIMER 1000
#define MAX_MOVE 1000
#define MAX_QUEUE 20*35
#define MAX_REPLAY 1000000
#define MAX_CODE 100
#define MAX_ITEM 10

#include "Class.h"

int queue[MAX_QUEUE],rear,front;

#define put(queue_data) queue[rear++]=queue_data
#define get() queue[front++]

#define BOX 9
#define INF -1

const CPoint MaxMapSize={MAX_X,MAX_Y};
const CKeyboard EXITGAME={27,1},SAVEREPLAY={19,1},MapRandom={18,1};
const CKeyboard KeyUpArrow={72,0},KeyDownArrow={80,0},KeyCommand={13,1};

//Map Character
#define CHARACTER_MAP_BACKGROUND "■"
#define CHARACTER_MAP0 "  "
#define CHARACTER_MAP1 "■"
#define CHARACTER_MOVE "※"
#define CHARACTER_BOX "□"

#endif
