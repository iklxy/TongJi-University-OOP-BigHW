/*2453359 李欣洋 信安*/
#pragma once
#include<iostream>
using namespace std;
//公共函数
int GetMenuChoice();
void printSeparator(int length, char ch);

//存放不同的菜单函数的声明
int simple_menu(const char* menu[], const char* choice);
int simple_menu(const char* menu, const char* choice);

//const char menu[][] 二维数组形式
template<size_t N,size_t M>
int simple_menu(const char (&menu)[N][M], const char* choice)
{
	for (size_t i = 0; i < N; i++)
	{
		cout << menu[i] << endl;
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

