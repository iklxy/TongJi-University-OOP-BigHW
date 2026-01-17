/*2453359 信安 李欣洋*/
#include<iostream>
#include <conio.h>
#include"90-01-b1-hanoi.h"
#include"../include/cmd_console_tools.h"
#include"../include/cmd_hdc_tools.h"
using namespace std;
/* ----------------------------------------------------------------------------------

     本文件功能：
	1、放main函数
	2、初始化屏幕
	3、调用菜单函数（hanoi_menu.cpp中）并返回选项
	4、根据选项调用菜单各项对应的执行函数（hanoi_multiple_solutions.cpp中）

     本文件要求：
	1、不允许定义全局变量（含外部全局和静态全局，const及#define不在限制范围内）
	2、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	3、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
const char* hanoi[] =
{
		"1.基本解",
		"2.基本解(步数记录)",
		"3.内部数组显示(横向)",
		"4.内部数组显示(纵向+横向)",
		"5.图形解-预备-画三个圆柱",
		"6.图形解-预备-在起始柱上画n个盘子",
		"7.图形解-预备-第一次移动",
		"8.图形解-自动移动版本",
		"9.图形解-游戏版",
		"0.退出",
		NULL
};
const char* hanoi_choice = "0123456789";

int main()
{
	cout << "请确认当前cmd窗口的大小为40行*120列以上，字体为新宋体/16，按C继续，Q退出" << endl;
	while (1) {
		char ch = _getch();
		if (ch == 'C' || ch == 'c')
			break;
		if (ch == 'Q' || ch == 'q')
			return 0;
	}

	int first_choose = simple_menu(hanoi,hanoi_choice);
	cout << first_choose-'0' << endl << endl;
	cout << endl;
	switch (first_choose-'0')
	{
		case 0:
			cct_cls();
			main();
			break;
		default:
			hanoi_Initialize(first_choose-'0');
	}
	return 0;
}