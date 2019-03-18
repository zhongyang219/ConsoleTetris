//************************************************************
//				本文件为方块类的定义
//************************************************************
#pragma once
#include"Define.h"
#include"Console.h"

class CBlock
{
private:
	BlockType m_type{ B_L };		//方块的类型
	Direction m_direction{ UP };		//方块的朝向
	Color m_color{ WHITE };		//方块的颜色

	int m_BlockPoint[4][2];		//储存当前方块每个像素点所在的坐标（每个方块4个像素点，每个点有x和y坐标）

	int m_x { WIDTH / 2 - 1 };		//方块的位置坐标
	int m_y { 0 };

	void SetBlockPoint();			//设置当前方块每个像素点的坐标
	//void SetColor();		//设置不同类型方块的颜色
	
public:
	CBlock();
	//CBlock(BlockType type, Direction dir);
	CBlock(BlockType type, Direction dir, Color color, int x_position);

	void DrawBlock(Cell board[WIDTH][HEIGHT], int width, int board_x) const;		//绘制方块
	void ClearBlock(Cell board[WIDTH][HEIGHT], int width, int board_x) const;		//擦除方块
	void ClearBlockNotPrint(Cell board[WIDTH][HEIGHT], int width) const;		//擦除方块但是不绘制到界面上
	void MoveBlock(Direction deriction, const Cell board[WIDTH][HEIGHT], int width);		//移动方块
	void RotateBlock(const Cell board[WIDTH][HEIGHT], int width);		//旋转方块

	bool LandTest(const Cell board[WIDTH][HEIGHT], int width);		//方块落下检测
	bool BlockCollisionTest(const Cell board[WIDTH][HEIGHT], int width) const;		//方块碰撞检测

	BlockType GetBlockType() const;
	Direction GetBlockDirection() const;
	Color GetBlockColor() const;
	
	int GetXPosition() const;
	int GetYPosition() const;

	void SetXPosition(int x_pos);		//设置方块的x坐标
	
	void PrintBlock(int x, int y) const;		//在指定的位置(x, y)处绘制方块（用于绘制界面中的下一个方块显示）
};


CBlock::CBlock()
{
	SetBlockPoint();
	//SetColor();
}
/*
CBlock::CBlock(BlockType type, Direction dir) : m_type{ type }, m_direction{ dir }
{
	SetBlockPoint();
	SetColor();
}*/

CBlock::CBlock(BlockType type, Direction dir, Color color, int x_position) : m_type{ type }, m_direction{ dir }, m_color{ color }, m_x{ x_position }
{
	SetBlockPoint();
}



void CBlock::SetBlockPoint()
{
	for (int i{ 0 }; i < 4; i++)		//设置当前方块的每个像素点的坐标，将方块坐标加上每个像素点的偏移
	{
		m_BlockPoint[i][0] = m_x + BlockPointOffset[m_type][m_direction][i][0];
		m_BlockPoint[i][1] = m_y + BlockPointOffset[m_type][m_direction][i][1];
	}
}
/*
void CBlock::SetColor()
{
	switch (m_type)
	{
		case B_I: m_color = WHITE; break;
		case B_L: m_color = RED; break;
		case B_J: m_color = GREEN; break;
		case B_Z: m_color = BLUE; break;
		case B_S: m_color = YELLOW; break;
		case B_T: m_color = CYAN; break;
		case B_O: m_color = PURPLE; break;
		default:  break;
	}
}*/

void CBlock::DrawBlock(Cell board[WIDTH][HEIGHT], int width, int board_x) const
{
	for (int i{ 0 }; i < 4; i++)
	{
		if (m_BlockPoint[i][0] >= 0 && m_BlockPoint[i][0] < width && m_BlockPoint[i][1] >= 0 && m_BlockPoint[i][1] < HEIGHT)		//确保制绘或擦除方块时不会使得数组越界
		{
			board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].type = FULL_CELL;
			board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].color = m_color;
			PrintCell(board[m_BlockPoint[i][0]][m_BlockPoint[i][1]], m_BlockPoint[i][0] + board_x, m_BlockPoint[i][1] + BOARD_Y);	//在将当前方块写到参数里的同时在界面上绘制出来
		}
	}
	if(m_type == B_BOMB)
	{
		PrintBomb(m_x + board_x, m_y + BOARD_Y);
	}
}

void CBlock::ClearBlock(Cell board[WIDTH][HEIGHT], int width, int board_x) const
{
	for (int i{ 0 }; i < 4; i++)
	{
		if (m_BlockPoint[i][0] >= 0 && m_BlockPoint[i][0] < width && m_BlockPoint[i][1] >= 0 && m_BlockPoint[i][1] < HEIGHT)		//确保绘制或擦除方块时不会使得数组越界
		{
			board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].type = EMPTY_CELL;
			board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].color = GRAY;
			PrintCell(board[m_BlockPoint[i][0]][m_BlockPoint[i][1]], m_BlockPoint[i][0] + board_x, m_BlockPoint[i][1] + BOARD_Y);	//在将当前方块写到参数里的同时在界面上绘制出来
		}
	}
	if(m_type == B_BOMB)
	{
		ClearBomb(m_x + board_x, m_y + BOARD_Y);
	}
}

void CBlock::ClearBlockNotPrint(Cell board[WIDTH][HEIGHT], int width) const
{
	for (int i{ 0 }; i < 4; i++)
	{
		if (m_BlockPoint[i][0] >= 0 && m_BlockPoint[i][0] < width && m_BlockPoint[i][1] >= 0 && m_BlockPoint[i][1] < HEIGHT)		//确保绘制或擦除方块时不会使得数组越界
		{
			board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].type = EMPTY_CELL;
			board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].color = GRAY;
		}
	}
}

void CBlock::MoveBlock(Direction deriction, const Cell board[WIDTH][HEIGHT], int width)
{
	switch (deriction)
	{
		case LEFT: 
			m_x--;
			SetBlockPoint();
			for (int i{ 0 }; i < 4; i++)
			{
				if (board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].type == FULL_CELL || m_BlockPoint[i][0] <0)		//如果在左移过程中碰到了其他方块或超出左边界，就右移一格
				{
					m_x++;
					SetBlockPoint();
				}
			}
			break;

		case RIGHT: 
			m_x++; 
			SetBlockPoint();
			for (int i{ 0 }; i < 4; i++)
			{
				if (board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].type == FULL_CELL || m_BlockPoint[i][0] >= width)		//如果在右移过程中碰到了其他方块或超出右边界，就左移一格
				{
					m_x--;
					SetBlockPoint();
				}
			}
			break;

		case DOWN: 
			m_y++;
			SetBlockPoint();
			break;

		default: break;
	}
}

void CBlock::RotateBlock(const Cell board[WIDTH][HEIGHT], int width)
{
	bool AbleToRotate{ true };		//可以旋转flag
	switch (m_direction)
	{
		case UP: m_direction = RIGHT; break;
		case RIGHT: m_direction = DOWN; break;
		case DOWN: m_direction = LEFT; break;
		case LEFT: m_direction = UP; break;
		default: m_direction = UP; break;
	}
	SetBlockPoint();		//对当前方块有修改则需要重新设置每个像素的坐标

	if (BlockCollisionTest(board, width))		//如果在旋转过后检测到碰撞
		AbleToRotate = false;		//可以旋转flag置为false

	if(AbleToRotate == false)
	{
		for (int i{ 1 }; i <= 3; i++)
		{
			m_x -= i;		//尝试左移i格
			SetBlockPoint();
			if (BlockCollisionTest(board, width))		//如果检测到碰撞
			{
				m_x += i;		//移回原来的位置
				SetBlockPoint();
			}
			else 		//如果没有检测到碰撞
			{
				return;		//可以旋转，退出函数
			}
			/*
			这里只需要尝试左移而不用尝试右移，因为任何一种方块都不会出现旋转之后在左侧碰到其他方块或超出左边界的情况，
			而如果尝试右移的话在某种特殊情况下会出现方块在旋转后出现在障碍物另一边的BUG。*/
			/*
			m_x += i;		//尝试右移i格
			SetBlockPoint();
			if (BlockCollisionTest(arry))		//如果检测到碰撞
			{
				m_x -= i;		//移回原来的位置
				SetBlockPoint();
			}
			else 		//如果没有检测到碰撞
			{
				return;
			}*/
		}

		for (int i{ 1 }; i <= 3; i++)
		{
			m_y += i;		//尝试下移i格
			SetBlockPoint();
			if (BlockCollisionTest(board, width))		//如果检测到碰撞
			{
				for (int j{ 1 }; j <= 3; j++)
				{
					m_x -= j;		//每下移i格时依次尝试左移j格
					SetBlockPoint();
					if (BlockCollisionTest(board, width))
					{
						m_x += j;		//检测到碰撞则右移回原来的位置
						SetBlockPoint();
					}
					else			//没有检测到碰撞
					{
						return;
					}
				}
				m_y -= i;		//内层循环结束，尝试3次左移还检测到碰撞，则移回原来的位置
				SetBlockPoint();
			}
			else 		//如果没有检测到碰撞
			{
				return;
			}
		}
	}

	if (AbleToRotate == false)		//如果不可以旋转
	{
		switch (m_direction)		//旋转回原来的位置
		{
			case UP: m_direction = LEFT; break;
			case RIGHT: m_direction = UP; break;
			case DOWN: m_direction = RIGHT; break;
			case LEFT: m_direction = DOWN; break;
			default: break;
		}
		SetBlockPoint();		//对当前方块有修改则需要重新设置每个像素的坐标
	}
}

bool CBlock::LandTest(const Cell board[WIDTH][HEIGHT], int width)		//必须在DrawBlock和ClearBlock函数之前调用
{
	bool LandFlag{ false };
	//检测下落时是否触碰到了其他方块或者已经到底了
	if(m_type == B_PIERCE)
	{
		if (board[m_x][m_y].type == FULL_CELL || m_y>=HEIGHT)
		{
			LandFlag = true;
			m_y = HEIGHT - 1;
			SetBlockPoint();
			while(board[m_x][m_y].type == FULL_CELL)
			{
				m_y--;
				SetBlockPoint();
			}
		}
	}
	else
	{
		for (int i{ 0 }; i < 4; i++)
		{
			if (board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].type == FULL_CELL || m_BlockPoint[i][1]>=HEIGHT)			//检测当前方块的每个像素点是否触碰到了已有方块或已经到底
			{
				LandFlag = true;
				m_y--;		//已经碰到了要上移一格
				SetBlockPoint();		//对当前方块有修改则需要重新设置每个像素的坐标
				i = -1;
			}
		}
	}
	return LandFlag;
}

bool CBlock::BlockCollisionTest(const Cell board[WIDTH][HEIGHT], int width) const
{
	for (int i{ 0 }; i < 4; i++)
	{
		if (board[m_BlockPoint[i][0]][m_BlockPoint[i][1]].type == FULL_CELL || m_BlockPoint[i][0] >= width || m_BlockPoint[i][0] < 0 || m_BlockPoint[i][1] >= HEIGHT || m_BlockPoint[i][1] < 0)		//如果当前方块有任何一个像素点所在位置已经不是空的或超出上下左右边界，则检测到碰撞
			return true;
	}
	return false;
}

inline BlockType CBlock::GetBlockType() const
{
	return m_type;
}

inline Direction CBlock::GetBlockDirection() const
{
	return m_direction;
}

inline Color CBlock::GetBlockColor() const
{
	return m_color;
}

inline int CBlock::GetXPosition() const
{
	return m_x;
}

inline int CBlock::GetYPosition() const
{
	return m_y;
}

void CBlock::SetXPosition(int x_pos)
{
	m_x = x_pos;
	SetBlockPoint();
}

void CBlock::PrintBlock(int x, int y) const
{
	//在显示方块之前先把这块区域用空格填充
	for (int i{ x }; i <= x + 3; i++)
		for (int j{ y }; j <= y + 3; j++)
			PrintString("  ", i, j, WHITE);

	if (m_type == B_BOMB)
	{
		PrintBomb(x, y);
	}
	else
	{
		Cell cell_temp{ FULL_CELL, m_color };
		int x_cell, y_cell;
		for (int i{ 0 }; i < 4; i++)		//依次输出方块的每一个点
		{
			x_cell = x + BlockPointOffset[m_type][m_direction][i][0];
			y_cell = y + BlockPointOffset[m_type][m_direction][i][1];
			PrintCell(cell_temp, x_cell, y_cell);
		}
	}
}
