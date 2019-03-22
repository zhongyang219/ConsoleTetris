//************************************************************
//				本文件为界面类的定义
//************************************************************
#pragma once
#pragma comment (lib,"winmm.lib")
#include"Define.h"
#include"Block.h"
#include"Console.h"
#include"Ini.h"
#include <windows.h>
#include<fstream>
#include<iostream>
using std::ofstream;
using std::ifstream;

class CUI
{
private:
	Cell m_board[WIDTH][HEIGHT];		//储存当前游戏界面中10x20的每个格子的状态和颜色，有FULL_CELL和EMPTY_CELL两种状态

	CBlock CurrentBlock1;		//玩家1的当前方块
	CBlock CurrentBlock2;		//玩家2的当前方块
	CBlock NextBlock1;		//玩家1的下一个方块
	CBlock NextBlock2;		//玩家2的下一个方块

	int m_score{ 0 };		//游戏得分
	int m_level{ 1 };		//游戏等级
	
	int m_strip_number{ 0 };		//长条道具的数量
	int m_bomb_number{ 0 };		//炸弹道具的数量
	int m_pierce_number{ 0 };		//穿甲弹道具的数量

	bool m_load_flag{ false };		//执行了载入操作后，此flag置为true

	int m_board_x{};		//绘制界面的起始x坐标（需要根据界面宽度m_width计算）
	int m_block1_x{};		//双人模式下玩家1控制方块的起始x位置（需要根据界面宽度m_width计算）
	int m_block2_x{};		//双人模式下玩家2控制方块的起始x位置（需要根据界面宽度m_width计算）

	//选项设置数据
	bool m_clear_animation;		//是否显示消除动画
	int m_clear_animation_speed;		//消除每个格子的间隔时间（单位：ms），值越小速度越快
	bool m_allow_level_up;		//是否允许升级
	int m_score_per_level;		//每多少分升级
	Color m_color_table[9];		//储存7种类型的方块+2种道具的颜色
	bool m_atuo_save{ false };		//退出时是否自动存档
	
	bool m_invisible_mode;		//隐形模式
	int m_invisible_mode_difficulty;		//隐形模式难度，0：低，1：中，2高
	bool m_double_player;		//双人模式
	int m_width;		//界面显示的宽度
	bool m_sound_effect;		//游戏音效
	
	//玩家1的键位设定数据
	int m_1_left_key;		//方块左移键
	int m_1_right_key;		//方块右移键
	int m_1_down_key;		//方块下移键
	int m_1_rotate_key;		//方块旋转键
	int m_1_sink_key;		//方块直接下落键
	//玩家2的键位设定数据
	int m_2_left_key;		//方块左移键
	int m_2_right_key;		//方块右移键
	int m_2_down_key;		//方块下移键
	int m_2_rotate_key;		//方块旋转键
	int m_2_sink_key;		//方块直接下落键

	void CalculateLevel();		//计算等级
	void ShowSettingsInfo(int x, int y) const;		//在坐标x,y处显示选项设置中的信息
	void ColorSettings();			//方块颜色设置
	void DrawBoardVisible() const;		//在任何时候都显示界面（用于在隐形模式时显示界面）
	void ShowPause() const;		//显示暂停时的游戏界面
	void Pause();		//暂停处理
	void KeySettings();		//键位设置
	void CalculateBoardXAndBlockPosition();		//根据界面宽度计算绘制界面的起始x坐标和方块起始x坐标
	
public:
	CUI();

	void DrawBoard() const;			//绘制游戏界面
	void DrawCurrentBlock(Player player);		//在游戏界面上绘制当前方块
	void ClearCurrentBlock(Player player, bool print);			//在游戏界面上擦除当前方块，参数表示是否在擦除的同时在界面上绘制出来

	bool BlockLandTest(Player player);			//方块下落判断
	void ClearLine();		//消除整行
	void GetAProp();		//随机获取一个道具
	bool GameOver(Player player) const;	//游戏结束判断

	void SetNextBlock(Player player);		//将下一个方块设置为当前方块

	void KeyDetect(int key);			//检测键盘输入
	void ShowInfo() const;			//显示游戏信息
	void ShowHelpInfo() const;		//显示帮助信息
	bool ShowGameOver() const;		//显示游戏结束时的游戏界面
	void OptionSettings();		//选项设置

	int GetIntervalTime() const;		//根据当前等级计算方块下落时的间隔时间（单位：ms）

	void SaveData() const;		//将游戏数据保存到文件
	void LoadData();		//从文件载入游戏数据
	void SaveSettings() const;		//将游戏设置保存到ini文件
	void LoadSettings();		//从ini文件载入游戏数据

	bool GetAutoSave() const;		//获取是否自动存档选项
	int GetDownKey(Player player) const;		//获取方块下移键的设定
	bool KeyIsControlKey(int key) const;		//用于判断按的键是不是游戏控制键
	bool DoublePlayer() const;		//获取双人模式选项

};


CUI::CUI()
{
	for (int i{ 0 }; i < WIDTH; i++)
	{
		for (int j{ 0 }; j < HEIGHT; j++)
		{
			m_board[i][j].type = EMPTY_CELL;
			m_board[i][j].color = GRAY;
		}
	}
	
	LoadSettings();		//初始化时载入设置
	m_color_table[7] = WHITE;		//炸弹道具颜色初始化
	m_color_table[8] = WHITE;		//穿甲弹道具颜色初始化
	CalculateBoardXAndBlockPosition();

	SYSTEMTIME current_time;
	GetLocalTime(&current_time);			//获取当前时间
	srand(current_time.wMilliseconds);		//用当前时间的毫秒数设置产生随机数的种子
	BlockType type{ static_cast<BlockType>(rand() % 7) };
	//随机生成一个玩家1的当前方块的类型和朝向。双人模式下在BLOCK1_X的位置生成，非双人模式下在界面中间位置生成
	CurrentBlock1 = CBlock(type, static_cast<Direction>(rand() % 4), m_color_table[type], m_double_player ? m_block1_x : m_width / 2 - 1);
	type = static_cast<BlockType>(rand() % 7);
	NextBlock1 = CBlock(type, static_cast<Direction>(rand() % 4), m_color_table[type], m_double_player ? m_block1_x : m_width / 2 - 1);		//随机生成下一个方块1的类型和朝向
	
	type = static_cast<BlockType>(rand()*2 % 7);
	CurrentBlock2 = CBlock(type, static_cast<Direction>(rand() % 4), m_color_table[type], m_block2_x);		//随机生成一个当前方块2的类型和朝向
	type = static_cast<BlockType>(rand() % 7);
	NextBlock2 = CBlock(type, static_cast<Direction>(rand() % 4), m_color_table[type], m_block2_x);		//随机生成下一个方块2的类型和朝向
}



void CUI::DrawBoard() const
{
	int i, j;
	for (i = 0; i < m_width; i++)
		for (j = 0; j < HEIGHT; j++)
			m_invisible_mode ? PrintCell(Cell{ EMPTY_CELL, GRAY }, i + m_board_x, j + BOARD_Y) : PrintCell(m_board[i][j], i + m_board_x, j + BOARD_Y);
}

void CUI::DrawBoardVisible() const
{
	int i, j;
	for (i = 0; i < m_width; i++)
		for (j = 0; j < HEIGHT; j++)
			PrintCell(m_board[i][j], i + m_board_x, j + BOARD_Y);
}

inline void CUI::DrawCurrentBlock(Player player)
{
	if (player == PLAYER1) CurrentBlock1.DrawBlock(m_board, m_width, m_board_x);
	else CurrentBlock2.DrawBlock(m_board, m_width, m_board_x);
}

inline void CUI::ClearCurrentBlock(Player player, bool print)
{
	if (player == PLAYER1) print ? CurrentBlock1.ClearBlock(m_board, m_width, m_board_x) : CurrentBlock1.ClearBlockNotPrint(m_board, m_width);
	else print ? CurrentBlock2.ClearBlock(m_board, m_width, m_board_x) : CurrentBlock2.ClearBlockNotPrint(m_board, m_width);
}

inline bool CUI::BlockLandTest(Player player)
{
	bool land_flag{ false };
	if (player == PLAYER1)
	{
		land_flag = CurrentBlock1.LandTest(m_board, m_width);
		if (land_flag && m_sound_effect && CurrentBlock1.GetBlockType() != B_BOMB) PlaySound(_T("./sound/land.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
	else
	{
		land_flag = CurrentBlock2.LandTest(m_board, m_width);
		if (land_flag && m_sound_effect && CurrentBlock2.GetBlockType() != B_BOMB) PlaySound(_T("./sound/land.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
	return land_flag;
}

void CUI::CalculateLevel()
{
	if (m_allow_level_up)
		m_level = m_score / m_score_per_level + 1;		//每增加特定的得分，等级提升1级
	else
		m_level = 1;		//不允许升级时等一直是1级
}

void CUI::ClearLine()
{
	int i, j;
	int clear_count{ 0 };		//统计消除行的数量
	if(CurrentBlock1.GetBlockType() == B_BOMB)		//当前方块是炸弹时消除炸弹所在区域及其下方4x4共4x8区域内的所有方块
	{
		if (m_sound_effect) PlaySound(_T("./sound/bomb.wav"), NULL, SND_ASYNC | SND_FILENAME);
		for(i = 0; i<4; i++)
			for(j = 0; j<8; j++)
			{
				if (CurrentBlock1.GetYPosition() + j < HEIGHT)
				{
					m_board[CurrentBlock1.GetXPosition()+i][CurrentBlock1.GetYPosition()+j].type = EMPTY_CELL;
					m_board[CurrentBlock1.GetXPosition()+i][CurrentBlock1.GetYPosition()+j].color = GRAY;
					PrintCell(Cell{ EMPTY_CELL, GRAY }, CurrentBlock1.GetXPosition() + i + m_board_x, CurrentBlock1.GetYPosition() + j + BOARD_Y);
				}
			}
	}
	else if (CurrentBlock2.GetBlockType() == B_BOMB)
	{
		if (m_sound_effect) PlaySound(_T("./sound/bomb.wav"), NULL, SND_ASYNC | SND_FILENAME);
		for (i = 0; i<4; i++)
			for (j = 0; j<8; j++)
			{
				if (CurrentBlock2.GetYPosition() + j < HEIGHT)
				{
					m_board[CurrentBlock2.GetXPosition() + i][CurrentBlock2.GetYPosition() + j].type = EMPTY_CELL;
					m_board[CurrentBlock2.GetXPosition() + i][CurrentBlock2.GetYPosition() + j].color = GRAY;
					PrintCell(Cell{ EMPTY_CELL, GRAY }, CurrentBlock2.GetXPosition() + i + m_board_x, CurrentBlock2.GetYPosition() + j + BOARD_Y);
				}
			}
	}
	else
	{
		bool clear_flag{ true };			//当前行可以消除flag
		for (i = HEIGHT - 1; i >= 0; i--)		//从最下面一行开始判断
		{
			for (j = 0; j < m_width; j++)
			{
				if (m_board[j][i].type == EMPTY_CELL)	//只要当前行有一个格子是空的，该行就不能消除
					clear_flag = false;	
			}

			if (clear_flag)		//如果当前行可以消除
			{
				if (m_sound_effect) PlaySound(_T("./sound/clear.wav"), NULL, SND_ASYNC | SND_FILENAME);
				if (m_invisible_mode && m_invisible_mode_difficulty<=1 && clear_count == 0)		//隐形模式且难度为低或中时如果有消除行，则在消除第一行之前显示界面500毫秒
				{
					DrawBoardVisible();
					Sleep(500);
				}
				if (m_clear_animation)		//如果允许显示消除动画，则逐个填充空格子以显示消除时的动画
				{
					if (m_invisible_mode_difficulty <=1)
						DrawBoardVisible();		//刷新一次界面显示
					else DrawBoard();
					if (m_invisible_mode && m_invisible_mode_difficulty == 2)		//在隐形模式且难度为高时如果有消除行，先把当前可消除的行显示出来
					{
						for (j = 0; j < m_width; j++)
						{
							PrintCell(m_board[j][i], j + m_board_x, i + BOARD_Y);
						}
					}
					if (clear_count % 2 == 0)
					{
						for (j = 0; j < m_width; j++)		//从左边的格子开始填充
						{
							PrintCell(Cell{ EMPTY_CELL, GRAY }, j + m_board_x, i + BOARD_Y);		//绘制一个空的格子
							Sleep(m_clear_animation_speed);				//每填充一个格子暂停一段时间
						}
					}
					else
					{
						for (j = m_width - 1; j >= 0; j--)		//从右边的格子开始填充
						{
							PrintCell(Cell{ EMPTY_CELL, GRAY }, j + m_board_x, i + BOARD_Y);		//绘制一个空的格子
							Sleep(m_clear_animation_speed);				//每填充一个格子暂停一段时间
						}
					}
				}

				for (int k{ i }; k>= 0; k--)		//从第i行开始上面的每一行依次下移一行
				{
					if (k == 0)		//如果到了最顶层那一行，则把该行清空
					{
						for (j = 0; j < m_width; j++)
						{
							m_board[j][k].type = EMPTY_CELL;
							m_board[j][k].color = GRAY;
						}
					}
					else
					{
						for (j = 0; j < m_width; j++)
						{
							m_board[j][k] = m_board[j][k - 1];
						}
					}
				}
				i++;		//如果把当前行消除了，仍然要从当前行开始判断，i就不能自减，在这里自加1
				clear_count++;		//消除行的数量加1
			}
			clear_flag = true;		//判断完一行后clear_flag置为true
		}
	}
	
	if (m_invisible_mode)
	{
		if(m_invisible_mode_difficulty == 0)		//隐形模式难度为低时，每次方块每次下落都显示界面界面500毫秒
		{
			DrawBoardVisible();
		}
		else if(m_invisible_mode_difficulty == 1)			//隐形模式难度为中时，如果有消除行，在消除后再显示界面500毫秒
		{
			if (clear_count > 0) DrawBoardVisible();
		}
		Sleep(500);
		DrawBoard();		//隐形模式时每次方块下落时都刷新界面
	}
	else
	{
		if (clear_count > 0) DrawBoard();		//非隐形模式时有消除行时重新刷新界面
	}
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);			//获取当前时间
	srand(current_time.wMilliseconds);		//用当前时间的毫秒数设置产生随机数的种子
	switch (clear_count)		//根据消除的行数增加相应的得分及获得道具
	{
		case 1:
			m_score += 1;
			if (rand() % 10 == 0) GetAProp();		//消除1行1/10的概率获得道具
			break;
		case 2:
			m_score += 3;
			if (rand() % 4 == 0) GetAProp();		//消除2行1/4的概率获得道具
			break;
		case 3:
			m_score += 5;
			if (rand() % 2 == 0) GetAProp();		//消除3行1/2的概率获得道具
			break;
		case 4:
			m_score += 8;
			GetAProp();		//消除4行必定获得一个道具
			break;
		default: break;
	}
	CalculateLevel();		//计算等级
}

void CUI::GetAProp()
{
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);			//获取当前时间
	srand(current_time.wMilliseconds);		//用当前时间的毫秒数设置产生随机数的种子
	//随机获得一个道具，但道具数量最多999个
	switch(rand() % 3)
	{
		case 0: if (m_strip_number < 1000) m_strip_number++; break;
		case 1: if (m_bomb_number < 1000) m_bomb_number++; break;
		case 2: if (m_pierce_number < 1000) m_pierce_number++; break;
	}
}

inline bool CUI::GameOver(Player player) const
{
	if (player == PLAYER1)
	{
		if (CurrentBlock1.GetBlockType() == B_BOMB) return false;		//当前方块是炸弹时不会使游戏结束
		else return CurrentBlock1.BlockCollisionTest(m_board, m_width);		//如果当前方块在一出来的时候就检测到碰撞，则游戏结束
	}
	else
	{
		if (CurrentBlock2.GetBlockType() == B_BOMB) return false;		//当前方块是炸弹时不会使游戏结束
		else return CurrentBlock2.BlockCollisionTest(m_board, m_width);		//如果当前方块在一出来的时候就检测到碰撞，则游戏结束
	}
}


void CUI::SetNextBlock(Player player)
{
	SYSTEMTIME current_time;
	GetLocalTime(&current_time);			//获取当前时间
	if (player == PLAYER1)
	{
		CurrentBlock1 = NextBlock1;
		srand(current_time.wMilliseconds);		//用当前时间的毫秒数设置产生随机数的种子
		BlockType type{ static_cast<BlockType>(rand() % 7) };
		NextBlock1 = CBlock(type, static_cast<Direction>(rand() % 4), m_color_table[type], m_double_player ? m_block1_x : m_width / 2 - 1);		//重新随机生成下一个方块的类型和朝向
	}
	else
	{
		CurrentBlock2 = NextBlock2;
		srand(current_time.wMilliseconds);		//用当前时间的毫秒数设置产生随机数的种子
		BlockType type{ static_cast<BlockType>(rand() % 7) };
		NextBlock2 = CBlock(type, static_cast<Direction>(rand() % 4), m_color_table[type], m_block2_x);		//重新随机生成下一个方块的类型和朝向
	}
}


void CUI::KeyDetect(int key)
{
	if (key == m_1_left_key)
	{
		CurrentBlock1.MoveBlock(LEFT, m_board, m_width);		//执行左移操作
	}
	else if (key == m_1_right_key)
	{
		CurrentBlock1.MoveBlock(RIGHT, m_board, m_width);	//执行右移操作
	}
	else if (key == m_1_down_key)
	{
		CurrentBlock1.MoveBlock(DOWN, m_board, m_width);		//执行下移操作
	}
	else if (key == m_1_rotate_key)
	{
		if (m_sound_effect) PlaySound(_T("./sound/rotate.wav"), NULL, SND_ASYNC | SND_FILENAME);
		CurrentBlock1.RotateBlock(m_board, m_width);		//执行旋转操作
	}
	else if (key == m_1_sink_key)
	{
		while (!BlockLandTest(PLAYER1))			//按下下沉键时一直执行向下操作，直到方块落下为止
			CurrentBlock1.MoveBlock(DOWN, m_board, m_width);
		CurrentBlock1.MoveBlock(DOWN, m_board, m_width);		//while循环结束之后再执行一次下落操作
	}
	else if (key == m_2_left_key && m_double_player)
	{
		CurrentBlock2.MoveBlock(LEFT, m_board, m_width);		//执行左移操作
	}
	else if (key == m_2_right_key && m_double_player)
	{
		CurrentBlock2.MoveBlock(RIGHT, m_board, m_width);	//执行右移操作
	}
	else if (key == m_2_down_key && m_double_player)
	{
		CurrentBlock2.MoveBlock(DOWN, m_board, m_width);		//执行下移操作
	}
	else if (key == m_2_rotate_key && m_double_player)
	{
		if (m_sound_effect) PlaySound(_T("./sound/rotate.wav"), NULL, SND_ASYNC | SND_FILENAME);
		CurrentBlock2.RotateBlock(m_board, m_width);		//执行旋转操作
	}
	else if (key == m_2_sink_key && m_double_player)
	{
		while (!BlockLandTest(PLAYER2))			//按下下沉键时一直执行向下操作，直到方块落下为止
			CurrentBlock2.MoveBlock(DOWN, m_board, m_width);
		CurrentBlock2.MoveBlock(DOWN, m_board, m_width);		//while循环结束之后再执行一次下落操作
	}
	else
	{
		switch (key)
		{
			case CHEAT_KEY:		//按下作弊键，随机获取一个道具
				GetAProp();
				ShowInfo();
				break;
			case LEVELUP_KEY:		//按下“一键升级”键，增加升1级所需的得分
				m_score += m_score_per_level;
				CalculateLevel();
				ShowInfo();
				break;
			case PLAYER1_STRIP1_KEY:		//按下道具1键，使用长条道具
				if (m_strip_number > 0 && NextBlock1.GetBlockType() != B_I)
				{
					NextBlock1 = CBlock(B_I, static_cast<Direction>(rand() % 4), m_color_table[B_I], m_block1_x);
					m_strip_number--;
					ShowInfo();
				}
				break;
			case PLAYER1_STRIP2_KEY:		//按下道具2键，使用炸弹道具
				if (m_bomb_number > 0 && NextBlock1.GetBlockType() != B_BOMB)
				{
					NextBlock1 = CBlock(B_BOMB, UP, m_color_table[B_BOMB], m_block1_x);
					m_bomb_number--;
					ShowInfo();
				}
				break;
			case PLAYER1_STRIP3_KEY:		//按下道具3键，使用穿甲弹道具
				if (m_pierce_number > 0 && NextBlock1.GetBlockType() != B_PIERCE)
				{
					NextBlock1 = CBlock(B_PIERCE, UP, m_color_table[B_PIERCE], m_block1_x);
					m_pierce_number--;
					ShowInfo();
				}
				break;
			case PLAYER2_STRIP1_KEY:		//按下道具1键，使用长条道具
				if (m_strip_number > 0 && NextBlock2.GetBlockType() != B_I && m_double_player)
				{
					NextBlock2 = CBlock(B_I, static_cast<Direction>(rand() % 4), m_color_table[B_I], m_block2_x);
					m_strip_number--;
					ShowInfo();
				}
				break;
			case PLAYER2_STRIP2_KEY:		//按下道具2键，使用炸弹道具
				if (m_bomb_number > 0 && NextBlock2.GetBlockType() != B_BOMB && m_double_player)
				{
					NextBlock2 = CBlock(B_BOMB, UP, m_color_table[B_BOMB], m_block2_x);
					m_bomb_number--;
					ShowInfo();
				}
				break;
			case PLAYER2_STRIP3_KEY:		//按下道具3键，使用穿甲弹道具
				if (m_pierce_number > 0 && NextBlock2.GetBlockType() != B_PIERCE && m_double_player)
				{
					NextBlock2 = CBlock(B_PIERCE, UP, m_color_table[B_PIERCE], m_block2_x);
					m_pierce_number--;
					ShowInfo();
				}
				break;
			case PAUSE_KEY:		//按下暂停键，执行暂停时的处理
				Pause();
				ClearCurrentBlock(PLAYER1, true);
				ClearCurrentBlock(PLAYER2, true);
				system("cls");
				ShowInfo();				//从暂停状态恢复后更新界面信息
				if (m_invisible_mode && m_load_flag == true)		//隐形模式下如果执行了载入操作，则从暂停状态恢复后显示界面500毫秒
				{
					DrawBoardVisible();
					Sleep(500);
					m_load_flag = false;
				}
				DrawBoard();
				break;
			default: break;
		}
	}
}

void CUI::ShowInfo() const
{
	const int x_left_info{ 1 };		//左侧信息的起始x坐标
	const int y_left_info{ 4 };		//左侧信息的起始y坐标
	const int x_right_info{ 29 };		//右侧信息的起始x坐标
	const int y_right_info{ 3 };		//右侧信息的起始y坐标
	PrintString(L"俄罗斯方块    版本：", 2, 0, WHITE);
	PrintString(L"1.93", 12, 0, YELLOW);
	PrintString(L"作者：ZY    邮箱：380526481@qq.com", 16, 0, WHITE);
	PrintString(L"得分：", x_left_info, y_left_info, WHITE);
	PrintInt(m_score, x_left_info + 3, y_left_info, CYAN);
	if (m_allow_level_up)		//只有允许升级时才显示等级信息
	{
		PrintString(L"等级：", x_left_info, y_left_info + 2, WHITE);
		PrintInt(m_level, x_left_info + 3, y_left_info + 2, CYAN);
	}
	
	if (m_strip_number > 0)
	{
		PrintString(L"1.长条", x_left_info, y_left_info + 6, GREEN);
		PrintString(L"×", x_left_info + 3, y_left_info + 6, YELLOW);
		PrintString(L"   ", x_left_info + 4, y_left_info + 6, WHITE);
		PrintInt(m_strip_number, x_left_info + 4, y_left_info + 6, YELLOW);
	}
	else PrintString(L"           ", x_left_info, y_left_info + 6, WHITE);
	if (m_bomb_number > 0)
	{
		PrintString(L"2.炸弹", x_left_info, y_left_info + 7, GREEN);
		PrintString(L"×", x_left_info + 3, y_left_info + 7, YELLOW);
		PrintString(L"   ", x_left_info + 4, y_left_info + 7, WHITE);
		PrintInt(m_bomb_number, x_left_info + 4, y_left_info + 7, YELLOW);
	}
	else PrintString(L"           ", x_left_info, y_left_info + 7, WHITE);
	if (m_pierce_number > 0)
	{
		PrintString(L"3.穿甲弹", x_left_info, y_left_info + 8, GREEN);
		PrintString(L"×", x_left_info + 4, y_left_info + 8, YELLOW);
		PrintString(L"   ", x_left_info + 5, y_left_info + 8, WHITE);
		PrintInt(m_pierce_number, x_left_info + 5, y_left_info + 8, YELLOW);
	}
	else PrintString(L"             ", x_left_info, y_left_info + 8, WHITE);

	PrintString(m_double_player ? L"玩家1下一个方块：" : L"下一个方块：", x_right_info, y_right_info, WHITE);
	NextBlock1.PrintBlock(x_right_info + 1, y_right_info + 2);	//输出玩家1的下一个方块

	if (m_double_player)		//只有双人模式下才显示玩家2的下一个方块
	{
		PrintString(L"玩家2下一个方块：", x_right_info, y_right_info+7, WHITE);
		NextBlock2.PrintBlock(x_right_info + 1, y_right_info + 9);	//输出玩家2的下一个方块
	}

	PrintString(L"帮助：", x_right_info, y_right_info + 14, WHITE);
	PrintString(L"按回车键暂停，", x_right_info, y_right_info + 15, WHITE);
	PrintString(L"按ESC键退出，", x_right_info, y_right_info + 16, WHITE);
	PrintString(L"按O键进入选项设置，", x_right_info, y_right_info + 17, WHITE);
	PrintString(L"按H键查看详细帮助信息。", x_right_info, y_right_info + 18, WHITE);
}

void CUI::ShowHelpInfo() const
{
	const int x{ 9 }, y{ 3 };		//选项界面字符输出的起始位置
	system("cls");
	PrintString(L"详细帮助", x, y, CYAN);
	PrintString(L"玩家1默认按键：", x, y + 2, WHITE);
	PrintString(L"A：左移；D：右移；S：下移；W：旋转；空格：直接下落", x, y + 3, WHITE);
	PrintString(L"玩家2默认按键：", x, y + 4, WHITE);
	PrintString(L"←：左移；→：右移；↓：下移；↑：旋转；/：直接下落", x, y + 5, WHITE);
	PrintString(L"玩家1按1、2、3键使用道具，玩家2按8、9、0键使用道具", x, y + 6, WHITE);
	PrintString(L"按ESC键返回。", x, y + 8, GRAY);
	while (GetKey() != QUIT_KEY) {}
	system("cls");
}

bool CUI::ShowGameOver() const
{
	int i, j;
	for (i = 0; i < m_width; i++)
		for (j = 0; j < HEIGHT; j++)
			PrintCell(m_board[i][j], i + m_board_x, j + BOARD_Y, RED);
	PrintString(L"               Game Over!               ", BOARD_X, BOARD_Y + 10, RED);
	PrintString(L"          是否继续?是(Y)|否(N)          ", BOARD_X, BOARD_Y + 11, RED);
	while (true)
	{
		switch (GetKey())
		{
			case 'Y': return true;
			case 'N': case QUIT_KEY: return false;
			default: break;
		}
	}
}


void CUI::ShowPause() const
{
	int i, j;
	for (i = 0; i < m_width; i++)
		for (j = 0; j < HEIGHT; j++)
			PrintCell(m_board[i][j], i + m_board_x, j + BOARD_Y, CYAN);
	PrintString(L"        已暂停，按回车键继续……        ", BOARD_X, BOARD_Y+9, CYAN);
	PrintString(L"      S：保存    L：载入    O：选项     ", BOARD_X, BOARD_Y + 10, CYAN);
}

void CUI::Pause()
{
	ShowInfo();
	ShowPause();		//显示游戏暂停时的界面
	int key;
	while (true)
	{
		key = GetKey();
		if (key == QUIT_KEY || key == PAUSE_KEY)		//按ESC键或暂停键返回
		{
			break;
		}
		if (key == 'S')		//按S键保存
		{
			ClearCurrentBlock(PLAYER1, false);		//保存前必须要先清除当前方块
			ClearCurrentBlock(PLAYER2, false);
			SaveData();
			PrintString(L"                已保存。                ", BOARD_X, BOARD_Y+11, CYAN);
		}
		if (key == 'L')		//按L键载入
		{
			LoadData();
			PrintString(L"                已载入。                ", BOARD_X, BOARD_Y + 11, CYAN);
			SetNextBlock(PLAYER1);			//载入之后舍弃当前方块，设置下一个方块
			SetNextBlock(PLAYER2);
			m_load_flag = true;
		}
		if (key == 'O')		//按O键进入选项设置
		{
			OptionSettings();
			ShowInfo();
			ShowPause();
		}
	}
}

int CUI::GetIntervalTime() const
{
	switch (m_level)
	{
		case 1: return 1000;
		case 2: return 800;
		case 3: return 650;
		case 4: return 500;
		case 5: return 400;
		case 6: return 320;
		case 7: return 250;
		case 8: return 190;
		default: return 150;
	}
}

void CUI::SaveData() const
{
	ofstream SaveFile{ "GameData.dat" };
	for (int j{ 0 }; j < HEIGHT; j++)
	{
		for (int i{ 0 }; i < WIDTH; i++)
		{
			if(m_board[i][j].type == FULL_CELL)
				SaveFile << 1;
			else SaveFile << 0;
			switch (m_board[i][j].color)
			{
				case	BLUE: SaveFile << 'B'; break;
				case GREEN: SaveFile << 'G'; break;
				case CYAN: SaveFile << 'C'; break;
				case RED: SaveFile << 'R'; break;
				case PURPLE: SaveFile << 'P'; break;
				case YELLOW: SaveFile << 'Y'; break;
				case WHITE: SaveFile << 'W'; break;
				default: SaveFile << 'N'; break;
			}
		}
	}
	SaveFile << m_score;
	SaveFile << ',';
	SaveFile << m_strip_number;
	SaveFile << ',';
	SaveFile << m_bomb_number;
	SaveFile << ',';
	SaveFile << m_pierce_number;
	SaveFile << ',';
	SaveFile.close();
}

void CUI::LoadData()
{
	ifstream OpenFile{ "GameData.dat" };
	char ch;
	//读取界面每个格子状态
	for (int j{ 0 }; j < HEIGHT; j++)
	{
		for (int i{ 0 }; i < WIDTH; i++)
		{
			OpenFile.get(ch);
			if (ch == '1')
				m_board[i][j].type = FULL_CELL;
			else
				m_board[i][j].type = EMPTY_CELL;
			OpenFile.get(ch);
			switch (ch)
			{
				case 'B': m_board[i][j].color = BLUE; break;
				case 'G': m_board[i][j].color = GREEN; break;
				case 'C': m_board[i][j].color = CYAN; break;
				case 'R': m_board[i][j].color = RED; break;
				case 'P': m_board[i][j].color = PURPLE; break;
				case 'Y': m_board[i][j].color = YELLOW; break;
				case 'W': m_board[i][j].color = WHITE; break;
				default:	 m_board[i][j].color = GRAY; break;
			}
		}
	}
	int i{ 0 };
	char str[10];
	//读取得分
	do OpenFile.get(str[i++]);
	while (str[i - 1] != ',' &&i < 10);
	str[i - 1] = '\0';
	m_score = atoi(str);		//将读取到的字符串转换成数字保存到m_score中
	if (m_score < 0) m_score = 0;
	//读取长条道具的个数
	i = 0;
	do OpenFile.get(str[i++]);
	while (str[i - 1] != ',' &&i < 10);
	str[i - 1] = '\0';
	m_strip_number = atoi(str);
	if (m_strip_number > 999) m_strip_number = 999;
	//读取炸弹道具的个数
	i = 0;
	do OpenFile.get(str[i++]);
	while (str[i - 1] != ',' &&i < 10);
	str[i - 1] = '\0';
	m_bomb_number = atoi(str);
	if (m_bomb_number > 999) m_bomb_number = 999;
	//读取穿甲弹道具的个数
	i = 0;
	do OpenFile.get(str[i++]);
	while (str[i - 1] != ',' &&i < 10);
	str[i - 1] = '\0';
	m_pierce_number = atoi(str);
	if (m_pierce_number > 999) m_pierce_number = 999;
	OpenFile.close();
}

void CUI::SaveSettings() const
{
	WriteIntToIni("配置", "消除动画", m_clear_animation);
	WriteIntToIni("配置", "消除动画间隔时间", m_clear_animation_speed);
	WriteIntToIni("配置", "允许升级", m_allow_level_up);
	WriteIntToIni("配置", "提升1级所需得分", m_score_per_level);
	WriteIntToIni("配置", "自动存档", m_atuo_save);
	WriteIntToIni("配置", "隐形模式", m_invisible_mode);
	WriteIntToIni("配置", "隐形模式难度", m_invisible_mode_difficulty);
	WriteIntToIni("配置", "双人模式", m_double_player);
	WriteIntToIni("配置", "界面显示的宽度", m_width);
	WriteIntToIni("配置", "游戏音效", m_sound_effect);

	WriteIntToIni("颜色设置", "I型方块颜色", m_color_table[0]);
	WriteIntToIni("颜色设置", "L型方块颜色", m_color_table[1]);
	WriteIntToIni("颜色设置", "J型方块颜色", m_color_table[2]);
	WriteIntToIni("颜色设置", "Z型方块颜色", m_color_table[3]);
	WriteIntToIni("颜色设置", "S型方块颜色", m_color_table[4]);
	WriteIntToIni("颜色设置", "T型方块颜色", m_color_table[5]);
	WriteIntToIni("颜色设置", "O型方块颜色", m_color_table[6]);
	
	WriteIntToIni("按键设置", "玩家1左移键", m_1_left_key);
	WriteIntToIni("按键设置", "玩家1右移键", m_1_right_key);
	WriteIntToIni("按键设置", "玩家1下移键", m_1_down_key);
	WriteIntToIni("按键设置", "玩家1旋转键", m_1_rotate_key);
	WriteIntToIni("按键设置", "玩家1直接下落键", m_1_sink_key);
	WriteIntToIni("按键设置", "玩家2左移键", m_2_left_key);
	WriteIntToIni("按键设置", "玩家2右移键", m_2_right_key);
	WriteIntToIni("按键设置", "玩家2下移键", m_2_down_key);
	WriteIntToIni("按键设置", "玩家2旋转键", m_2_rotate_key);
	WriteIntToIni("按键设置", "玩家2直接下落键", m_2_sink_key);
}

void CUI::LoadSettings()
{
	m_clear_animation = (GetIntFromIni("配置", "消除动画", 1) != 0);
	m_clear_animation_speed = GetIntFromIni("配置", "消除动画间隔时间", 40);
	if (m_clear_animation_speed < 5) m_clear_animation_speed = 40;		//如果载入了非法的值，则设置成默认值
	m_allow_level_up = (GetIntFromIni("配置", "允许升级", 1) != 0);
	m_score_per_level = GetIntFromIni("配置", "提升1级所需得分", 150);
	if (m_score_per_level < 1) m_score_per_level = 150;		//如果载入了非法的值，则设置成默认值
	m_atuo_save = (GetIntFromIni("配置", "自动存档", 0) != 0);
	m_invisible_mode = (GetIntFromIni("配置", "隐形模式", 0) != 0);
	m_invisible_mode_difficulty = GetIntFromIni("配置", "隐形模式难度", 0);
	if (m_invisible_mode_difficulty > 2) m_invisible_mode_difficulty = 2;
	if (m_invisible_mode_difficulty < 0) m_invisible_mode_difficulty = 0;
	m_double_player = (GetIntFromIni("配置", "双人模式", 0) != 0);
	m_width = GetIntFromIni("配置", "界面显示的宽度", 10);
	if (m_width < 10) m_width = 10;
	if (m_width > 20) m_width = 20;
	m_sound_effect = (GetIntFromIni("配置", "游戏音效", 1) != 0);

	m_color_table[0] = static_cast<Color>(GetIntFromIni("颜色设置", "I型方块颜色", WHITE));
	m_color_table[1] = static_cast<Color>(GetIntFromIni("颜色设置", "L型方块颜色", RED));
	m_color_table[2] = static_cast<Color>(GetIntFromIni("颜色设置", "J型方块颜色", GREEN));
	m_color_table[3] = static_cast<Color>(GetIntFromIni("颜色设置", "Z型方块颜色", BLUE));
	m_color_table[4] = static_cast<Color>(GetIntFromIni("颜色设置", "S型方块颜色", YELLOW));
	m_color_table[5] = static_cast<Color>(GetIntFromIni("颜色设置", "T型方块颜色", CYAN));
	m_color_table[6] = static_cast<Color>(GetIntFromIni("颜色设置", "O型方块颜色", PURPLE));
	
	m_1_left_key = GetIntFromIni("按键设置", "玩家1左移键", 'A');
	m_1_right_key = GetIntFromIni("按键设置", "玩家1右移键", 'D');
	m_1_down_key = GetIntFromIni("按键设置", "玩家1下移键", 'S');
	m_1_rotate_key = GetIntFromIni("按键设置", "玩家1旋转键", 'W');
	m_1_sink_key = GetIntFromIni("按键设置", "玩家1直接下落键", SPACE_KEY);
	m_2_left_key = GetIntFromIni("按键设置", "玩家2左移键", LEFT_KEY);
	m_2_right_key = GetIntFromIni("按键设置", "玩家2右移键", RIGHT_KEY);
	m_2_down_key = GetIntFromIni("按键设置", "玩家2下移键", DOWN_KEY);
	m_2_rotate_key = GetIntFromIni("按键设置", "玩家2旋转键", UP_KEY);
	m_2_sink_key = GetIntFromIni("按键设置", "玩家2直接下落键", '/');
}

inline void CUI::ShowSettingsInfo(int x, int y) const
{
	system("cls");
	PrintString(L"选项设置", x, y, CYAN);
	PrintString(L"1、显示消除动画：", x, y+2, WHITE);
	PrintString(L"2、消除动画间隔时间（单位：ms）：", x, y + 3, WHITE);
	PrintString(L"3、允许升级：", x, y + 4, WHITE);
	PrintString(L"4、提升1级所需的得分：", x, y + 5, WHITE);
	PrintString(L"5、退出时自动存档：", x, y + 6, WHITE);
	PrintString(L"6、隐形模式：", x, y + 7, WHITE);
	PrintString(L"7、隐形模式难度：", x, y + 8, WHITE);
	PrintString(L"8、方块颜色设置", x, y + 9, WHITE);
	PrintString(L"9、键位设置", x, y + 10, WHITE);
	PrintString(L"10、双人模式：", x, y + 11, WHITE);
	PrintString(L"11、界面宽度：", x, y + 12, WHITE);
	PrintString(L"12、游戏音效：", x, y + 13, WHITE);

	PrintString(L"按上下方向键选择项目，回车键确认，ESC键返回", x, y+16, GRAY);
}

void CUI::OptionSettings()
{
	const int x{ 9 }, y{ 3 };		//选项界面字符输出的起始位置
	int item_select{ 1 };		//选中的项目
	const int max_selection{ 12 };		//选项总数
	ShowSettingsInfo(x, y);		//显示选项界面中固定的信息
	
	while (true)
	{
		//显示选项界面中需要变化的信息
		PrintString(m_clear_animation ? L"是" : L"否", x + 9, y + 2, YELLOW);
		PrintInt(m_clear_animation_speed, x + 17, y + 3, YELLOW);
		PrintString(m_allow_level_up ? L"是" : L"否", x + 7, y + 4, YELLOW);
		PrintInt(m_score_per_level, x + 12, y + 5, YELLOW);
		PrintString(m_atuo_save ? L"是" : L"否", x + 10, y + 6, YELLOW);
		PrintString(m_invisible_mode ? L"开" : L"关", x + 7, y + 7, YELLOW);
		switch(m_invisible_mode_difficulty)
		{
			case 0: PrintString(L"低", x + 9, y + 8, YELLOW); break;
			case 1: PrintString(L"中", x + 9, y + 8, YELLOW); break;
			case 2: PrintString(L"高", x + 9, y + 8, YELLOW); break;
			default: break;
		}
		PrintString(m_double_player ? L"开" : L"关", x + 7, y + 11, YELLOW);
		PrintInt(m_width, x + 7, y + 12, YELLOW);
		PrintString(m_sound_effect ? L"开" : L"关", x + 7, y + 13, YELLOW);

		//输出选项指示字符
		for (int i{ 1 }; i <= max_selection; i++)
			PrintString(L"  ", x - 2, y + 1 + i, WHITE);		//先在输出选项指示字符的区域输出空格
		PrintString(L"◆", x - 2, y + 1 + item_select, CYAN);

		switch (GetKey())
		{
			case UP_KEY: 
				item_select--;
				if (item_select < 1) item_select = max_selection;
				break;
			case DOWN_KEY:
				item_select++;
				if (item_select > max_selection) item_select = 1;
				break;
			case ENTER_KEY: case LEFT_KEY: case RIGHT_KEY:
				switch (item_select)
				{
					case 1: m_clear_animation = !m_clear_animation; break;
					case 2: 
						GotoXY(x + 17, y + 3);
						CursorVisible(true);
						std::cin >> m_clear_animation_speed;
						if (m_clear_animation_speed < 5) m_clear_animation_speed = 5;		//消除动画的间隔时间不允许小于5毫秒
						if (m_clear_animation_speed > 9999) m_clear_animation_speed = 9999;		//消除动画的间隔时间不允许大于9999毫秒
						CursorVisible(false);
						PrintString(L"          ", x + 17, y + 3, WHITE);		//用空格覆盖掉原来的字符
						break;
					case 3:
						m_allow_level_up = !m_allow_level_up;
						CalculateLevel();
						break;
					case 4:
						GotoXY(x + 12, y + 5);
						CursorVisible(true);
						std::cin >> m_score_per_level;
						if (m_score_per_level < 1) m_score_per_level = 1;		//每升一级需要的得分不允许小于1
						CalculateLevel();
						CursorVisible(false);
						PrintString(L"          ", x + 12, y + 5, WHITE);			//用空格覆盖掉原来的字符
						break;
					case 5:
						m_atuo_save = !m_atuo_save;
						break;
					case 6:
						m_invisible_mode = !m_invisible_mode;
						break;
					case 7:
						m_invisible_mode_difficulty ++;
						if (m_invisible_mode_difficulty > 2) m_invisible_mode_difficulty = 0;
						break;
					case 8:
						ColorSettings();
						ShowSettingsInfo(x, y);			//从颜色设置的二级菜单退出重新刷新一次信息
						break;
					case 9:
						KeySettings();
						ShowSettingsInfo(x, y);			//从键位设置的二级菜单退出重新刷新一次信息
						break;
					case 10:
						m_double_player = !m_double_player;
						NextBlock1.SetXPosition(m_double_player ? m_block1_x : m_width / 2 - 1);		//切换单人/双人模式后，重新设置玩家1下一个方块的位置
						if (m_double_player)
						{
							NextBlock2.SetXPosition(m_block2_x);		//当从单人模式切换为双人模式时，下一个方块的x位置设置为双人模式下的位置
							SetNextBlock(PLAYER2);		//舍弃玩家2的当前方块，设定下一个方块
						}
						break;
					case 11:
						//由于改变界面宽度可能要改变当前方块的位置，所以需要在宽度改变前先清除当前方块
						ClearCurrentBlock(PLAYER1, false);
						ClearCurrentBlock(PLAYER2, false);

						GotoXY(x + 7, y + 12);
						CursorVisible(true);
						std::cin >> m_width;
						if (m_width < 10) m_width = 10;
						if (m_width > 20) m_width = 20;
						CursorVisible(false);
						PrintString(L"  ", x + 7, y + 12, WHITE);			//用空格覆盖掉原来的字符

						CalculateBoardXAndBlockPosition();
						NextBlock1.SetXPosition(m_double_player ? m_block1_x : m_width / 2 - 1);		//修改界面宽度后，重新设置下一个方块的位置
						NextBlock2.SetXPosition(m_block2_x);
						//当设置的界面宽度比原来小时，如果当前方块已经在界面外面，需要将它移动到里面，防止判断为游戏结束
						if (CurrentBlock1.BlockCollisionTest(m_board, m_width))
							CurrentBlock1.SetXPosition(m_double_player ? m_block1_x : m_width / 2 - 1);
						if (CurrentBlock2.BlockCollisionTest(m_board, m_width))
							CurrentBlock2.SetXPosition(m_block2_x);
						break;
					case 12:
						m_sound_effect = !m_sound_effect;
						break;
					default:
						break;
				}
				SaveSettings();		//选项设置更改后保存设置
				break;
			case QUIT_KEY:
				system("cls");
				return;
			default: break;
		}
	}
}

void CUI::ColorSettings()
{
	const int x{ 9 }, y{ 3 };		//选项界面字符输出的起始位置
	const int max_selection{ 8 };		//选项总数
	int color_select{ 1 };		//方块颜色设置中选中的项目
	system("cls");
	//显示选项界面中固定的信息
	PrintString(L"方块颜色设置", x, y, CYAN);
	PrintString(L"I型：", x, y + 2, WHITE);
	PrintString(L"L型：", x, y + 3, WHITE);
	PrintString(L"J型：", x, y + 4, WHITE);
	PrintString(L"Z型：", x, y + 5, WHITE);
	PrintString(L"S型：", x, y + 6, WHITE);
	PrintString(L"T型：", x, y + 7, WHITE);
	PrintString(L"O型：", x, y + 8, WHITE);
	PrintString(L"恢复默认颜色", x, y + 9, WHITE);
	
	PrintString(L"按上下方向键选择项目，左右方向键选择颜色，ESC键返回", x, y+12, GRAY);

	while (true)
	{
		//输出颜色设置的选项指示字符
		for (int i{ 1 }; i <= max_selection; i++)
			PrintString(L"  ", x - 2, y + 1 + i, WHITE);
		PrintString(L"◆", x - 2, y + 1 + color_select, CYAN);
		//显示选项界面中需要变化的信息
		PrintColor(x + 3, y + 2, m_color_table[B_I]);
		PrintColor(x + 3, y + 3, m_color_table[B_L]);
		PrintColor(x + 3, y + 4, m_color_table[B_J]);
		PrintColor(x + 3, y + 5, m_color_table[B_Z]);
		PrintColor(x + 3, y + 6, m_color_table[B_S]);
		PrintColor(x + 3, y + 7, m_color_table[B_T]);
		PrintColor(x + 3, y + 8, m_color_table[B_O]);
		
		switch (GetKey())
		{
			case UP_KEY:
				color_select--;
				if (color_select < 1) color_select = max_selection;
				break;
			case DOWN_KEY:
				color_select++;
				if (color_select > max_selection) color_select = 1;
				break;
			case ENTER_KEY: case RIGHT_KEY:
				int temp;
				if (color_select == 8)		//选择了“恢复默认颜色”
				{
					m_color_table[B_I] = WHITE;
					m_color_table[B_L] = RED;
					m_color_table[B_J] = GREEN;
					m_color_table[B_Z] = BLUE;
					m_color_table[B_S] = YELLOW;
					m_color_table[B_T] = CYAN;
					m_color_table[B_O] = PURPLE;
				}
				else
				{
					temp = static_cast<int>(m_color_table[color_select-1]);			//由于枚举类型不能自加，所以先将颜色类型强制转换成int型，自加后转换回颜色类型
					temp++;
					m_color_table[color_select-1] = static_cast<Color>(temp);
					if (m_color_table[color_select-1] > WHITE)
						m_color_table[color_select-1] = GRAY;
				}
				break;
			case LEFT_KEY:
				if (color_select < 8)
				{
					temp = static_cast<int>(m_color_table[color_select - 1]);
					temp--;
					m_color_table[color_select - 1] = static_cast<Color>(temp);
					if (m_color_table[color_select - 1] < GRAY)
						m_color_table[color_select - 1] = WHITE;
				}
				break;
			case QUIT_KEY:
				return;
			default:
				break;
		}
	}
}

void CUI::KeySettings()
{
	const int x{ 9 }, y{ 3 };		//选项界面字符输出的起始位置
	const int max_selection{ 11 };		//选项总数
	int item_select{ 1 };		//键位设置中选中的项目
	system("cls");
	//显示选项界面中固定的信息
	PrintString(L"键位设置", x, y, CYAN);
	PrintString(L"玩家1左移键：", x, y + 2, WHITE);
	PrintString(L"玩家1右移键：", x, y + 3, WHITE);
	PrintString(L"玩家1下移键：", x, y + 4, WHITE);
	PrintString(L"玩家1旋转键：", x, y + 5, WHITE);
	PrintString(L"玩家1直接下落键：", x, y + 6, WHITE);
	PrintString(L"玩家2左移键：", x, y + 7, WHITE);
	PrintString(L"玩家2右移键：", x, y + 8, WHITE);
	PrintString(L"玩家2下移键：", x, y + 9, WHITE);
	PrintString(L"玩家2旋转键：", x, y + 10, WHITE);
	PrintString(L"玩家2直接下落键：", x, y + 11, WHITE);
	PrintString(L"恢复默认设定", x, y + 12, WHITE);
	
	PrintString(L"按上下方向键选择项目，回车键进入相应的键位设置，ESC键返回", x, y+15, GRAY);

	while (true)
	{
		//输出键位设置的选项指示字符
		for (int i{ 1 }; i <= max_selection; i++)
			PrintString(L"  ", x - 2, y + 1 + i, WHITE);
		PrintString(L"◆", x - 2, y + 1 + item_select, CYAN);
		//显示选项界面中需要变化的信息
		PrintKey(m_1_left_key, x + 7, y + 2, YELLOW);
		PrintKey(m_1_right_key, x + 7, y + 3, YELLOW);
		PrintKey(m_1_down_key, x + 7, y + 4, YELLOW);
		PrintKey(m_1_rotate_key, x + 7, y + 5, YELLOW);
		PrintKey(m_1_sink_key, x + 9, y + 6, YELLOW);
		PrintKey(m_2_left_key, x + 7, y + 7, YELLOW);
		PrintKey(m_2_right_key, x + 7, y + 8, YELLOW);
		PrintKey(m_2_down_key, x + 7, y + 9, YELLOW);
		PrintKey(m_2_rotate_key, x + 7, y + 10, YELLOW);
		PrintKey(m_2_sink_key, x + 9, y + 11, YELLOW);

		switch (GetKey())
		{
			case UP_KEY:
				item_select--;
				if (item_select < 1) item_select = max_selection;
				break;
			case DOWN_KEY:
				item_select++;
				if (item_select > max_selection) item_select = 1;
				break;
			case ENTER_KEY: case RIGHT_KEY:
				switch (item_select)
				{
					case 1:
						GotoXY(x + 7, y + 2);
						CursorVisible(true);
						while(true)
						{
							m_1_left_key = GetKey();
							if (IllegalControlKey(m_1_left_key))
								PrintString(L"非法按键，请重新输入！",x + 7, y + 2, RED);
							else break;
						}
						PrintString(L"                      ",x + 7, y + 2, WHITE);
						CursorVisible(false);
						break;
					case 2:
						GotoXY(x + 7, y + 3);
						CursorVisible(true);
						while(true)
						{
							m_1_right_key = GetKey();
							if (IllegalControlKey(m_1_right_key))
								PrintString(L"非法按键，请重新输入！",x + 7, y + 3, RED);
							else break;
						}
						PrintString(L"                      ",x + 7, y + 3, WHITE);
						CursorVisible(false);
						break;
					case 3:
						GotoXY(x + 7, y + 4);
						CursorVisible(true);
						while(true)
						{
							m_1_down_key = GetKey();
							if (IllegalControlKey(m_1_down_key))
								PrintString(L"非法按键，请重新输入！",x + 7, y + 4, RED);
							else break;
						}
						PrintString(L"                      ",x + 7, y + 4, WHITE);
						CursorVisible(false);
						break;
					case 4:
						GotoXY(x + 7, y + 5);
						CursorVisible(true);
						while(true)
						{
							m_1_rotate_key = GetKey();
							if (IllegalControlKey(m_1_rotate_key))
								PrintString(L"非法按键，请重新输入！",x + 7, y + 5, RED);
							else break;
						}
						PrintString(L"                      ",x + 7, y + 5, WHITE);
						CursorVisible(false);
						break;
					case 5:
						GotoXY(x + 9, y + 6);
						CursorVisible(true);
						while(true)
						{
							m_1_sink_key = GetKey();
							if (IllegalControlKey(m_1_sink_key))
								PrintString(L"非法按键，请重新输入！",x + 9, y + 6, RED);
							else break;
						}
						PrintString(L"                      ",x + 9, y + 6, WHITE);
						CursorVisible(false);
						break;
					case 6:
						GotoXY(x + 7, y + 7);
						CursorVisible(true);
						while (true)
						{
							m_2_left_key = GetKey();
							if (IllegalControlKey(m_2_left_key))
								PrintString(L"非法按键，请重新输入！", x + 7, y + 7, RED);
							else break;
						}
						PrintString(L"                      ", x + 7, y + 7, WHITE);
						CursorVisible(false);
						break;
					case 7:
						GotoXY(x + 7, y + 8);
						CursorVisible(true);
						while (true)
						{
							m_2_right_key = GetKey();
							if (IllegalControlKey(m_2_right_key))
								PrintString(L"非法按键，请重新输入！", x + 7, y + 8, RED);
							else break;
						}
						PrintString(L"                      ", x + 7, y + 8, WHITE);
						CursorVisible(false);
						break;
					case 8:
						GotoXY(x + 7, y + 9);
						CursorVisible(true);
						while (true)
						{
							m_2_down_key = GetKey();
							if (IllegalControlKey(m_2_down_key))
								PrintString(L"非法按键，请重新输入！", x + 7, y + 9, RED);
							else break;
						}
						PrintString(L"                      ", x + 7, y + 9, WHITE);
						CursorVisible(false);
						break;
					case 9:
						GotoXY(x + 7, y + 10);
						CursorVisible(true);
						while (true)
						{
							m_2_rotate_key = GetKey();
							if (IllegalControlKey(m_2_rotate_key))
								PrintString(L"非法按键，请重新输入！", x + 7, y + 10, RED);
							else break;
						}
						PrintString(L"                      ", x + 7, y + 10, WHITE);
						CursorVisible(false);
						break;
					case 10:
						GotoXY(x + 9, y + 11);
						CursorVisible(true);
						while (true)
						{
							m_2_sink_key = GetKey();
							if (IllegalControlKey(m_2_sink_key))
								PrintString(L"非法按键，请重新输入！", x + 9, y + 11, RED);
							else break;
						}
						PrintString(L"                      ", x + 9, y + 11, WHITE);
						CursorVisible(false);
						break;
					case 11:		//选择了“恢复默认设定”
						m_1_left_key = 'A';
						m_1_right_key = 'D';
						m_1_down_key = 'S';
						m_1_rotate_key = 'W';
						m_1_sink_key = SPACE_KEY;
						m_2_left_key = LEFT_KEY;
						m_2_right_key = RIGHT_KEY;
						m_2_down_key = DOWN_KEY;
						m_2_rotate_key = UP_KEY;
						m_2_sink_key = '/';
						break;
				}
				break;
			case QUIT_KEY:
				return;
			default:
				break;
		}
	}
}

inline bool CUI::GetAutoSave() const
{
	return m_atuo_save;
}

inline int CUI::GetDownKey(Player player) const
{
	if (player == PLAYER1) return m_1_down_key;
	else return m_2_down_key;
}

inline bool CUI::KeyIsControlKey(int key) const
{
	return (key == m_1_left_key || key == m_1_right_key || key == m_1_down_key || key == m_1_rotate_key || key == m_1_sink_key ||
				key == m_2_left_key || key == m_2_right_key || key == m_2_down_key || key == m_2_rotate_key || key == m_2_sink_key ||
				key == CHEAT_KEY || key == LEVELUP_KEY || key == PAUSE_KEY ||
				key == PLAYER1_STRIP1_KEY || key == PLAYER1_STRIP2_KEY || key == PLAYER1_STRIP3_KEY ||
				key == PLAYER2_STRIP1_KEY || key == PLAYER2_STRIP2_KEY || key == PLAYER2_STRIP3_KEY);
}

inline bool CUI::DoublePlayer() const
{
	return m_double_player;
}

void CUI::CalculateBoardXAndBlockPosition()
{
	m_board_x = BOARD_X + 10 - m_width / 2;
	//BOARD_X + 10为界面中心位置，为了让游戏界面在宽度变化的时候始终在中心位置，绘制界面的起始位置为中心位置-m_width/2
	m_block1_x = m_width / 4 - 1;
	m_block2_x = m_width / 4 * 3 - 1;
}