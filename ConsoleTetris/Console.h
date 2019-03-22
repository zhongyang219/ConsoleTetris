//************************************************************
//				本文件为控制台相关全局函数的定义
//************************************************************
#pragma once
#include<Windows.h>
#include"Define.h"

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

//计算一个宽字节字符串的长度占用的半角字符数
size_t WcharStrHalfWidthLen(const wchar_t* str)
{
	size_t size{ 0 };
	const size_t length{ wcslen(str) };
	for (int i{ 0 }; i < length; i++)
	{
		if (str[i] >= 0 && str[i] < 128)
			size++;		//如果一个Unicode字符编码在0~127范围内，它占一个半角字符宽度
		else
			size += 2;		//否则它占两个半角字符宽度
	}
	return size;
}

//在x,y坐标处输出界面的格子
void PrintCell(Cell cell, short x, short y)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	if (cell.type == FULL_CELL)
	{
		WriteConsoleOutputCharacterW(handle, L"■", 1, pos, &unuse);
		FillConsoleOutputAttribute(handle, cell.color, 2, pos, &unuse);
	}
	else
	{
		WriteConsoleOutputCharacterW(handle, L"□", 1, pos, &unuse);
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
		WriteConsoleOutputCharacterW(handle, L"■", 1, pos, &unuse);
	else
		WriteConsoleOutputCharacterW(handle, L"□", 1, pos, &unuse);
	FillConsoleOutputAttribute(handle, color, 2, pos, &unuse);
}


//在界面的x,y坐标处输出一个数字
void PrintInt(int num, short x, short y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	wchar_t str[20];
	_itow_s(num, str, 10);
	size_t len{ wcslen(str) };
	WriteConsoleOutputCharacterW(handle, str, len, pos, &unuse);		//输出字符
	FillConsoleOutputAttribute(handle, color, len, pos, &unuse);		//设置颜色
}

//在界面的x,y坐标处输出一个字符串
void PrintString(const wchar_t *str, short x, short y, Color color)
{
	COORD pos;
	ULONG unuse;
	pos.X = x * 2;
	pos.Y = y;
	size_t len{ wcslen(str) };
	size_t len_halfwidth{ WcharStrHalfWidthLen(str) };	//字符串占用半角字符数
	WriteConsoleOutputCharacterW(handle, str, len, pos, &unuse);
	FillConsoleOutputAttribute(handle, color, len_halfwidth, pos, &unuse);		//设置颜色
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
	switch (color)
	{
		case WHITE: PrintString(L"白色  ", x, y, color); break;
		case RED: PrintString(L"红色  ", x, y, color); break;
		case GREEN: PrintString(L"绿色  ", x, y, color); break;
		case BLUE: PrintString(L"蓝色  ", x, y, color); break;
		case YELLOW: PrintString(L"黄色  ", x, y, color); break;
		case CYAN: PrintString(L"蓝绿色", x, y, color); break;
		case PURPLE: PrintString(L"紫色  ", x, y, color); break;
		case GRAY:PrintString(L"灰色  ", x, y, color); break;
		default: break;
	}
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
	wchar_t buf[2];
	/*if (IllegalControlKey(key))
	{
		WriteConsoleOutputCharacterW(handle, "按键非法！", 10, pos, &unuse);
		FillConsoleOutputAttribute(handle, RED, 10, pos, &unuse);
	}*/
	if(key != UP_KEY && key != DOWN_KEY && key != LEFT_KEY && key != RIGHT_KEY && (key>127 || key<=0))
	{
		PrintString(L"未设定", x, y, RED);
		return;
	}
	switch (key)
	{
		case ENTER_KEY:
			PrintString(L"回车键", x, y, color);
			break;
		case ' ':
			PrintString(L"空格键", x, y, color);
			break;
		case 9:
			PrintString(L"Tab键", x, y, color);
			break;
		case UP_KEY:
			PrintString(L"↑", x, y, color);
			break;
		case DOWN_KEY:
			PrintString(L"↓", x, y, color);
			break;
		case LEFT_KEY:
			PrintString(L"←", x, y, color);
			break;
		case RIGHT_KEY:
			PrintString(L"→", x, y, color);
			break;
		default:
			buf[0] = static_cast<char>(key);
			buf[1] = '\0';
			PrintString(buf, x, y, color);
	}
}