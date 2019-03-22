#pragma once
#include<Windows.h>
#define UP_KEY (-1)			//定义上方向键
#define DOWN_KEY (-2)		//定义下方向键
#define LEFT_KEY (-3)		//定义左方向键
#define RIGHT_KEY (-4)	//定义右方向键
#define QUIT_KEY 27		//定义退出键（按esc键退出）
#define SPACE_KEY ' '	//定义空格键
#define TAB_KEY 9		//定义tab键
#define ENTER_KEY 13		//定义回车键

#define FULL_CELL true		//定义界面上的格子满的状态
#define EMPTY_CELL false		//定义界面上的格子空的状态
const int WIDTH{ 20 };
const int HEIGHT{ 23 };
const int MAP_X{ 2 };		//地图编辑打印的起始x坐标
const int MAP_Y{ 1 };		//地图编辑打印的起始y坐标
const int SETTINGS_X{ 24 };		//设置编辑打印的起始x坐标
const int SETTINGS_Y{ 1 };		//设置编辑打印的起始y坐标

using CellType = bool;		//界面上的格子的状态

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

enum Color
{
	GRAY = 8,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PURPLE,
	YELLOW,
	WHITE
};

struct Cell
{
	CellType type;
	Color color;
};

//在x,y坐标处输出界面的格子
void PrintCell(Cell cell, int x, int y)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	if (cell.type == FULL_CELL)
		WriteConsoleOutputCharacterA(h, "■", 2, pos, &unuse);
	else
		WriteConsoleOutputCharacterA(h, "□", 2, pos, &unuse);
	FillConsoleOutputAttribute(h, cell.color, 2, pos, &unuse);
}
/*
//在x,y坐标处输出界面的格子（指定颜色）
void PrintCell(Cell cell, int x, int y, Color color)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	if (cell.type == FULL_CELL)
		WriteConsoleOutputCharacterA(h, "■", 2, pos, &unuse);
	else
		WriteConsoleOutputCharacterA(h, "□", 2, pos, &unuse);
	FillConsoleOutputAttribute(h, color, 2, pos, &unuse);
}*/


//在界面的x,y坐标处输出一个数字
void PrintInt(int num, int x, int y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	char str[20];
	_itoa_s(num, str, 10);
	size_t len{ strlen(str) };
	WriteConsoleOutputCharacterA(h, str, len, pos, &unuse);		//输出字符
	FillConsoleOutputAttribute(h, color, len, pos, &unuse);		//设置颜色
}

//在界面的x,y坐标处输出一个字符串
void PrintString(const char *str, int x, int y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	size_t len{ strlen(str) };
	WriteConsoleOutputCharacterA(h, str, len, pos, &unuse);
	FillConsoleOutputAttribute(h, color, len, pos, &unuse);		//设置颜色
}

//光标移动到x,y坐标
void GotoXY(int x, int y)
{
	COORD pos;
	pos.X = x * 2;
	pos.Y = y;
	SetConsoleCursorPosition(h, pos);
}

//显示或隐藏光标
void CursorVisible(bool visible)
{
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = visible;
	cci.dwSize = 25;		//光标厚度，取值为1~100
	SetConsoleCursorInfo(h, &cci);
}