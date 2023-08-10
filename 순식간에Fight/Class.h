#ifndef CLASS_H
#define CLASS_H

#include<time.h>
#include "Resource.h"

typedef enum{Up,Down,Left,Right,Stay}Direction;

class CPoint
{
public:
	int x,y;
};

class CRange
{
public:
	int Max,Min,Pos;
};

class CKeyboard
{
public:
	int Key;
	bool Type;
};

class CStep
{
public:
	int Old,Bonus,Level;
	int GetStep()
	{
		return Old+Bonus*Level;
	}
};

class CTimer
{
public:
	long Time,Elapse;
	void SetTimer(long nElapse)
	{
		Elapse=nElapse;
		SetTime();
	}
	void SetTime(long nTime=clock())
	{
		Time=nTime;
	}
	bool IsElapse(void)
	{
		return clock()-Time>=Elapse;
	}
};

class CItem
{
public:
	int Attack,Range,Shot;
	char ItemCharacter[11];
	long Elapse;
	CTimer AttackSpeed;
};

class CPlayer
{
public:
	CRange Hp;
	CTimer HpUp;
	CPoint Point;
	char Character[3],CharacterColor[3];
	Direction Vision;
	CTimer MoveSpeed;
	int Focus;
	bool Die,MoveCheck;
	//æ∆¿Ã≈€
	CItem Item[MAX_ITEM];
};

class CUserKey
{
public:
	CKeyboard Up,Down,Left,Right,Command,Prev,Next;
};

class CReplay
{
public:
	int UserIndex;
	long Time;
	unsigned char UserDirection;
};

#endif
