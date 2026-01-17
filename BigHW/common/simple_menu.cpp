/*2453359 信安 李欣洋*/
#include<iostream>
#include <conio.h>
#include"../include/simple_menu.h"
using namespace std;
/* ----------------------------------------------------------------------------------

     本文件功能：
	1、放被 hanoi_main.cpp 调用的菜单函数，要求显示各菜单项，读入正确的选项后返回

     本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、不允许定义静态全局变量（全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
//不同的菜单
/****************************************************************************/
//对于二维字符数组的形式使用了模板函数，因此声明和定义在simple_menu.h头文件中
/***************************************************************************/

//const char* menu[] 一维字符指针数组
int simple_menu(const char* menu[],const char* choice)
{
	for (int i = 0; menu[i] != NULL; i++)
	{
		cout <<menu[i] << endl;
	}
	
	while (1)
	{
		int Choice = GetMenuChoice();
		for (int i = 0; choice[i] != '\0'; i++)
		{
			if (Choice == choice[i])
			{
				return Choice;
			}
		}
	}
}

//const char* menu 一维字符数组
int simple_menu(const char* menu, const char* choice)
{
	cout << menu;

	while (1)
	{
		int Choice = GetMenuChoice();
		for (int i = 0; choice[i] != '\0'; i++)
		{
			if (Choice == choice[i])
			{
				return Choice;
			}
		}
	}
}
//公共函数
int GetMenuChoice()
{
	int ch = _getch();

	// 判断输入的是数字还是字母
	if (ch >= '0' && ch <= '9') 
	{
		// 数字：返回ASCII码
		return ch ;
	}
	else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) 
	{
		return ch;
	}
	else {
		// 其他字符：直接返回原ASCII码
		return ch;
	}
}

//打印分割线
void printSeparator(int length, char ch)
{
	for (int i = 0; i < length; i++)
	{
		cout << ch;
	}
	cout << endl;
}