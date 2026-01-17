/*2453359 信安 李欣洋*/
#pragma once
#include"../include/cmd_console_tools.h"
void option_a();
void option_b(int jud,char first,char second);
void option_c(char first,char second,int choose);
void function_c(char& first, char& second,int &choose);
void option_d();
void option_e();
void option_f();
void option_g();
void option_h();
void option_i();
void option_j();
void option_k();
void print_marked(int x, int y);
void print_marked_1(int x, int y);
void print_marked_2(int x, int y);
void print_fork(int x, int y);
void print_blank(int x, int y);
void print_end_i(int width);
void print_middle1_i_end(int width, int jud, int max, int jud2);
void stop(int y);
struct Coord
{
	int row;
	int col;
};
struct Hint
{
	int nums[8];
	int count;
};
void print_middle1_i(int width, int jud, int max, int jud2, Hint(&rowHints)[15]);
void option_g_cheat(
	int& width,
	bool(&grid)[15][15],
	bool(&marked_ball)[15][15],
	bool(&marked_fork)[15][15],
	Hint(&rowHints)[15],
	Hint(&colHints)[15],
	int& maxRowHintCount,
	int& maxColHintCount
);
void option_k_cheat(
	int& width,
	bool(&grid)[15][15],
	bool(&marked_ball)[15][15],
	bool(&marked_fork)[15][15],
	Hint(&rowHints)[15],
	Hint(&colHints)[15],
	int& maxRowHintCount,
	int& maxColHintCount);
int inputwidth();
void print_first(int width);
void print_middle1(int width,int jud,int jud2);
void print_end(int width);
void print_last(int width);
void print_zero(int x, int y, int width, bool grid[15][15], int jud);
int main();
/* 提示：空文件，仅为了提醒你不要忘记首行 && 不要把文件名弄错 */