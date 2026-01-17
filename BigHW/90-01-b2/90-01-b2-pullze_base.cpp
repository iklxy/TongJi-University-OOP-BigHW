/*2453359 信安 李欣洋*/
#include<iostream>
#include"90-01-b2-pullze.h"
#include "../include/cmd_console_tools.h"
#include<ctime>
#include<iomanip>
#include <cstdlib>
using namespace std;

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

/* 提示：空文件，仅为了提醒你不要忘记首行 && 不要把文件名弄错 */
// 判断坐标是否已存在
bool isUsed(Coord* used, int count, int row, int col)
{
	for (int i = 0; i < count; ++i) 
	{
		if (used[i].row == row && used[i].col == col) 
		{
			return true;
		}
	}
	return false;
}

void print_zero(int x,int y,int width, bool grid[15][15],int jud)
{
	for (int i = 0; i < width; ++i)
		for (int j = 0; j < width; ++j)
			grid[i][j] = false;

	const int MIN_X = x, MIN_Y = y;
	const int CELL_WIDTH = 2;
	const int CELL_HEIGHT = 1;
	const int kong_x = 2;
	const int kong_y = 1;
	if (width != 5 && width != 10 && width != 15) {
		return;  // 无效输入直接返回
	}
	srand((unsigned int)(time(0)));
	int zeroCount = 1+(width * width) / 2;
	const int MAX_WIDTH = 15;
	int cellX[MAX_WIDTH];  // 每列的起始X坐标
	int cellY[MAX_WIDTH];  // 每行的起始Y坐标
	// 计算列坐标 (每个单元格宽2字符，每5列后有2字符的额外空格)
	cellX[0] = MIN_X + 1;  // 第一列的起始 X（跳过 "| |"）
	for (int col = 1; col < width; col++)
	{
		if (col % 5 == 0)
		{
			cellX[col] = cellX[col - 1] + CELL_WIDTH + kong_x;
		}
		else
		{
			cellX[col] = cellX[col - 1] + CELL_WIDTH;
		}
	}

	// 计算行坐标 (每行占1行)
	cellY[0] = MIN_Y;
	for (int row = 1; row < width; row++)
	{
		if (row % 5 == 0)
		{
			cellY[row] = cellY[row - 1] + CELL_HEIGHT + kong_y;
		}
		else
		{
			cellY[row] = cellY[row - 1] + CELL_HEIGHT;
		}
	}

	Coord used[MAX_WIDTH * MAX_WIDTH];
	int usedCount = 0;

	while (usedCount < zeroCount) 
	{
		int col = rand() % width;
		int row = rand() % width;

		if (isUsed(used, usedCount, row, col)) 
		{
			continue; 
		}
		used[usedCount].row = row;
		used[usedCount].col = col;
		usedCount++;
		grid[row][col] = true;
		int x = cellX[col];
		int y = cellY[row];
		if (jud == 1)
		{
			cct_showch(x, y, '0', 0, 15, 1);
		}
	}
	if (jud == 1)
	{
		stop(cellY[width - 1] + 3);
	}
}

//选项c
void option_c(char first, char second,int choose)
{
	static int width;
	static bool grid[15][15] = { false }; // 存储0的分布
	static bool marked[15][15] = { false }; // 存储已标记的位置
	static Hint rowHints[15], colHints[15];
	static int maxRowHintCount = 0;
	static int maxColHintCount = 0;
	static bool initialized = true; 
	static int initialized2 = 1;
	bool judgement = true;
	
	while (initialized)
	{
		cct_cls();
		cout << "请输入区域大小(5/10/15)：";
		cin >> width;
		print_zero(3, 5, width, grid, 0); // 获取0的分布
		// 统计每行提示
		for (int row = 0; row < width; ++row) {
			int count = 0;
			rowHints[row].count = 0;
			for (int col = 0; col < width; ++col) {
				if (grid[row][col]) {
					count++;
				}
				else if (count > 0) {
					rowHints[row].nums[rowHints[row].count++] = count;
					count = 0;
				}
			}
			if (count > 0) {
				rowHints[row].nums[rowHints[row].count++] = count;
			}
			if (rowHints[row].count > maxRowHintCount)
			{
				maxRowHintCount = rowHints[row].count;
			}
		}
		// 统计每列提示
		for (int col = 0; col < width; ++col)
		{
			int count = 0;
			colHints[col].count = 0;
			for (int row = 0; row < width; ++row)
			{
				if (grid[row][col])
				{
					count++;
				}
				else if (count > 0)
				{
					colHints[col].nums[colHints[col].count++] = count;
					count = 0;
				}
			}
			if (count > 0)
			{
				colHints[col].nums[colHints[col].count++] = count;
			}
			if (colHints[col].count > maxColHintCount)
			{
				maxColHintCount = colHints[col].count;
			}
		}
		cout << "初始数组:" << endl;
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << "-";
		}
		cout << "+";
		for (int i = 0; i < width / 5; i++)
		{
			cout << "-----------+";
		}
		cout << endl;
		//打印列提示数
		for (int i = 0; i < maxColHintCount; i++)
		{
			for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
			{
				cout << " ";
			}
			cout << "|";
			for (int j = 0; j < width; j++)
			{
				if (j == 5 || j == 10 || j == 15)
					cout << " |";
				int hintIdx = i - (maxColHintCount - colHints[j].count);
				if (hintIdx >= 0)
					cout <<setw(2)<< colHints[j].nums[hintIdx];
				else
					cout << "  ";
			}
			cout << " |" << endl;
		}
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << "-";
		}
		cout << "+";
		for (int i = 0; i < width / 5; i++)
		{
			cout << "-----------+";
		}
		cout << endl;
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << " ";
		}
		cout << "|";
		int huanhang = 0;
		for (int i = 0; i < width; i++)
		{
			cout << " " << char('a' + i);
			huanhang++;
			if (huanhang % 5 == 0)
				cout << " " << "|";
		}
		cout << endl;
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << "-";
		}
		cout << "+";
		for (int i = 0; i < width / 5; i++)
		{
			cout << "-----------+";
		}
		cout << endl;
		//打印行提示数和生成的0
		for (int i = 0; i < width; i++)
		{
			if (i == 5 || i == 10 || i == 15)
			{
				for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
				{
					cout << "-";
				}
				cout << "+";
				for (int i = 0; i < width / 5; i++)
				{
					cout << "-----------+";
				}
				cout << endl;
			}
			for (int j = 0; j < maxRowHintCount; j++)
			{
				int hintIdx = j - (maxRowHintCount - rowHints[i].count);
				if (hintIdx >= 0)
					cout <<setw(2)<< rowHints[i].nums[hintIdx];
				else
					cout << "  ";
			}
			cout << " |" << char('A' + i) << "|";
			for (int i = 0; i < width / 5; i++)
				cout << "           |";
			cout << endl;
		}
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << "-";
		}
		cout << "+";
		for (int i = 0; i < width / 5; i++)
		{
			cout << "-----------+";
		}
		cout << endl;
		initialized = false;
	}
	int jud = 1;
	int jud2 = 0;
	while (jud)
	{
		char no1 = 0, no2 = 0;
		int choose1 = 0;
		function_c(no1, no2, choose1);
		if (no1 == 'y' || first == 'Y')
		{
			int temp = 1;
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 15; j++)
				{
					if (grid[i][j] != marked[i][j])
						temp = 0;
				}
			}
			if (temp == 0)
			{
				cout << endl;
				cout << "提交错误, 可用作弊模式查看" << endl;
			}
			else
			{
				cout << "提交正确" << endl;
				cout << "本小题结束，请输入End继续..." << endl;
				char end = 0;
				cin >> end;
				cct_cls();
				cin.ignore(1024, '\n');
				main();
			}
		}
		if (no1 == 'z' || no1 == 'Z')
		{
			if (jud2 == 1)
			{
				jud2 = 0;
			}
			else if (jud2 == 0)
			{
				jud2 = 1;
			}
		}
		if (jud2 == 0)
		{
			if (marked[no1 - 'A'][no2 - 'a'] == true)
			{
				marked[no1 - 'A'][no2 - 'a'] = false;
			}
			else
			{
				marked[no1 - 'A'][no2 - 'a'] = true;
			}
		}
		for (int i = 0; i < 2 * maxRowHintCount + 2; i++)
		{
			cout << "-";
		}
		cout << "+";
		for (int i = 0; i < width / 5; i++)
		{
			cout << "-----------+";
		}
		cout << endl;
		//打印列提示数
		for (int i = 0; i < maxColHintCount; i++)
		{
			for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
			{
				cout << " ";
			}
			cout << "|";
			for (int j = 0; j < width; j++)
			{
				if (j == 5 || j == 10 || j == 15)
					cout << " |";
				int hintIdx = i - (maxColHintCount - colHints[j].count);
				if (hintIdx >= 0)
					cout << " " << colHints[j].nums[hintIdx];
				else
					cout << "  ";
			}
			cout << " |" << endl;
		}
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << "-";
		}
		cout << "+";
		for (int i = 0; i < width / 5; i++)
		{
			cout << "-----------+";
		}
		cout << endl;
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << " ";
		}
		cout << "|";
		int huanhang = 0;
		for (int i = 0; i < width; i++)
		{
			cout << " " << char('a' + i);
			huanhang++;
			if (huanhang % 5 == 0)
				cout << " " << "|";
		}
		cout << endl;
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << "-";
		}
		cout << "+";
		for (int i = 0; i < width / 5; i++)
		{
			cout << "-----------+";
		}
		cout << endl;
		//打印行提示数和生成的0
		for (int i = 0; i < width; i++)
		{
			if (i == 5 || i == 10 || i == 15)
			{
				for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
				{
					cout << "-";
				}
				cout << "+";
				for (int i = 0; i < width / 5; i++)
				{
					cout << "-----------+";
				}
				cout << endl;
			}
			for (int j = 0; j < maxRowHintCount; j++)
			{
				int hintIdx = j - (maxRowHintCount - rowHints[i].count);
				if (hintIdx >= 0)
					cout << " " << rowHints[i].nums[hintIdx];
				else
					cout << "  ";
			}
			cout << " |" << char('A' + i) << "|";
			for (int k = 0; k < width; k++)
			{
				if (k == 5 || k == 10 || k == 15)
					cout << " |";
				if (jud2 == 0)
				{
					if (marked[i][k] == true)
					{
						cout << " ";
						cct_setcolor(14, 0);
						cout << "0";
						cct_setcolor(0, 7);
					}
					else
					{
						cout << "  ";
					}
				}
				else if (jud2 == 1)
				{
					if (grid[i][k] == true)
					{
						cout << " ";
						cct_setcolor(14, 0);
						cout << "0";
						cct_setcolor(0, 7);
					}
					else if(grid[i][k]==false&&(i!=no1-'A'||k!=no2-'a'))
					{
						cout << "  ";
					}
					else if (grid[i][k] == false && i == no1 - 'A' && k == no2 - 'a')
					{
						cout << " ";
						cct_setcolor(14, 0);
						cout << "X";
						cct_setcolor(0, 7);
					}
				}
			}
			cout << " |" << endl;
		}
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << "-";
		}
		cout << "+";
		for (int i = 0; i < width / 5; i++)
		{
			cout << "-----------+";
		}
		cout << endl;
	}
}
//选项b
void option_b(int jud,char first,char second)
{
	int width;
	bool grid[15][15]; // 存储0的分布
	Hint rowHints[15], colHints[15];
	int maxRowHintCount = 0;
	int maxColHintCount = 0;
	cct_cls();
	cout << "请输入区域大小(5/10/15)：";
	cin >> width;
	print_zero(3, 5, width, grid, 0); // 获取0的分布
	// 统计每行提示
	for (int row = 0; row < width; ++row) {
		int count = 0;
		rowHints[row].count = 0;
		for (int col = 0; col < width; ++col) {
			if (grid[row][col]) {
				count++;
			}
			else if (count > 0) {
				rowHints[row].nums[rowHints[row].count++] = count;
				count = 0;
			}
		}
		if (count > 0) {
			rowHints[row].nums[rowHints[row].count++] = count;
		}
		if (rowHints[row].count > maxRowHintCount)
		{
			maxRowHintCount = rowHints[row].count;
		}
	}
	// 统计每列提示
	for (int col = 0; col < width; ++col)
	{
		int count = 0;
		colHints[col].count = 0;
		for (int row = 0; row < width; ++row)
		{
			if (grid[row][col])
			{
				count++;
			}
			else if (count > 0)
			{
				colHints[col].nums[colHints[col].count++] = count;
				count = 0;
			}
		}
		if (count > 0)
		{
			colHints[col].nums[colHints[col].count++] = count;
		}
		if (colHints[col].count > maxColHintCount)
		{
			maxColHintCount = colHints[col].count;
		}
	}
	cout << "初始数组:" << endl;
	for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
	{
		cout << "-";
	}
	cout << "+";
	for (int i = 0; i < width / 5; i++)
	{
		cout << "-----------+";
	}
	cout << endl;
	//打印列提示数
	for (int i = 0; i < maxColHintCount; i++)
	{
		for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
		{
			cout << " ";
		}
		cout << "|";
		for (int j = 0; j < width; j++)
		{
			if (j == 5 || j == 10 || j == 15)
				cout << " |";
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout <<setw(2)<< colHints[j].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << " |" << endl;
	}
	for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
	{
		cout << "-";
	}
	cout << "+";
	for (int i = 0; i < width / 5; i++)
	{
		cout << "-----------+";
	}
	cout << endl;
	for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
	{
		cout << " ";
	}
	cout << "|";
	int huanhang = 0;
	for (int i = 0; i < width; i++)
	{
		cout << " " << char('a' + i);
		huanhang++;
		if (huanhang % 5 == 0)
			cout << " " << "|";
	}
	cout << endl;
	for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
	{
		cout << "-";
	}
	cout << "+";
	for (int i = 0; i < width / 5; i++)
	{
		cout << "-----------+";
	}
	cout << endl;
	//打印行提示数和生成的0
	for (int i = 0; i < width; i++)
	{
		if (i == 5 || i == 10 || i == 15)
		{
			for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
			{
				cout << "-";
			}
			cout << "+";
			for (int i = 0; i < width / 5; i++)
			{
				cout << "-----------+";
			}
			cout << endl;
		}
		for (int j = 0; j < maxRowHintCount; j++)
		{
			int hintIdx = j - (maxRowHintCount - rowHints[i].count);
			if (hintIdx >= 0)
				cout <<setw(2)<< rowHints[i].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << " |" << char('A' + i) << "|";
		if (jud == 1)
		{
			for (int k = 0; k < width; k++)
			{
				if (k == 5 || k == 10 || k == 15)
					cout << " |";
				if (grid[i][k] == true)
				{
					cout << " 0";
				}
				else
				{
					cout << "  ";
				}
			}
			cout << " |" << endl;
		}
		else
		{
			for (int i = 0; i < width / 5; i++)
			{
				cout << "           |";
			}
			cout << endl;
		}
	}
	for (int i = 0; i < 2 * maxRowHintCount + 3; i++)
	{
		cout << "-";
	}
	cout << "+";
	for (int i = 0; i < width / 5; i++)
	{
		cout << "-----------+";
	}
	cout << endl;
	if (jud == 0)
	{
	}
	if (jud == 1)
	{
		stop(width + 10 + maxColHintCount);
	}
}
//选项a
void option_a()
{
	cct_cls();
	cout << "请输入区域大小(5/10/15) ：";
	int width = 0;
	cin >> width;
	cout << "初始数组:" << endl;
	cout << "+-+";
	for (int i = 0; i < width / 5; i++)
		cout << "-----------+";
	cout << endl;
	cout << "| |";
	int huanhang = 0;
	for (int i = 0; i < width; i++)
	{
		cout << " " << char('a' + i);
		huanhang++;
		if (huanhang % 5 == 0)
			cout << " " << "|";
	}
	cout << endl;
	cout << "+-+";
	for (int i = 0; i < width / 5; i++)
		cout << "-----------+";
	cout << endl;
	for (int i = 0; i < width; i++)
	{
		if (i ==5||i==10||i==15)
		{
			cout << "+-+";
			for (int i = 0; i < width / 5; i++)
				cout << "-----------+";
			cout << endl;
			cout << "|" << char('A' + i) << "|";
			for (int i = 0; i < width / 5; i++)
			{
				cout << "           |";
			}
		}
		else
		{
			cout << "|" << char('A' + i) << "|";
			for (int i = 0; i < width / 5; i++)
			{
				cout << "           |";
			}
		}
		cout << endl;
	}
	cout << "+-+";
	for (int i = 0; i < width / 5; i++)
		cout << "-----------+";
	bool a[15][15] = { 0 };//对于选项a无用的参数
	print_zero(3,5,width,a,1);
}
