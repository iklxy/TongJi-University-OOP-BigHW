/*2453359 信安 李欣洋*/
#include<iostream>
#include"90-01-b2-pullze.h"
#include"../include/simple_menu.h"
#include "../include/cmd_console_tools.h"
using namespace std;

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
const char pullze[][80] =
{
		"A.内部数组，原样输出",
		"B.内部数组，生成提示行列并输出",
		"C.内部数组，游戏版",
		"--------------------------------------------------------",
		"D.n*n的框架(无分隔线)，原样输出",
		"E.n*n的框架(无分隔线)，含提示行列",
		"F.n*n的框架(无分隔线)，显示初始状态，鼠标移动可显示坐标",
		"G.cmd图形界面完整版(无分隔线)",
		"--------------------------------------------------------",
		"H.n*n的框架(有分隔线)，原样输出",
		"I.n*n的框架(有分隔线)，含提示行列",
		"J.n*n的框架(有分隔线)，显示初始状态，鼠标移动可显示坐标",
		"K.cmd图形界面完整版(有分隔线)",
		"--------------------------------------------------------",
		"Q.退出",
		""
};
const char* pullze_choice = "ABCDEFGHIJKQabcdefghijkq";

/* 提示：空文件，仅为了提醒你不要忘记首行 && 不要把文件名弄错 */
int main()
{
	char choose = simple_menu(pullze,pullze_choice);
	if (choose == 'a')
	{
		option_a();
	}
	else if (choose == 'b')
	{
		option_b(1,0,0);
	}
	else if (choose == 'c')
	{
		cct_cls();
		option_c(0,0,0);
	}
	else if (choose == 'd')
	{
		option_d();
	}
	else if (choose == 'e')
	{
		option_e();
	}
	else if (choose == 'f')
	{
		option_f();
	}
	else if (choose == 'g')
	{
		option_g();
	}
	else if (choose == 'h')
	{
		option_h();
	}
	else if (choose == 'i')
	{
		option_i();
	}
	else if (choose == 'j')
	{
		option_j();
	}
	else if (choose == 'k')
	{
		option_k();
	}
	return 0;
}
