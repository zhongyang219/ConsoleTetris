//************************************************************
//本文件包含宏定义、全局常量、枚举类型和全局函数的定义
//************************************************************
#pragma once
#include<Windows.h>

//定义功能键
#define UP_KEY (-1)			//定义上方向键
#define DOWN_KEY (-2)		//定义下方向键
#define LEFT_KEY (-3)		//定义左方向键
#define RIGHT_KEY (-4)		//定义右方向键
//定义字符键
#define PAUSE_KEY 13			//定义暂停键
#define QUIT_KEY 27		//定义退出键（按esc键退出）
#define SPACE_KEY ' '		//定义空格键
#define CHEAT_KEY '4'		//定义作弊键
#define LEVELUP_KEY '5'		//定义“一键升级”键
#define ENTER_KEY 13		//定义回车键
#define PLAYER1_STRIP1_KEY '1'	//玩家1使用道具1键
#define PLAYER1_STRIP2_KEY '2'	//玩家1使用道具2键
#define PLAYER1_STRIP3_KEY '3'	//玩家1使用道具3键
#define PLAYER2_STRIP1_KEY '8'	//玩家2使用道具1键
#define PLAYER2_STRIP2_KEY '9'	//玩家2使用道具2键
#define PLAYER2_STRIP3_KEY '0'	//玩家2使用道具3键

#define FULL_CELL true		//定义界面上的格子满的状态
#define EMPTY_CELL false		//定义界面上的格子空的状态

#define PLAYER1 true		//玩家1
#define PLAYER2 false		//玩家2

using CellType = bool;		//界面上的格子的状态
using Player = bool;		//玩家选择

const int WIDTH{ 20 };		//界面的宽度（最大宽度）
const int HEIGHT{ 23 };		//界面的高度
const int BOARD_X{ 8 };	//绘制界面的起始x坐标（界面宽度为最大20时的绘制界面起始x坐标）
const int BOARD_Y{ 1 };	//绘制界面的起始y坐标

enum BlockType		//方块的类型
{
	B_I,
	B_L,
	B_J,
	B_Z,
	B_S,
	B_T,
	B_O,
	B_BOMB,		//炸弹
	B_PIERCE		//穿甲弹
};

enum Direction
{
	UP,
	RIGHT,
	DOWN,
	LEFT
};

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

//enum Method
//{
//	MOVE_LEFT,
//	MOVE_RIGHT,
//	ROTATE
//};

struct Cell
{
	CellType type;
	Color color;
};

//struct BlockState
//{
//	int x_position;
//	Direction dirction;
//};

//储存7+2种方块4个朝向的每个像素点距离方块坐标的位置偏移
const int BlockPointOffset[9][4][4][2]
{
	//I形方块
	{
		//UP方向
		{ { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
		//RIGHT方向
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
		//DOWN方向
		{ { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
		//LEFT方向
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } }
	},

	//L形方块
	{
		//UP方向
		{	{ 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 2 }	},
		//RIGHT方向
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 } },
		//DOWN方向
		{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
		//LEFT方向
		{ { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } }
	},

	//J形方块
	{
		//UP方向
		{ { 1, 0 }, { 1, 1 }, { 0, 2 }, { 1, 2 } },
		//RIGHT方向
		{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
		//DOWN方向
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 0, 2 } },
		//LEFT方向
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 } }
	},

	//Z形方块
	{
		//UP方向
		{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
			//RIGHT方向
		{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } },
		//DOWN方向
		{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
		//LEFT方向
		{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } }
	},

	//S形方块
	{
		//UP方向
		{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
		//RIGHT方向
		{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
		//DOWN方向
		{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
		//LEFT方向
		{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } }
	},

	//T形方块
	{
		//UP方向
		{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 } },
		//RIGHT方向
		{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 0, 2 } },
		//DOWN方向
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 1 } },
		//LEFT方向
		{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } }
	},

	//O形方块
	{
		//UP方向
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
		//RIGHT方向
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
		//DOWN方向
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
		//LEFT方向
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } }
	},
	
	//炸弹
	{
		//UP方向
		{ { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 } },
		//RIGHT方向
		{ { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 } },
		//DOWN方向
		{ { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 } },
		//LEFT方向
		{ { 0, 3 }, { 1, 3 }, { 2, 3 }, { 3, 3 } }
	},
	
	//穿甲弹
	{ { { 0 } } }
};

//为SYSTEMTIME结构重载减号运算符，在确保a>b的情况下返回两个时间差的毫秒数，但是返回的值不会超过3000毫秒
int operator-(SYSTEMTIME a, SYSTEMTIME b)
{
	if (a.wSecond == b.wSecond)
		return a.wMilliseconds - b.wMilliseconds;
	else if (a.wSecond - b.wSecond == 1 || a.wSecond - b.wSecond == -59)
		return a.wMilliseconds - b.wMilliseconds + 1000;
	else
		return a.wMilliseconds - b.wMilliseconds + 2000;
}

//获取一个键盘输入按键并将其返回
int GetKey()
{
	int key{ _getch() };
	if (key == 0xE0 || key == 0)		//如果获得的按键值为0x0E或0则表示按下了功能键
	{
		switch (_getch())		//按下了功能键需要再次调用_getch函数
		{
			case 72: key = UP_KEY; break;
			case 80: key = DOWN_KEY; break;
			case 75: key = LEFT_KEY; break;
			case 77: key = RIGHT_KEY; break;
			default: break;
		}
	}
	if (key>='a' && key<='z')		//如果按的是小写字母，则自动转换成大写字母
		key -= 32;
	return key;
}

//用于在键位设定时判断用户按下的键是否为非法
inline bool IllegalControlKey(int key)
{
	//当用户按下的键为现有的控制键或非ASCII码字符键时，判定为按键非法
	return (key == PAUSE_KEY || key == PLAYER1_STRIP1_KEY || key == PLAYER1_STRIP2_KEY || key == PLAYER1_STRIP3_KEY ||
		key == PLAYER2_STRIP1_KEY || key == PLAYER2_STRIP2_KEY || key == PLAYER2_STRIP3_KEY ||
		key == QUIT_KEY || key =='O' ||key > 127);
}