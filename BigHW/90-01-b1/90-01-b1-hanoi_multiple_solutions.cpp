/*2453359 信安 李欣洋*/
#include<iostream>
#include<iomanip>
#include<windows.h>
#include <conio.h>
#include"90-01-b1-hanoi.h"
#include"../include/cmd_console_tools.h"
#include"../include/cmd_hdc_tools.h"
using namespace std;

/* ----------------------------------------------------------------------------------

     本文件功能：
	1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

     本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
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

void print_innitialization(int floor, char first);

int topa = 0, topb = 0, topc = 0;//三个栈顶指针
int step_count = 0;//计数器
int arra[10] = { 0 }, arrb[10] = { 0 }, arrc[10] = { 0 };//三个用于储存圆盘的数

void innitialization(int floor, char first,int choose)
{
    if (first == 'A')
    {
        topa = floor;
        for (int i = 0; i < floor; i++)
        {
            arra[i] = floor - i;
        }
        if (choose == 3)
        {
            print_innitialization(floor, first);
        } 
    }
    if (first == 'B')
    {
        topb = floor;
        for (int i = 0; i < floor; i++)
        {
            arrb[i] = floor - i;
        }
        if (choose == 3)
        {
            print_innitialization(floor, first);
        }
    }
    if (first == 'C')
    {
        topc = floor;
        for (int i = 0; i < floor; i++)
        {
            arrc[i] = floor - i;
        }
        if (choose == 3)
        {
            print_innitialization(floor, first);
        }
    }
}

void print_innitialization(int floor, char first)
{
    cout << "初始:                ";
    if (first == 'A')
    {
        cout << "A:";
        for (int i = 0; i < floor; i++)
        {
            cout << setw(2) << arra[i];
        }
        for (int j = 0; j < 10 - floor; j++)
        {
            cout << "  ";
        }
        cout << " B:                     C:                     ";
    }
    if (first == 'B')
    {
        cout << "A:                     B:";
        for (int i = 0; i < floor; i++)
        {
            cout << setw(2) << arrb[i];
        }
        for (int j = 0; j < 10 - floor; j++)
        {
            cout << "  ";
        }
        cout << " C:                     ";
    }
    if (first == 'C')
    {
        cout << "A:                     B:                     C:";
        for (int i = 0; i < floor; i++)
        {
            cout << setw(2) << arrc[i];
        }
        for (int j = 0; j < 10 - floor; j++)
        {
            cout << "  ";
        }
        cout << " ";
    }
    cout << endl;
}
void just_move(char from, char to)
{
    int mid = 0;
    if (from == 'A')
    {
        mid = arra[--topa];
    }
    else if (from == 'B')
    {
        mid = arrb[--topb];
    }
    else if (from == 'C')
    {
        mid = arrc[--topc];
    }

    if (to == 'A')
    {
        arra[topa++] = mid;
    }
    else if (to == 'B')
    {
        arrb[topb++] = mid;
    }
    else if (to == 'C')
    {
        arrc[topc++] = mid;
    }
}
void print_condition_1(int count,int move, char from, char to, char the_first, char the_end,int choose1,int floor,int sleep)
{
    if (choose1 == 3)
    {
        cout << "第" << setw(4) << count << " 步(" << setw(2) << move << "): " << from << "-->" << to << " ";
        cout << "A:";
        for (int i = 0; i < topa; i++)
        {
            cout << setw(2) << arra[i];
        }
        for (int j = 0; j < MAX_LAYER - topa; j++)
        {
            cout << "  ";
        }
        cout << " B:";
        for (int k = 0; k < topb; k++)
        {
            cout << setw(2) << arrb[k];
        }
        for (int l = 0; l < MAX_LAYER - topb; l++)
        {
            cout << "  ";
        }
        cout << " C:";
        for (int m = 0; m < topc; m++)
        {
            cout << setw(2) << arrc[m];
        }
        for (int n = 0; n < MAX_LAYER - topc; n++)
        {
            cout << "  ";
        }
        cout << endl;
    }
    if (choose1 == 8)
    {
        cct_gotoxy(MenuItem8_Start_X, MenuItem8_Start_Y);
        cout << "第" << setw(4) << step_count << " 步" << "(" << setw(2) << "#" << move << ": " << from << "-->" << to << " )";
        cout << "A:";
        for (int i = 0; i < topa; i++)
        {
            cout << setw(2) << arra[i];
        }
        for (int j = 0; j < MAX_LAYER - topa; j++)
        {
            cout << "  ";
        }

        cout << " B:";
        for (int k = 0; k < topb; k++)
        {
            cout << setw(2) << arrb[k];
        }
        for (int l = 0; l < MAX_LAYER - topb; l++)
        {
            cout << "  ";
        }

        cout << " C:";
        for (int m = 0; m < topc; m++)
        {
            cout << setw(2) << arrc[m];
        }
        for (int n = 0; n < MAX_LAYER - topc; n++)
        {
            cout << "  ";
        }
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET);
        cout << "A";
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem8_Start_Y + Underpan_A_Y_OFFSET);
        cout << "B";
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + 2*Underpan_Distance, MenuItem8_Start_Y + Underpan_A_Y_OFFSET);
        cout << "C";
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET - 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 1);
        cout << "=================================================================================";
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topa; i++)
        {
            cout << arra[i];
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topa; i < 10 - topa; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topb; i++)
        {
            cout << arrb[i];
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topb; i < 10 - topb; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topc; i++)
        {
            cout << arrc[i];
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topc; i < 10 - topc; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem8_Start_X, MenuItem8_Start_Y);
        cout << "第" << setw(4) << step_count << " 步(" << setw(2) << move << "): " << from << "-->" << to << " ";
    }
    if (choose1 == 9)
    {
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET);
        cout << "A              B              C";
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET - 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 1);
        cout << "===================================";
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topa; i++)
        {
            cout << arra[i];
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topa; i < 10 - topa; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topb; i++)
        {
            cout << arrb[i];
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topb; i < 10 - topb; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topc; i++)
        {
            cout << arrc[i];
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topc; i < 10 - topc; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem8_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem8_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem8_Start_X, MenuItem8_Start_Y + Underpan_A_Y_OFFSET+1);
        cout << "第" << setw(4) << count << " 步(" << setw(2) << move << "): " << from << "-->" << to << " ";
        cout << "A:";
        for (int i = 0; i < topa; i++)
        {
            cout << setw(2) << arra[i];
        }
        for (int j = 0; j < 10 - topa; j++)
        {
            cout << "  ";
        }
        cout << " B:";
        for (int k = 0; k < topb; k++)
        {
            cout << setw(2) << arrb[k];
        }
        for (int l = 0; l < 10 - topb; l++)
        {
            cout << "  ";
        }
        cout << " C:";
        for (int m = 0; m < topc; m++)
        {
            cout << setw(2) << arrc[m];
        }
        for (int n = 0; n < 10 - topc; n++)
        {
            cout << "  ";
        }
        cout << endl;
    }
    if (choose1 == 4)
    {
        cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
        cout << "第" << setw(4) << step_count << " 步" << "(" << setw(2) << "#" << move << ": " << from << "-->" << to << " )";
        cout << "A:";
        for (int i = 0; i < topa; i++)
        {
            cout << setw(2) << arra[i];
        }
        for (int j = 0; j < 10 - topa; j++)
        {
            cout << "  ";
        }

        cout << " B:";
        for (int k = 0; k < topb; k++)
        {
            cout << setw(2) << arrb[k];
        }
        for (int l = 0; l < 10 - topb; l++)
        {
            cout << "  ";
        }

        cout << " C:";
        for (int m = 0; m < topc; m++)
        {
            cout << setw(2) << arrc[m];
        }
        for (int n = 0; n < 10 - topc; n++)
        {
            cout << "  ";
        }
        if (choose1 == 0)
        {
            while (1)
            {
                int ch = _getch();
                if (ch != '\r')
                {
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
        if (sleep != 0)
        {
            Sleep(sleep);
        }
        else
        {
            while (1)
            {
                char temp = _getch();
                if (temp == '\r')
                {
                    break;
                }
            }
        }
        cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET, MenuItem4_Start_Y + Underpan_A_Y_OFFSET);
        cout << "A              B              C";
        cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET - 2, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 1);
        cout << "===================================";
        cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topa; i++)
        {
            cout << arra[i];
            cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topa; i < 10; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topb; i++)
        {
            cout << arrb[i];
            cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topb; i < 10; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 2);
        for (int i = 0; i < topc; i++)
        {
            cout << arrc[i];
            cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 3 - i);
        }
        for (int i = topc; i < 10; i++)
        {
            putchar(' ');
            cct_gotoxy(MenuItem4_Start_X + Underpan_A_X_OFFSET + Underpan_Distance * 2, MenuItem4_Start_Y + Underpan_A_Y_OFFSET - 4 - i);
        }
        cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
        cout << "第" << setw(4) << step_count << " 步(" << setw(2) << move << "): " << from << "-->" << to << " ";
    }
}

void move_1(char from, char to, char the_first, char the_end, int floor,int choose1,int sleep)
{
    int mid = 0;

    if (from == 'A') {
        mid = arra[--topa];
    }
    else if (from == 'B') {
        mid = arrb[--topb];
    }
    else if (from == 'C') {
        mid = arrc[--topc];
    }

    if (to == 'A') {
        arra[topa++] = mid;
    }
    else if (to == 'B') {
        arrb[topb++] = mid;
    }
    else if (to == 'C') {
        arrc[topc++] = mid;
    }
    print_condition_1(step_count,mid, from, to, the_first, the_end, choose1,floor,sleep);
}

void input(int *_height, char *_first, char *_tmp, char *_last, int *_sleep, int choose)
{
    int floor ,sleep= 0;
    char first, end, tmp = 0;
    while (1)
    {
        cout << "请输入汉诺塔的层数(1-10)" << endl;
        cin >> floor;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        if (floor < 1 || floor>16)
        {
            cin.ignore(1024, '\n');
        }
        if (floor >= 1 && floor <= 16)
        {
            cin.ignore(1024, '\n');
            break;
        }
    }
    while (1)
    {
        cout << "请输入起始柱(A-C)" << endl;
        cin >> first;
        if (first == 'a')
        {
            first = 'A';
        }
        if (first == 'b')
        {
            first = 'B';
        }
        if (first == 'c')
        {
            first = 'C';
        }
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        if (first == 'A' || first == 'B' || first == 'C')
        {
            cin.ignore(1024, '\n');
            break;
        }
        if (first != 'A' && first != 'B' && first != 'C')
        {
            cin.clear();
            cin.ignore(1024, '\n');
        }
    }
    while (1)
    {
        cout << "请输入目标柱(A-C)" << endl;
        cin >> end;
        if (end == 'a')
        {
            end = 'A';
        }
        if (end == 'b')
        {
            end = 'B';
        }
        if (end == 'c')
        {
            end = 'C';
        }
        if (end == first)
        {
            cout << "目标柱" << "(" << end << ")" << "不能与" << "起始柱" << "(" << first << ")相同" << endl;
            cin.ignore(1024, '\n');
            continue;
        }
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        if (end == 'A' || end == 'B' || end == 'C')
        {
            cin.ignore(1024, '\n');
            break;
        }
        if (end != 'A' && end && 'B' && end != 'C')
        {
            cin.clear();
            cin.ignore(1024, '\n');
        }
    }
    if (first != 'A' && end != 'A')
    {
        tmp = 'A';
    }
    if (first != 'B' && end != 'B')
    {
        tmp = 'B';
    }
    if (first != 'C' && end != 'C')
    {
        tmp = 'C';
    }
    *_height = floor;
    *_first = first;
    *_tmp = tmp;
    *_last = end;
}
void function_1(int height, char first, char tmp, char last, int sleep)
{
    cout << setw(2) << height << "# " << first << "-->" << last << endl;
}
void function_2(int height, char first, char tmp, char last, int sleep)
{
    cout << "第"<<setw(5) << step_count << "步:" << setw(3) << height << "# " << first << "-->" << last << endl;
}
void function_3(int height, char first, char tmp, char last, char the_first, char the_last, int sleep, int choose1)
{
    move_1(first, last, the_first, the_last, height, choose1,sleep);
}
void function_4(int height, char first, char tmp, char last,char the_first,char the_last,int sleep,int choose1)
{
    move_1(first, last, the_first, the_last, height,choose1,sleep);
}
void function_5()//用于画柱子
{
    cct_cls();
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    hdc_init(RGB(255, 255, 255), RGB(0, 0, 0), 8 * 120 * 8, 16 * 30 * 16);
    hdc_cls();
    hdc_rectangle(HDC_Start_X, HDC_Start_Y, HDC_Base_Width*24, HDC_Base_High, 0,1,3,HDC_COLOR[11]);
    Sleep(HDC_Init_Delay);
    hdc_rectangle(HDC_Start_X+ HDC_Base_Width * 23+ HDC_Underpan_Distance, HDC_Start_Y, HDC_Base_Width * 24, HDC_Base_High, 0, 1, 1, HDC_COLOR[11]);
    Sleep(HDC_Init_Delay);
    hdc_rectangle(HDC_Start_X+ 2*HDC_Base_Width * 23+2* HDC_Underpan_Distance, HDC_Start_Y, HDC_Base_Width * 24, HDC_Base_High, 0, 1, 1, HDC_COLOR[11]);
    Sleep(HDC_Init_Delay);
    hdc_rectangle(HDC_Start_X + HDC_Base_Width * 12, HDC_Start_Y - 12 * HDC_Base_High, HDC_Base_Width, 12 * HDC_Base_High, 0, 1, 1, HDC_COLOR[11]);
    Sleep(HDC_Init_Delay);
    hdc_rectangle(HDC_Start_X + HDC_Base_Width * 23 + HDC_Underpan_Distance + HDC_Base_Width * 12, HDC_Start_Y - 12 * HDC_Base_High, HDC_Base_Width, 12 * HDC_Base_High, 0, 1, 1, HDC_COLOR[11]);
    Sleep(HDC_Init_Delay);
    hdc_rectangle(HDC_Start_X + 2 * HDC_Base_Width * 23 + 2 * HDC_Underpan_Distance + HDC_Base_Width * 12, HDC_Start_Y - 12 * HDC_Base_High, HDC_Base_Width, 12 * HDC_Base_High, 0, 1, 1, HDC_COLOR[11]);
    Sleep(HDC_Init_Delay);
    cct_gotoxy(Status_Line_X, Status_Line_Y);
}
void function_6(int height,char first,char last)//初始化盘子
{
    function_5();
    if (first == 'A')
    {
        cct_gotoxy(Status_Line_X, Status_Line_Y);
        Sleep(500);
        cout << "从" << first << "移动到" << last << ", 共" << height << "层";
        int j = 1;
        for (int i = height; i > 0; i--)
        {
            hdc_rectangle(HDC_Start_X + HDC_Base_Width * 12-i* HDC_Base_Width, HDC_Start_Y - j * HDC_Base_High+1, (2*i+1)*HDC_Base_Width, HDC_Base_High, 0, 1, 1,HDC_COLOR[i]);
            Sleep(HDC_Init_Delay);
            j++;
        }
    }
    if (first == 'B')
    {
        cct_gotoxy(Status_Line_X, Status_Line_Y);
        Sleep(500);
        cout << "从" << first << "移动到" << last << ", 共" << height << "层";
        int j = 1;
        for (int i = height; i > 0; i--)
        {
            hdc_rectangle(HDC_Start_X + HDC_Base_Width * 35 - i * HDC_Base_Width+ HDC_Underpan_Distance, HDC_Start_Y - j * HDC_Base_High+1, (2 * i + 1) * HDC_Base_Width, HDC_Base_High, 0, 1, 1, HDC_COLOR[i]);
            Sleep(HDC_Init_Delay);
            j++;
        }
    }
    if (first == 'C')
    {
        cct_gotoxy(Status_Line_X, Status_Line_Y);
        Sleep(500);
        cout << "从" << first << "移动到" << last << ", 共" << height << "层";
        int j = 1;
        for (int i = height; i > 0; i--)
        {
            hdc_rectangle(HDC_Start_X + HDC_Base_Width * 58 - i * HDC_Base_Width +2 * HDC_Underpan_Distance, HDC_Start_Y - j * HDC_Base_High+1, (2 * i + 1) * HDC_Base_Width, HDC_Base_High, 0, 1, 1, HDC_COLOR[i]);
            Sleep(HDC_Init_Delay);
            j++;
        }
    }
}
void move_pan(char from, char to,int yanshi)
{
    static int jishi = 0;
    int n;
    int rank_from;
    int rank_pan_from;
    if (from == 'A')
    {
        rank_from = from - 'A';
        rank_pan_from = topa+1;
        n = arra[topa];
    }
    else if (from == 'B')
    {
        rank_from = from - 'A';
        rank_pan_from = topb+1;
        n = arrb[topb];
    }
    else
    {
        rank_from = from - 'A';
        rank_pan_from = topc+1;
        n = arrc[topc];
    }
    int width_base = (1 + 2 * n) * HDC_Base_Width;
    int height_base = HDC_Base_High;

    int rank_to;
    int rank_pan_to;
    if (to == 'A')
    {
        rank_to = to - 'A';
        rank_pan_to = topa - 1;
    }
    else if (to == 'B')
    {
        rank_to = to - 'A';
        rank_pan_to = topb - 1;
    }
    else
    {
        rank_to = to - 'A';
        rank_pan_to = topc - 1;
    }
    int X_pan_from = HDC_Start_X + (HDC_Underpan_Distance + 23 * HDC_Base_Width) * rank_from + (12-n) * HDC_Base_Width;
    int Y_pan_from = HDC_Start_Y - rank_pan_from * HDC_Base_High;

    int X_pan_to = HDC_Start_X + (HDC_Underpan_Distance + 23 * HDC_Base_Width) * rank_to + (12-n)* HDC_Base_Width;
    int Y_pan_to = HDC_Start_Y - (rank_pan_to+1) * HDC_Base_High;

    //向上移动
    for (int i = Y_pan_from; i >= HDC_Top_Y; i -= HDC_Step_Y)
    {
        if (yanshi != 0)
        {
            if (jishi < 7)
            {
                Sleep(yanshi);
            }
            else
            {
                Sleep(0);
            }
        }
        else
        {
            if (jishi < 7)
            {
                while (1)
                {
                    char temp = _getch();
                    if (temp == '\r')
                    {
                        break;
                    }
                }
            }
            else
            {
                Sleep(0);
            }
        }
        hdc_rectangle(X_pan_from, i, width_base, height_base, 0, 1, 1, HDC_COLOR[n]);
        if (i >= HDC_Start_Y - 13 * HDC_Base_High)
        {
            hdc_rectangle(X_pan_from, i + height_base- HDC_Step_Y, n*HDC_Base_Width, HDC_Step_Y, 0, 1, 1, HDC_COLOR[0]);
            hdc_rectangle(X_pan_from + n * HDC_Base_Width, i + height_base- HDC_Step_Y, HDC_Base_Width, HDC_Step_Y, 0, 1, 1, HDC_COLOR[11]);
            hdc_rectangle(X_pan_from + (n + 1) * HDC_Base_Width, i + height_base- HDC_Step_Y, n*HDC_Base_Width, HDC_Step_Y, 0, 1, 1, HDC_COLOR[0]);
        }
        if (i < HDC_Start_Y - 13 * HDC_Base_High)
        {
            hdc_rectangle(X_pan_from, i + height_base, width_base, HDC_Step_Y, 0, 1, 1, HDC_COLOR[0]);
        }
    }

    //左右移动
    //int HDC_Step_X_new = (X_pan_to > X_pan_from) ? HDC_Step_X : -HDC_Step_X;
    if (X_pan_from > X_pan_to)//向左
    {
        for (int i = X_pan_from; i>= X_pan_to; i -= HDC_Step_X)
        {
            if (yanshi != 0)
            {
                if (jishi < 7)
                {
                    Sleep(yanshi);
                }
                else
                {
                    Sleep(0);
                }
            }
            else
            {
                if (jishi < 7)
                {
                    while (1)
                    {
                        char temp = _getch();
                        if (temp == '\r')
                        {
                            break;
                        }
                    }
                }
                else
                {
                    Sleep(0);
                }
            }
            hdc_rectangle(i, HDC_Top_Y, width_base, height_base, 0, 1, 1, HDC_COLOR[n]);
            hdc_rectangle(i+ width_base, HDC_Top_Y, HDC_Step_X, height_base, 0, 1, 1, HDC_COLOR[0]);
        }
    }
    else if(X_pan_from < X_pan_to)
    {
        for (int i = X_pan_from; i<= X_pan_to; i += HDC_Step_X)
        {
            if (yanshi != 0)
            {
                if (jishi < 7)
                {
                    Sleep(yanshi);
                }
                else
                {
                    Sleep(0);
                }
            }
            else
            {
                if (jishi < 7)
                {
                    while (1)
                    {
                        char temp = _getch();
                        if (temp == '\r')
                        {
                            break;
                        }
                    }
                }
                else
                {
                    Sleep(0);
                }
            }
            hdc_rectangle(i, HDC_Top_Y, width_base, height_base, 0, 1, 1, HDC_COLOR[n]);
            hdc_rectangle(i- HDC_Step_X, HDC_Top_Y, HDC_Step_X, height_base, 0, 1, 1, HDC_COLOR[0]);
        }
    }

    //向下移动
    for (int i = HDC_Top_Y; i <= Y_pan_to; i+= HDC_Step_Y)
    {
        if (yanshi != 0)
        {
            if (jishi < 7)
            {
                Sleep(yanshi);
            }
            else
            {
                Sleep(0);
            }
        }
        else
        {
            if (jishi < 7)
            {
                while (1)
                {
                    char temp = _getch();
                    if (temp == '\r')
                    {
                        break;
                    }
                }
            }
            else
            {
                Sleep(0);
            }
        }
        hdc_rectangle(X_pan_to, i, width_base, height_base, 0, 1, 1, HDC_COLOR[n]);
        if (i <= HDC_Start_Y - 12 * HDC_Base_High)
        {
            hdc_rectangle(X_pan_to, i, width_base, HDC_Step_Y, 0, 1, 1, HDC_COLOR[0]);
        }
        if (i > HDC_Start_Y - 13 * HDC_Base_High)
        {
            hdc_rectangle(X_pan_to, i- HDC_Step_Y, n*HDC_Base_Width, HDC_Step_Y, 0, 1, 1, HDC_COLOR[0]);
            hdc_rectangle(X_pan_to + n * HDC_Base_Width, i- HDC_Step_Y, HDC_Base_Width, HDC_Step_Y, 0, 1, 1, HDC_COLOR[11]);
            hdc_rectangle(X_pan_to + (n + 1) * HDC_Base_Width, i- HDC_Step_Y, n*HDC_Base_Width, HDC_Step_Y, 0, 1, 1, HDC_COLOR[0]);
        }
    }
    jishi++;
}
void function_7(int height,char first,char tmp,char last,int yanshi)//第一次移动
{
    if (height % 2 == 0)
    {
        function_6(height, first, last);
        just_move(first, tmp);
        move_pan(first, tmp, yanshi);
    }
    else
    {
        function_6(height, first, last);
        just_move(first, last);
        move_pan(first, last, yanshi);
    }
    
}
void function_8(int height, char first, char tmp, char last, char the_first, char the_last, int sleep, int choose1)
{
    move_1(first, last, the_first, the_last, height, choose1,sleep);
    move_pan(first, last,sleep);
}
void function_9(int height, char first, char tmp, char last, char the_first, char the_last, int sleep, int choose1)
{
    //hdc_init(RGB(255, 255, 255), RGB(0, 0, 0),)
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    Sleep(500);
    cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出) ：";
    cct_gotoxy(Status_Line_X+60, Status_Line_Y);
    putchar(' ');
    putchar(' ');
    cct_gotoxy(Status_Line_X+60, Status_Line_Y);
    char from, to;
    while (1)
    {
        cct_gotoxy(Status_Line_X + 60, Status_Line_Y);
        int rank_from=0, rank_to=0;
        from = _getch();
        cout << from;
        if (from == 'a')
        {
            from = 'A';
        }
        if (from == 'b')
        {
            from = 'B';
        }
        if (from == 'c')
        {
            from = 'C';
        }
        if (from == '\r')
        {
            continue;
        }
        if (from == 'q' || from == 'Q')
        {
            Sleep(500);
            cout << endl;
            cout << "游戏结束,按回车继续";
            while (1)
            {
                char temp = _getch();
                if (temp == '\r')
                {
                    break;
                }
            }
            cct_cls();
            main();
        }
        to = _getch();
        cout << to;
        if (to == 'a')
        {
            to = 'A';
        }
        if (to == 'b')
        {
            to = 'B';
        }
        if (to == 'c')
        {
            to = 'C';
        }
        if (to == '\r')
        {
            continue;
        }
        while (1)
        {
            char tt = _getch();
            if (tt == '\r')
            {
                break;
            }
        }
        if (from == to)
        {
            continue;
        }
        else
        {
            if (from == 'A')
            {
                rank_from = arra[topa - 1];
            }
            else if (from == 'B')
            {
                rank_from = arrb[topb - 1];
            }
            else if (from == 'C')
            {
                rank_from = arrc[topc - 1];
            }
            if (to == 'A')
            {
                rank_to = arra[topa - 1];
            }
            else if (to == 'B')
            {
                rank_to = arrb[topb - 1];
            }
            else if (to == 'C')
            {
                rank_to = arrc[topc - 1];
            }
            if (rank_from == 0)
            {
                Sleep(300);
                cout << "源柱为零!   非法操作";
                Sleep(600); 
                break;
            }
            if (rank_from > rank_to && rank_to!=0)
            {
                Sleep(300);
                cout << "大盘压小盘，非法操作";
                Sleep(600);
                break;
            }
            else
            {
                step_count++;
                move_1(from, to, the_first, the_last, height, choose1,sleep);
                move_pan(from, to,sleep);
                break;
            }
        }
    }
    int jud=0;
    if (the_last == 'A')
    {
        for (int i = 0; i < 10; i++)
        {
            if (arra[i] != 0)
            {
                jud++;
            }
        }
    }
    else if (the_last == 'B')
    {
        for (int i = 0; i < 10; i++)
        {
            if (arrb[i] != 0)
            {
                jud++;
            }
        }
    }
    else if (the_last == 'C')
    {
        for (int i = 0; i < 10; i++)
        {
            if (arrc[i] != 0)
            {
                jud++;
            }
        }
    }
    if (jud == height)
    {
        return;
    }
    else
    {
        function_9(height, first, tmp, last, first, last, sleep, choose1);
    }
}
void function_choose(int height,char first,char tmp,char last,int sleep,int choose)
{
    static int tiaoshi = 1;
    int choose1 = 0;
    switch (choose)
    {
        case 1:
            function_1(height, first, tmp, last, sleep);
            break;
        case 2:
            function_2(height, first, tmp, last, sleep);
            break;
        case 3:
            function_3(height, first, tmp, last, first, last, sleep, choose);
            break;
        case 4:
            if (tiaoshi == 1)
            {
                tiaoshi++;
                cct_cls();
            }
            function_4(height, first, tmp, last, first, last, sleep, choose);
            break;
        case 8:
            function_8(height, first, tmp, last, first, last, sleep, choose);
            break;
    }
}
void hanoi(int height, char first, char tmp, char last, int sleep, int choose,char the_first,char the_end)
{
    if (height == 1)
    {
        step_count++;
        function_choose(height, first, tmp, last, sleep, choose);
    }
    else if (height >= 2)
    {
        hanoi(height - 1, first, last, tmp, sleep, choose,first,last);
        step_count++;
        function_choose(height, first, tmp, last, sleep, choose);
        hanoi(height - 1, tmp, first, last, sleep, choose, first, last);
    }
}
void hanoi_Initialize(int choose)
{
    if (choose != 5)
    {
        int floor, sleep = 0;
        char first, end, tmp = 0;
        input(&floor, &first, &tmp, &end, &sleep, choose);
        if (choose != 5 && choose != 6 && choose != 7&& choose!= 8&& choose!=9 && choose!=4)
        {
            innitialization(floor, first, choose);
            hanoi(floor, first, tmp, end, sleep, choose, first, end);
            Sleep(500);
            cout << endl;
            cout << "游戏结束,按回车继续";
            while (1)
            {
                char temp = _getch();
                if (temp == '\r')
                {
                    break;
                }
            }
            cct_cls();
            main();
        }
        if (choose == 4)
        {
            Sleep(500);
            cout << "请输入移动速度(0-200：0-按回车单步演示 1-200:延时1-200ms)";
            int yanshi = 0;
            cin >> yanshi;
            innitialization(floor, first, choose);
            cct_cls();
            cct_gotoxy(MenuItem4_Start_X, MenuItem4_Start_Y);
            print_innitialization(floor, first);
            if (yanshi != 0)
            {
                Sleep(yanshi);
            }
            else
            {
                while (1)
                {
                    char temp = _getch();
                    if (temp == '\r')
                    {
                        break;
                    }
                }
            }
            hanoi(floor, first, tmp, end, yanshi, choose, first, end);
            Sleep(500);
            cout << endl;
            cout << "游戏结束,按回车继续";
            while (1)
            {
                char temp = _getch();
                if (temp == '\r')
                {
                    break;
                }
            }
            cct_cls();
            main();
        }
        if (choose == 6)
        {
            function_6(floor, first, end);
            Sleep(300);
            cout << endl;
            cout << "游戏结束,按回车继续";
            while (1)
            {
                char temp = _getch();
                if (temp == '\r')
                {
                    break;
                }
            }
            cct_cls();
            main();
        }
        if (choose == 7)
        {
            int yanshi = 0;
            Sleep(300);
            cout << "请输入移动速度(0-10：0-按回车单步演示 1-20:延时1-20ms)" << endl;
            cin >> yanshi;
            innitialization(floor, first, choose);
            function_7(floor, first, tmp, end,yanshi);
            Sleep(300);
            cout << endl;
            cout << "游戏结束,按回车继续";
            while (1)
            {
                char temp = _getch();
                if (temp == '\r')
                {
                    break;
                }
            }
            cct_cls();
            main();
        }
        if (choose == 8)
        {
            int yanshi = 0;
            Sleep(300);
            cout << "请输入移动速度(0-10：0-按回车单步演示 1-20:延时1-20ms)" << endl;
            cin >> yanshi;
            function_6(floor, first, end);
            innitialization(floor, first, choose);
            hanoi(floor, first, tmp, end, yanshi, choose, first, end);
            Sleep(300);
            cout << endl;
            cout << "游戏结束,按回车继续";
            while (1)
            {
                char temp = _getch();
                if (temp == '\r')
                {
                    break;
                }
            }
            cct_cls();
            main();
        }
        if (choose == 9)
        {
            int yanshi = 0;
            Sleep(300);
            cout << "请输入移动速度(0-10：0-按回车单步演示 1-20:延时1-20ms)" << endl;
            cin >> yanshi;
            function_6(floor, first, end);
            innitialization(floor, first, choose);
            function_9(floor, first, tmp, end, first, end, yanshi, choose);
        }
    }
    if (choose == 5)
    {
        function_5();
        Sleep(300);
        cout << endl;
        cout << "游戏结束,按回车继续";
        while (1)
        {
            char temp = _getch();
            if (temp == '\r')
            {
                break;
            }
        }
        cct_cls();
        main();
    }
}