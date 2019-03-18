#include <conio.h>			//用于_kbhit()和_getch()函数
#include <windows.h>		//用于GetLocalTime()和Sleep()函数
#include"UI.h"
#include"Block.h"

int main()
{
	CursorVisible(false);
	CUI ui;
	int key{ 0 }, key2{ 0 };
	SYSTEMTIME temp_time, current_time;
	GetLocalTime(&temp_time);
	ui.ShowInfo();
	ui.DrawBoard();
	while (true)
	{
		if (ui.BlockLandTest(PLAYER1))		//当前方块1落地时的处理
		{
			ui.DrawCurrentBlock(PLAYER1);
			ui.ClearLine();
			ui.SetNextBlock(PLAYER1);
			ui.ShowInfo();
			if (ui.GameOver(PLAYER1))			//游戏结束时的处理
			{
				if (ui.ShowGameOver())		//显示游戏结束时的界面，如果函数返回true则重新开始游戏
				{
					system("cls");
					ui = CUI();			//重新开始游戏
					ui.ShowInfo();
					ui.DrawBoard();
				}
				else return 0;		//退出程序
			}
			GetLocalTime(&temp_time);
		}
		if (ui.BlockLandTest(PLAYER2) && ui.DoublePlayer())		//当前方块2落地时的处理
		{
			ui.DrawCurrentBlock(PLAYER2);
			ui.ClearLine();
			ui.SetNextBlock(PLAYER2);
			ui.ShowInfo();
			if (ui.GameOver(PLAYER2))			//游戏结束时的处理
			{
				if (ui.ShowGameOver())		//显示游戏结束时的界面，如果函数返回true则重新开始游戏
				{
					system("cls");
					ui = CUI();			//重新开始游戏
					ui.ShowInfo();
					ui.DrawBoard();
				}
				else return 0;		//退出程序
			}
			GetLocalTime(&temp_time);
		}
		ui.DrawCurrentBlock(PLAYER1);
		if (ui.DoublePlayer()) ui.DrawCurrentBlock(PLAYER2);
		while (true)
		{
			GetLocalTime(&current_time);
			if (current_time - temp_time >= ui.GetIntervalTime())		//如果获得的当前时间和之前的时间之差超过了设定的值则执行一次下落
			{
				temp_time = current_time;
				key = ui.GetDownKey(PLAYER1);
				if (ui.DoublePlayer()) key2 = ui.GetDownKey(PLAYER2);
				break;
			}
			if (_kbhit())		//如果有按键被按下
			{
				key= GetKey();
				key2 = 0;
				if (key == QUIT_KEY)
				{
					if (ui.GetAutoSave())
					{
						ui.ClearCurrentBlock(PLAYER1, false);
						ui.ClearCurrentBlock(PLAYER2, false);
						ui.SaveData();		//自动保存
					}
					return 0;		//退出
				}
				if (key == 'O')
				{
					ui.OptionSettings();
					ui.ShowInfo();
					ui.DrawBoard();
				}
				if (key == 'H')
				{
					ui.ShowHelpInfo();
					ui.ShowInfo();
					ui.DrawBoard();
				}
				if (ui.KeyIsControlKey(key))		//只有按了指定的游戏控制键程序才继续
					break;
			}
			Sleep(20);			//延时以降低CPU利用率
		}
		if (key != PAUSE_KEY)
		{
			ui.ClearCurrentBlock(PLAYER1, true);		//在移动或旋转之前必须要先清除当前方块
			ui.ClearCurrentBlock(PLAYER2, true);
		}
		ui.KeyDetect(key);
		ui.KeyDetect(key2);
	}
}