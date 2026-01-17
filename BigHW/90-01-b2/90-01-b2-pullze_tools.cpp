/*2453359 信安 李欣洋*/
#include<iostream>
#include<conio.h>
#include<iomanip>
#include"90-01-b2-pullze.h"
#include "../include/cmd_console_tools.h"
using namespace std;

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

/* 提示：空文件，仅为了提醒你不要忘记首行 && 不要把文件名弄错 */
//H I J K功能的边框打印函数
void print_middle_i(int width)
{
	cct_setcolor(7, 0);
	cout << "╠";
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << "═";
		}
		cout << "══";
	}
	cout << "═╣ ";
	cct_setcolor(0, 7);
}
void print_marked(int x, int y)
{
	cct_setcolor(1, 0);
	cct_gotoxy(x, y);
	cout << "╔═══╗ ";
	cct_gotoxy(x, y + 1);
	cout << "║ ○║ ";
	cct_gotoxy(x, y + 2);
	cout << "╚═══╝ ";
	cct_setcolor(0, 7);
}
void print_marked_1(int x, int y)
{
	cct_setcolor(15, 0);
	cct_gotoxy(x, y);
	cout << "╔═══╗ ";
	cct_gotoxy(x, y + 1);
	cout << "║ ○║ ";
	cct_gotoxy(x, y + 2);
	cout << "╚═══╝ ";
	cct_setcolor(0, 7);
}
void print_marked_2(int x, int y)
{
	cct_setcolor(4, 0);
	cct_gotoxy(x, y);
	cout << "╔═══╗ ";
	cct_gotoxy(x, y + 1);
	cout << "║ ○║ ";
	cct_gotoxy(x, y + 2);
	cout << "╚═══╝ ";
	cct_setcolor(0, 7);
}
void print_fork(int x, int y)
{
	cct_setcolor(4, 0);
	cct_gotoxy(x, y);
	cout << "╔═══╗ ";
	cct_gotoxy(x, y + 1);
	cout << "║ ×║ ";
	cct_gotoxy(x, y + 2);
	cout << "╚═══╝ ";
	cct_setcolor(0, 7);
}
void print_blank(int x, int y)
{
	cct_setcolor(7, 0);
	cct_gotoxy(x, y);
	cout << "      ";
	cct_gotoxy(x, y + 1);
	cout << "      ";
	cct_gotoxy(x, y + 2);
	cout << "      ";
	cct_setcolor(0, 7);
}
void print_first(int width)//H	用
{
	//cout << "   ";
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << "═";
		}
		cout << "═╗";
	}
	cout << " ";
	cct_setcolor(0, 7);
}
void print_middle1(int width,int jud,int jud2)
{
	if (jud == 1)
	{
		cout << "  "<<char('A'+jud2);
	}
	else
	{
		cout << "   ";
	}
	cct_setcolor(7, 0);
	cout << "║";
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			cout << " ";
		}
		cout << "║";
	}
	cout << " ";
	cct_setcolor(0, 7);
}
void print_middle1_i(int width,int jud,int max,int jud2, Hint(&rowHints)[15])
{
	cct_setcolor(7, 0);
	cout << "║";
	if (jud == 1)
	{
		for (int j = 0; j < max; j++)
		{
			int hintIdx = j - (max - rowHints[jud2].count);
			if (hintIdx >= 0)
				cout << setw(2) << rowHints[jud2].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << "║";
		cout << setw(3) << char('A' + jud2);
		cout << "║";
	}
	else
	{
		for (int i = 0; i <=2*max-2; i++)
		{
			cout << " ";
		}
		cout << " ║   ║";
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			cout << " ";
		}
		cout << "║";
	}
	cout << " ";
	cct_setcolor(0, 7);
}
void print_middle1_i_end(int width, int jud, int max, int jud2)
{
	cct_setcolor(7, 0);
	cout << "║";
	for (int i = 0; i <= 2 * max - 2; i++)
	{
		cout << " ";
	}
	cout << " ║   ";
	cout << "╠";
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << "═";
		}
		cout << "═╣";
	}
	cout << " ";
	cct_setcolor(0, 7);
}
void print_end_i(int width)
{
	cct_setcolor(7, 0);
	cout << "╠";
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << "═";
		}
		cout << "═╣";
	}
	cout << " ";
	cct_setcolor(0, 7);
}
void print_end(int width)
{
	cout << "   ";
	cct_setcolor(7, 0);
	cout << "╠";
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << "═";
		}
		cout << "═╣";
	}
	cout << " ";
	cct_setcolor(0, 7);
}
void print_last(int width)
{
	cout << "   ";
	cct_setcolor(7, 0);
	cout << "╚";
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << "═";
		}
		cout << "═╝";
	}
	cout << " ";
	cct_setcolor(0, 7);
}
//输入宽度函数
int inputwidth()
{
	cct_cls();
	int width;
	cout << "请输入区域大小(5/10/15) ：";
	cin >> width;
	return width;
}
//选项c中的选项函数
void function_c(char& first, char& second,int &choose)
{
	while (1)
	{
		cout << endl << endl;
		cout << "命令形式：Aa=等价于图形游戏中鼠标左键选择Aa位(区分大小写)" << endl;
		cout << "             不需要支持图形界面的右键打叉，再次输入Aa相当于清除" << endl;
		cout << "          X/x=退出(新行仅有X/x，不分大小写)" << endl;
		cout << "          Y/y=提交(新行仅有Y/y，不分大小写)" << endl;
		cout << "          Z/z=作弊(新行仅有Z/z，不分大小写)" << endl;
		cout << "          Ctrl+c 强制退出" << endl;
		cout << "请输入 :";
		first=_getch();
		cout << first;
		if (first == 'x' || first == 'X')
		{
			cout << endl;
			cout << "本小题结束，请输入End继续..." << endl;
			char end = 0;
			cin >> end;
			cct_cls();
			cin.ignore(1024, '\n');
			main();
		}
		if (first == 'y' || first == 'Y')
		{
			break;
		}
		if (first == 'z' || first == 'Z')
		{
			cout << endl;
			choose = 1;
			break;
		}
		second = _getch();
		cout << second << endl;
		putchar('\b');
		putchar('\b');
		putchar('  ');
		while (1)
		{
			char stop = _getch();
			if (stop == '\r')
				break;
		}
		if ((int)first >= 65 && (int)first <= 90 && (int)second >= 97 && (int)second <= 122)
			break;
		else
			cout << "输入错误" << endl;
	}
}
//结束函数
void stop(int y)
{
	cct_gotoxy(0, y);
	cout << "本小题结束，请输入End继续..." << endl;
	char end = 0;
	cin >> end;
	cct_cls();
	cin.ignore(1024, '\n');
	main();
}
