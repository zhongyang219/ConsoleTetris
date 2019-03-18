#pragma once
#include <windows.h>

//获取要保存的ini文件的路径
/* void GetIniPath(char* path)
{
	GetModuleFileName(NULL, path, 256);	//获得当前exe文件的绝对路径
	char temp[12] {"\\config.ini"};
	int idx{255};
	while (path[idx] != '\\')		//查找路径最后一个反斜杠的位置
		idx--;
	for(int i{ idx }, j{ 0 }; j<12; i++, j++)		//从最后一个反斜杠处加上"\\config.ini"
		path[i] = temp[j];
}
 */
//向ini文件写入一个int数据
inline void WriteIntToIni(char* AppName, char* KeyName, int value)
{
	char buff[11];
	_itoa_s(value, buff, 10);
	WritePrivateProfileStringA(AppName,KeyName,buff, "./config.ini");
}

//从ini文件读取一个int数据
inline int GetIntFromIni(char* AppName, char* KeyName, int Default)
{
	return GetPrivateProfileIntA(AppName, KeyName, Default, "./config.ini");
}