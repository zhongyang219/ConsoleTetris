//************************************************************
//				本文件为控制台相关全局函数的定义
//************************************************************
#pragma once
#include<Windows.h>
#include"Define.h"

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

//在x,y坐标处输出界面的格子
void PrintCell(Cell cell, short x, short y)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	if (cell.type == FULL_CELL)
	{
		WriteConsoleOutputCharacterA(handle, "■", 2, pos, &unuse);
		FillConsoleOutputAttribute(handle, cell.color, 2, pos, &unuse);
	}
	else
	{
		WriteConsoleOutputCharacterA(handle, "□", 2, pos, &unuse);
		FillConsoleOutputAttribute(handle, GRAY, 2, pos, &unuse);
	}
}

//在x,y坐标处输出界面的格子（指定颜色）
void PrintCell(Cell cell, short x, short y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	if (cell.type == FULL_CELL)
		WriteConsoleOutputCharacterA(handle, "■", 2, pos, &unuse);
	else
		WriteConsoleOutputCharacterA(handle, "□", 2, pos, &unuse);
	FillConsoleOutputAttribute(handle, color, 2, pos, &unuse);
}


//在界面的x,y坐标处输出一个数字
void PrintInt(int num, short x, short y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	char str[20];
	_itoa_s(num, str, 10);
	size_t len{ strlen(str) };
	WriteConsoleOutputCharacterA(handle, str, len, pos, &unuse);		//输出字符
	FillConsoleOutputAttribute(handle, color, len, pos, &unuse);		//设置颜色
}

//在界面的x,y坐标处输出一个字符串
void PrintString(char *str, short x, short y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	size_t len{ strlen(str) };
	WriteConsoleOutputCharacterA(handle, str, len, pos, &unuse);
	FillConsoleOutputAttribute(handle, color, len, pos, &unuse);		//设置颜色
}

//光标移动到x,y坐标
void GotoXY(short x, short y)
{
	COORD pos;
	pos.X = x * 2;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

//在界面的x,y处输出不同颜色的汉字
void PrintColor(short x, short y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	switch (color)
	{
		case WHITE: WriteConsoleOutputCharacterA(handle, "白色  ", 6, pos, &unuse); break;
		case RED: WriteConsoleOutputCharacterA(handle, "红色  ", 6, pos, &unuse); break;
		case GREEN: WriteConsoleOutputCharacterA(handle, "绿色  ", 6, pos, &unuse); break;
		case BLUE: WriteConsoleOutputCharacterA(handle, "蓝色  ", 6, pos, &unuse); break;
		case YELLOW: WriteConsoleOutputCharacterA(handle, "黄色  ", 6, pos, &unuse); break;
		case CYAN: WriteConsoleOutputCharacterA(handle, "蓝绿色", 6, pos, &unuse); break;
		case PURPLE: WriteConsoleOutputCharacterA(handle, "紫色  ", 6, pos, &unuse); break;
		case GRAY:WriteConsoleOutputCharacterA(handle, "灰色  ", 6, pos, &unuse); break;
		default: break;
	}
	FillConsoleOutputAttribute(handle, color, 6, pos, &unuse);
}

//显示或隐藏光标
void CursorVisible(bool visible)
{
	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = visible;
	cci.dwSize = 25;		//光标厚度，取值为1~100
	SetConsoleCursorInfo(handle, &cci);
}

//绘制炸弹
void PrintBomb(int x, int y)
{
	PrintCell(Cell{ FULL_CELL, RED }, x, y);
	PrintCell(Cell{ FULL_CELL, GREEN }, x + 3, y);
	PrintCell(Cell{ FULL_CELL, BLUE }, x , y+ 3);
	PrintCell(Cell{ FULL_CELL, YELLOW }, x + 3, y + 3);
	PrintCell(Cell{ FULL_CELL, GRAY }, x + 1, y + 3);
	PrintCell(Cell{ FULL_CELL, GRAY }, x + 2, y + 3);

	PrintCell(Cell{ FULL_CELL, GREEN }, x + 1, y + 1);
	PrintCell(Cell{ FULL_CELL, RED }, x + 2, y + 1);
	PrintCell(Cell{ FULL_CELL, YELLOW }, x + 1, y + 2);
	PrintCell(Cell{ FULL_CELL, BLUE }, x + 2, y + 2);
}

//擦除炸弹
void ClearBomb(int x, int y)
{
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x, y);
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x + 3, y);
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x, y + 3);
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x + 3, y + 3);
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x + 1, y + 3);
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x + 2, y + 3);

	PrintCell(Cell{ EMPTY_CELL, GRAY }, x + 1, y + 1);
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x + 2, y + 1);
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x + 1, y + 2);
	PrintCell(Cell{ EMPTY_CELL, GRAY }, x + 2, y + 2);
}

//用于在键位设定中输出一个按键的信息
void PrintKey(int key, short x, short y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	char buf[2];
	/*if (IllegalControlKey(key))
	{
		WriteConsoleOutputCharacterA(handle, "按键非法！", 10, pos, &unuse);
		FillConsoleOutputAttribute(handle, RED, 10, pos, &unuse);
	}*/
	if(key != UP_KEY && key != DOWN_KEY && key != LEFT_KEY && key != RIGHT_KEY && (key>127 || key<=0))
	{
		WriteConsoleOutputCharacterA(handle, "未设定", 6, pos, &unuse);
		FillConsoleOutputAttribute(handle, RED, 6, pos, &unuse);
		return;
	}
	switch (key)
	{
		case ENTER_KEY:
			WriteConsoleOutputCharacterA(handle, "回车键", 6, pos, &unuse);
			FillConsoleOutputAttribute(handle, color, 6, pos, &unuse);
			break;
		case ' ':
			WriteConsoleOutputCharacterA(handle, "空格键", 6, pos, &unuse);
			FillConsoleOutputAttribute(handle, color, 6, pos, &unuse);
			break;
		case 9:
			WriteConsoleOutputCharacterA(handle, "Tab键", 5, pos, &unuse);
			FillConsoleOutputAttribute(handle, color, 5, pos, &unuse);
			break;
		case UP_KEY:
			WriteConsoleOutputCharacterA(handle, "↑", 2, pos, &unuse);
			FillConsoleOutputAttribute(handle, color, 2, pos, &unuse);
			break;
		case DOWN_KEY:
			WriteConsoleOutputCharacterA(handle, "↓", 2, pos, &unuse);
			FillConsoleOutputAttribute(handle, color, 2, pos, &unuse);
			break;
		case LEFT_KEY:
			WriteConsoleOutputCharacterA(handle, "←", 2, pos, &unuse);
			FillConsoleOutputAttribute(handle, color, 2, pos, &unuse);
			break;
		case RIGHT_KEY:
			WriteConsoleOutputCharacterA(handle, "→", 2, pos, &unuse);
			FillConsoleOutputAttribute(handle, color, 2, pos, &unuse);
			break;
		default:
			buf[0] = static_cast<char>(key);
			buf[1] = '\0';
			WriteConsoleOutputCharacterA(handle, buf, 1, pos, &unuse);
			FillConsoleOutputAttribute(handle, color, 1, pos, &unuse);
	}
}