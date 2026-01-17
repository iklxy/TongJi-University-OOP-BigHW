/*2453359 信安 李欣洋*/
#include<iostream>
#include "../include/cmd_console_tools.h"
#include"90-01-b2-pullze.h"
#include<iomanip>
using namespace std;

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/

/* 提示：空文件，仅为了提醒你不要忘记首行 && 不要把文件名弄错 */
void option_d()
{
	cct_cls();
	int width;
	bool grid[15][15]; // 存储0的分布
	int maxRowHintCount = 0;
	int maxColHintCount = 0;
	cct_cls();
	cout << "请输入区域大小(5/10/15)：";
	cin >> width;
	print_zero(3, 5, width, grid, 0); // 获取0的分布
	cct_cls();
	cct_setfontsize("楷体",30, 0);
	cout << "   ";
	for (int i = 0; i < width; i++)
	{
		cout << " " << char('a' + i);
	}
	cout << endl;
	cout << "  ";
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i <=2*width; i++)
	{
		cout << "═";
	}
	cout << "╗ ";
	cct_setcolor(0, 7);
	cout << endl;
	for (int i = 0; i < width; i++)
	{
		cct_setcolor(0, 7);
		cout << char('A' + i) << " ";
		cct_setcolor(7, 0);
		cout << "║ ";
		for (int k = 0; k < width; k++)
		{
			if (grid[i][k] == true)
			{
				cct_setcolor(1, 0);
				cout << "○";
				cct_setcolor(7, 0);
			}
			else
			{
				cout << "  ";
			}
		}
		cct_setcolor(7, 0);
		cout << "║ " << endl;
	}
	cct_setcolor(0, 7);
	cout << "  ";
	cct_setcolor(7, 0);
	cout << "╚";
	for (int i = 0; i <= 2*width; i++)
	{
		cout << "═";
	}
	cout << "╝ ";
	cct_setcolor(0, 7);
	cout << endl;
	cout << "本小题结束，请输入End继续..." << endl;
	char end = 0;
	cin >> end;
	cct_cls();
	cin.ignore(1024, '\n');
	cct_setfontsize("新宋体", 16,0);
	main();
}
void option_e()
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
	cct_setfontsize("楷体", 30, 0);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╗ ";
	cout << endl;
	//打印列提示数
	for (int i = 0; i < maxColHintCount; i++)
	{
		cct_setcolor(0, 7);
		for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
		{
			cout << " ";
		}
		cct_setcolor(7, 0);
		cout << "║";
		for (int j = 0; j < width; j++)
		{
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout <<setw(2)<< colHints[j].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << " ║ " << endl;
	}
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "╠";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╣ ";
	cout << endl;
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "║";
	for (int i = 0; i < width; i++)
	{
		cout << " " << char('a' + i);
	}
	cout << " ║ " << endl;
	cout << "╔";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╦";
	cout << "══";
	cout << "═╬";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╣ ";
	cout << endl;
	//打印行提示数和生成的0
	for (int i = 0; i < width; i++)
	{
		cout << "║";
		for (int j = 0; j < maxRowHintCount; j++)
		{
			int hintIdx = j - (maxRowHintCount - rowHints[i].count);
			if (hintIdx >= 0)
				cout <<setw(2)<< rowHints[i].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << "║  ";
		cout<< char('A' + i) << "║ ";
		for (int k = 0; k < width; k++)
		{
			if (grid[i][k] == true)
			{
				cct_setcolor(1, 0);
				cout << "○";
				cct_setcolor(7, 0);
			}
			else
			{
				cout << "  ";
			}
		}
		cout << "║ " << endl;
	}
	cout << "╚";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╩";
	cout << "══";
	cout << "═╩";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╝ " << endl;
	cct_setcolor(0, 7);
	cout << endl;
	cout << "本小题结束，请输入End继续..." << endl;
	char end = 0;
	cin >> end;
	cct_cls();
	cin.ignore(1024, '\n');
	cct_setfontsize("新宋体", 16, 0);
	main();
}
void option_f()
{
	int width;
	bool grid[15][15]; // 存储0的分布
	Hint rowHints[15], colHints[15];
	int maxRowHintCount = 0;
	int maxColHintCount = 0;
	cct_setconsoleborder(1000, 1000, 1000, 1000);
	cout << "测试键盘/鼠标左键/右键，按回车退出" << endl;
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
	if (width == 15)
	{
		cct_setfontsize("楷体", 15, 0);
	}
	else
	{
		cct_setfontsize("楷体", 30, 0);
	}
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╗ ";
	cout << endl;
	//打印列提示数
	for (int i = 0; i < maxColHintCount; i++)
	{
		cct_setcolor(0, 7);
		for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
		{
			cout << " ";
		}
		cct_setcolor(7, 0);
		cout << "║";
		for (int j = 0; j < width; j++)
		{
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout <<setw(2)<< colHints[j].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << " ║ " << endl;
	}
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "╠";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╣ ";
	cout << endl;
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "║";
	for (int i = 0; i < width; i++)
	{
		cout << " " << char('a' + i);
	}
	cout << " ║ " << endl;
	cout << "╔";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╦";
	cout << "══";
	cout << "═╬";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╣ ";
	cout << endl;
	//打印行提示数和生成的0
	for (int i = 0; i < width; i++)
	{
		cout << "║";
		for (int j = 0; j < maxRowHintCount; j++)
		{
			int hintIdx = j - (maxRowHintCount - rowHints[i].count);
			if (hintIdx >= 0)
				cout <<setw(2)<< rowHints[i].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << "║  ";
		cout << char('A' + i) << "║ ";
		for (int k = 0; k < width; k++)
		{
			if (grid[i][k] == true)
			{
				cct_setcolor(1, 0);
				cout << "○";
				cct_setcolor(7, 0);
			}
			else
			{
				cout << "  ";
			}
		}
		cout << "║ " << endl;
	}
	cout << "╚";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╩";
	cout << "══";
	cout << "═╩";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╝ " << endl;
	cct_setcolor(0, 7);
	cout << endl;

	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	char end = 0;
	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);	//关闭光标
	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

		if (ret == CCT_MOUSE_EVENT) {
			/* 转到第7行进行打印 */
			cct_gotoxy(0, maxColHintCount+width+10);
			if (Y >= maxColHintCount + 6&&Y<= maxColHintCount + 6+ width-1&&X>=7+ 2*maxRowHintCount&&X<= 7 + 2*maxRowHintCount+width*2-1)
			{
				char xx,yy;
				yy = char('A' + Y - maxColHintCount - 6);
				xx = char('a' + (X - 7 - 2 * maxRowHintCount)/2);
				cout << "[当前光标]" << setw(2) << yy << " 行" << setw(2) << xx << " 列";
			}
			else
			{
				cout << "[当前光标]位置非法 ";
			}
			
			switch (maction) {
				case MOUSE_ONLY_MOVED:
					cout << "移动          " << endl;
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					cout << "按下左键      " << endl;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//双击左键
					cout << "双击左键      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					cout << "按下右键      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
					cout << "双击右键      " << endl;
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
				case MOUSE_LEFTRIGHT_BUTTON_CLICK:		//同时按下左右键
					cout << "同时按下左右键" << endl;
					break;
				case MOUSE_WHEEL_CLICK:
					cout << "按下滚轮      " << endl;
					break;
				case MOUSE_WHEEL_MOVED_UP:
					cout << "滚轮向上移动  " << endl;
					break;
				case MOUSE_WHEEL_MOVED_DOWN:
					cout << "滚轮向下移动  " << endl;
					break;
				case MOUSE_NO_ACTION:
				default:
					cout << "其它操作" << endl;
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* 转到第9行进行打印 */
			cct_gotoxy(0, maxColHintCount + width + 10);
			cout << "[当前按键] : ";

			switch (keycode1) {
				case 13:	
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:
							cout << "上箭头    ";
							break;
						case KB_ARROW_DOWN:
							cout << "下箭头    ";
							break;
						case KB_ARROW_LEFT:
							cout << "左箭头    ";
							break;
						case KB_ARROW_RIGHT:
							cout << "右箭头    ";
							break;
					}
					break;
				default:	//其他键直接打印ASCII值
					if (keycode2)
						cout << "Keycode=" << keycode1 << '/' << keycode2 << endl;
					else
						cout << "Keycode=" << keycode1 << endl;
					break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT）
	}
}
void option_g()
{
	int width;
	bool grid[15][15]; // 存储0的分布
	bool marked_ball[15][15] = { false };//存储鼠标输入的0
	bool marked_fork[15][15] = { false };//存储鼠标输入的x
	Hint rowHints[15], colHints[15];
	int maxRowHintCount = 0;
	int maxColHintCount = 0;
	cct_setconsoleborder(1000, 1000, 1000, 1000);
	cout << "测试键盘/鼠标左键/右键，按回车退出" << endl;
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
	if (width == 15)
	{
		cct_setfontsize("楷体", 15, 0);
	}
	else
	{
		cct_setfontsize("楷体", 30, 0);
	}
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╗ ";
	cout << endl;
	//打印列提示数
	for (int i = 0; i < maxColHintCount; i++)
	{
		cct_setcolor(0, 7);
		for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
		{
			cout << " ";
		}
		cct_setcolor(7, 0);
		cout << "║";
		for (int j = 0; j < width; j++)
		{
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout << setw(2) << colHints[j].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << " ║ " << endl;
	}
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "╠";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╣ ";
	cout << endl;
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "║";
	for (int i = 0; i < width; i++)
	{
		cout << " " << char('a' + i);
	}
	cout << " ║ " << endl;
	cout << "╔";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╦";
	cout << "══";
	cout << "═╬";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╣ ";
	cout << endl;
	//打印行提示数和生成的0
	for (int i = 0; i < width; i++)
	{
		cout << "║";
		for (int j = 0; j < maxRowHintCount; j++)
		{
			int hintIdx = j - (maxRowHintCount - rowHints[i].count);
			if (hintIdx >= 0)
				cout << setw(2) << rowHints[i].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << "║  ";
		cout << char('A' + i) << "║ ";
		for (int k = 0; k < width; k++)
		{
			cout << "  ";
		}
		cout << "║ " << endl;
	}
	cout << "╚";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╩";
	cout << "══";
	cout << "═╩";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╝ " << endl;
	cct_setcolor(0, 7);
	cout << endl;

	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	char end = 0;
	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);	//关闭光标
	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		char xx = 0, yy = 0;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

		if (ret == CCT_MOUSE_EVENT) {
			/* 转到第7行进行打印 */
			cct_gotoxy(0, maxColHintCount + width + 10);
			if (Y >= maxColHintCount + 6 && Y <= maxColHintCount + 6 + width - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 2 - 1)
			{
				yy = char('A' + Y - maxColHintCount - 6);
				xx = char('a' + (X - 7 - 2 * maxRowHintCount) / 2);
				cout << "[当前光标]" << setw(2) << yy << " 行" << setw(2) << xx << " 列";
			}
			else
			{
				cout << "[当前光标]位置非法 ";
			}

			switch (maction) {
				case MOUSE_ONLY_MOVED:
					cout << "移动          " << endl;
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					cout << "按下左键      " << endl;
					if (Y >= maxColHintCount + 6 && Y <= maxColHintCount + 6 + width - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 2 - 1)
					{
						if (xx >= 'a' && xx <= 'o' && yy >= 'A' && yy <= 'O')
						{
							if (marked_ball[xx - 'a'][yy - 'A'] == false)
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(1, 0);
								cout << "○";
								cct_setcolor(0, 7);
								marked_ball[xx - 'a'][yy - 'A'] = true;
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
							else
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(7, 0);
								cout << "  ";
								cct_setcolor(0, 7);
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
						}
					}
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					cout << "按下右键      " << endl;
					if (Y >= maxColHintCount + 6 && Y <= maxColHintCount + 6 + width - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 2 - 1)
					{
						if (marked_fork[xx - 'a'][yy - 'A'] == false)
						{
							cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
							cct_setcolor(12, 0);
							cout << "×";
							cct_setcolor(0, 7);
							marked_fork[xx - 'a'][yy - 'A'] = true;
							marked_ball[xx - 'a'][yy - 'A'] = false;
						}
						else
						{
							cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
							cct_setcolor(7, 0);
							cout << "  ";
							cct_setcolor(0, 7);
							marked_fork[xx - 'a'][yy - 'A'] = false;
						}
					}
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* 转到第9行进行打印 */
			cct_gotoxy(0, maxColHintCount + width + 10);
			cout << "[当前按键] : ";

			switch (keycode1) {
				case 113:
				case 81:
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
				case 122:
				case 90:
					option_g_cheat(width, grid, marked_ball, marked_fork, rowHints, colHints, maxRowHintCount, maxColHintCount);
					break;
				case 89:
				case 121:
					int temp = 1;
					for (int i = 0; i < 15; i++)
					{
						for (int j = 0; j < 15; j++)
						{
							if (grid[j][i] != marked_ball[i][j])
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
					break;
			}
		}
	}
}
void option_g_cheat(
	int& width,
	bool(&grid)[15][15],
	bool(&marked_ball)[15][15],
	bool(&marked_fork)[15][15],
	Hint(&rowHints)[15],
	Hint(&colHints)[15],
	int& maxRowHintCount,
	int& maxColHintCount
)
{
	cct_gotoxy(0, 2);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╗ ";
	cout << endl;
	//打印列提示数
	for (int i = 0; i < maxColHintCount; i++)
	{
		cct_setcolor(0, 7);
		for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
		{
			cout << " ";
		}
		cct_setcolor(7, 0);
		cout << "║";
		for (int j = 0; j < width; j++)
		{
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout << setw(2) << colHints[j].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << " ║ " << endl;
	}
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "╠";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╣ ";
	cout << endl;
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	cct_setcolor(7, 0);
	cout << "║";
	for (int i = 0; i < width; i++)
	{
		cout << " " << char('a' + i);
	}
	cout << " ║ " << endl;
	cout << "╔";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╦";
	cout << "══";
	cout << "═╬";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╣ ";
	cout << endl;
	//打印行提示数和生成的0
	for (int i = 0; i < width; i++)
	{
		cout << "║";
		for (int j = 0; j < maxRowHintCount; j++)
		{
			int hintIdx = j - (maxRowHintCount - rowHints[i].count);
			if (hintIdx >= 0)
				cout << setw(2) << rowHints[i].nums[hintIdx];
			else
				cout << "  ";
		}
		cout << "║  ";
		cout << char('A' + i) << "║ ";
		for (int k = 0; k < width; k++)
		{
			if (grid[i][k] == true)
			{
				cct_setcolor(15, 0);
				cout << "○";
				cct_setcolor(7, 0);
			}
			else
			{
				cout << "  ";
			}
		}
		cout << "║ " << endl;
	}
	cout << "╚";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╩";
	cout << "══";
	cout << "═╩";
	for (int i = 0; i <= 2 * width; i++)
	{
		cout << "═";
	}
	cout << "╝ " << endl;
	cct_setcolor(0, 7);
	cout << endl;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			marked_ball[i][j] = false;
		}
	}

	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	char end = 0;
	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);	//关闭光标
	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		char xx = 0, yy = 0;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

		if (ret == CCT_MOUSE_EVENT) {
			/* 转到第7行进行打印 */
			cct_gotoxy(0, maxColHintCount + width + 10);
			if (Y >= maxColHintCount + 6 && Y <= maxColHintCount + 6 + width - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 2 - 1)
			{
				yy = char('A' + Y - maxColHintCount - 6);
				xx = char('a' + (X - 7 - 2 * maxRowHintCount) / 2);
				cout << "[当前光标]" << setw(2) << yy << " 行" << setw(2) << xx << " 列";
			}
			else
			{
				cout << "[当前光标]位置非法 ";
			}

			switch (maction) {
				case MOUSE_ONLY_MOVED:
					cout << "移动          " << endl;
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					cout << "按下左键      " << endl;
					if (Y >= maxColHintCount + 6 && Y <= maxColHintCount + 6 + width - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 2 - 1)
					{
						if (xx >= 'a' && xx <= 'o' && yy >= 'A' && yy <= 'O')
						{
							if (marked_ball[xx - 'a'][yy - 'A'] == false && grid[yy - 'A'][xx - 'a'] == true)
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(1, 0);
								cout << "○";
								cct_setcolor(0, 7);
								marked_ball[xx - 'a'][yy - 'A'] = true;
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_ball[xx - 'a'][yy - 'A'] == false && grid[yy - 'A'][xx - 'a'] != true)
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(4, 0);
								cout << "○";
								cct_setcolor(0, 7);
								marked_ball[xx - 'a'][yy - 'A'] = true;
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_ball[xx - 'a'][yy - 'A'] == true && grid[yy - 'A'][xx - 'a'] != true)
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(7, 0);
								cout << "  ";
								cct_setcolor(0, 7);
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_ball[xx - 'a'][yy - 'A'] == true && grid[yy - 'A'][xx - 'a'] == true)
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(15, 0);
								cout << "○";
								cct_setcolor(0, 7);
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
						}
					}
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					cout << "按下右键      " << endl;
					if (Y >= maxColHintCount + 6 && Y <= maxColHintCount + 6 + width - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 2 - 1)
					{
						if (xx >= 'a' && xx <= 'o' && yy >= 'A' && yy <= 'O')
						{
							if (marked_fork[xx - 'a'][yy - 'A'] == false)
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(12, 0);
								cout << "×";
								cct_setcolor(0, 7);
								marked_fork[xx - 'a'][yy - 'A'] = true;
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_fork[xx - 'a'][yy - 'A'] == true && grid[yy - 'A'][xx - 'a'] != true)
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(7, 0);
								cout << "  ";
								cct_setcolor(0, 7);
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_fork[xx - 'a'][yy - 'A'] == true && grid[yy - 'A'][xx - 'a'] == true)
							{
								cct_gotoxy((int(xx) - 'a') * 2 + 7 + 2 * maxRowHintCount, Y);
								cct_setcolor(15, 0);
								cout << "○";
								cct_setcolor(0, 7);
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
						}
					}
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* 转到第9行进行打印 */
			cct_gotoxy(0, maxColHintCount + width + 10+1);
			cout << "[当前按键] : ";

			switch (keycode1) {
				case 113:
				case 81:
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
				case 89:
				case 121:
					int temp = 1;
					for (int i = 0; i < width; i++)
					{
						for (int j = 0; j < width; j++)
						{
							if (grid[j][i] != marked_ball[i][j])
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
					break;
			}
		}
	}
}
void option_h()
{
	cct_setconsoleborder(1000, 1000, 1000, 1000);
	int width = inputwidth();
	bool grid[15][15] = { false }; // 存储0的分布
	print_zero(3, 5, width, grid, 0); // 获取0的分布
	cct_setconsoleborder(1000, 1000, 1000, 1000);
	cct_setfontsize("楷体", 15, 0);
	for (int i = 0; i < width; i++)
	{
		cout << setw(8) << char('a'+i);
	}
	cout << endl;
	cout << "   ";
	print_first(width);
	cout << endl;
	for (int i = 1; i < width; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			print_middle1(width,j,i-1);
			cout << endl;
		}
		print_end(width);
		cout << endl;
	}
	for (int j = 0; j < 3; j++)
	{
		print_middle1(width,j,width-1);
		cout << endl;
	}
	print_last(width);
	cout << endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (grid[i][j] == true)
			{
				print_marked(5+8*i, 2+4*j);
			}
		}
	}
	cct_gotoxy(0,5*width);
	char end;
	cout << "本小题结束，请输入End继续..." << endl;
	cin >> end;
	cct_cls();
	cin.ignore(1024, '\n');
	cct_setfontsize("新宋体", 16, 0);
	main();
}
void option_i()
{
	cct_setconsoleborder(1000, 1000, 1000, 1000);
	int width = inputwidth();
	bool grid[15][15] = { false }; // 存储0的分布
	Hint rowHints[15], colHints[15];
	int maxRowHintCount = 0;
	int maxColHintCount = 0;
	print_zero(3, 5, width, grid, 0); // 获取0的分布
	cct_setconsoleborder(1000, 1000, 1000, 1000);
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
	cct_setfontsize("楷体", 15, 0);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	print_first(width);
	cout << endl;
	for (int i = 0; i < maxColHintCount; i++)
	{
		cct_setcolor(0, 7);
		for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
		{
			cout << " ";
		}
		cct_setcolor(7, 0);
		cout << "║";
		for (int j = 0; j < width; j++)
		{
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout <<"    " << colHints[j].nums[hintIdx]<<"  ║";
			else
				cout << "       ║";
		}
		cout << " ";
		cout << endl;
	}
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	print_end_i(width);
	cout << endl;
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	for (int i = 0; i < width; i++)
	{
		cct_setcolor(7, 0);
		if (i == width - 1)
		{
			cout << "║    " << char('a' + i) << "  ║ ";
		}
		else
		{
			cout << "║    " << char('a' + i) << "  ";
		}
	}
	cct_setcolor(0, 7);
	cout << endl;
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i < 2*maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╗";
	cout << "═══";
	print_end_i(width);
	cout << endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			print_middle1_i(width, j, maxRowHintCount, i, rowHints);
			cout << endl;
		}
		print_middle1_i_end(width, 0, maxRowHintCount, 1);
		cout << endl;
	}
	int temp = 0;
	if (maxRowHintCount % 2 == 0)
	{
		temp = maxRowHintCount;
	}
	else if(maxRowHintCount==3)
	{
		temp = maxRowHintCount - 1;
	}
	else if (maxRowHintCount == 5)
	{
		temp = maxRowHintCount + 1;
	}
	if (width == 15 && maxRowHintCount == 5)
	{
		temp = maxRowHintCount - 1;
	}
	else if (width == 15 && maxRowHintCount == 7)
	{
		temp = maxRowHintCount+1;
	}
	if (width == 10)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[j][i] == true)
				{
					print_marked(11 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}
	if (width == 5)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[j][i] == true)
				{
					print_marked(9 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}
	if (width == 15)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[j][i] == true)
				{
					print_marked(13 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}
    cct_gotoxy(0, 6 * width);
	cout << "本小题结束，请输入End继续..." << endl;
	char end = 0;
	cin >> end;
	cct_cls();
	cin.ignore(1024, '\n');
	cct_setfontsize("新宋体", 16, 0);
	main();
}
void option_j()
{
	cct_setconsoleborder(1000, 1000, 1000, 1000);
	int width = inputwidth();
	bool grid[15][15] = { false }; // 存储0的分布
	Hint rowHints[15], colHints[15];
	int maxRowHintCount = 0;
	int maxColHintCount = 0;
	print_zero(3, 5, width, grid, 0); // 获取0的分布
	cct_setconsoleborder(1000, 1000, 1000, 1000);
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
	cct_setfontsize("楷体", 15, 0);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	print_first(width);
	cout << endl;
	for (int i = 0; i < maxColHintCount; i++)
	{
		cct_setcolor(0, 7);
		for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
		{
			cout << " ";
		}
		cct_setcolor(7, 0);
		cout << "║";
		for (int j = 0; j < width; j++)
		{
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout << "    " << colHints[j].nums[hintIdx] << "  ║";
			else
				cout << "       ║";
		}
		cout << " ";
		cout << endl;
	}
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	print_end_i(width);
	cout << endl;
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	for (int i = 0; i < width; i++)
	{
		cct_setcolor(7, 0);
		if (i == width - 1)
		{
			cout << "║    " << char('a' + i) << "  ║ ";
		}
		else
		{
			cout << "║    " << char('a' + i) << "  ";
		}
	}
	cct_setcolor(0, 7);
	cout << endl;
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╗";
	cout << "═══";
	print_end_i(width);
	cout << endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			print_middle1_i(width, j, maxRowHintCount, i, rowHints);
			cout << endl;
		}
		print_middle1_i_end(width, 0, maxRowHintCount, 1);
		cout << endl;
	}
	int temp = 0;
	if (maxRowHintCount % 2 == 0)
	{
		temp = maxRowHintCount;
	}
	else if (maxRowHintCount == 3)
	{
		temp = maxRowHintCount - 1;
	}
	else if (maxRowHintCount == 5)
	{
		temp = maxRowHintCount + 1;
	}
	if (width == 15 && maxRowHintCount == 5)
	{
		temp = maxRowHintCount - 1;
	}
	else if (width == 15 && maxRowHintCount == 7)
	{
		temp = maxRowHintCount + 1;
	}
	if (width == 10)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[i][j] == true)
				{
					print_marked(11 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}
	if (width == 5)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[i][j] == true)
				{
					print_marked(9 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}
	if (width == 15)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[i][j] == true)
				{
					print_marked(13 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}
	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	char end = 0;
	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);	//关闭光标
	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

		if (ret == CCT_MOUSE_EVENT) {
			/* 转到第7行进行打印 */
			cct_gotoxy(0, maxColHintCount + 4 + width * 4);
			if (Y >= maxColHintCount + 4 && Y <= maxColHintCount + 4 + width*4 - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 4 *2 - 1)
			{
				if ((Y - maxColHintCount - 3) % 4 == 0 || (X - 7 - 2 * maxRowHintCount+2) % 8 == 0)
				{
					cout << "[当前光标]位置非法 ";
				}
				else
				{
					char xx, yy;
					yy = char('A' + (Y - maxColHintCount - 4) / 4);
					xx = char('a' + (X - 7 - 2 * maxRowHintCount) / 8);
					cout << "[当前光标]" << setw(2) << yy << " 行" << setw(2) << xx << " 列";
				}	
			}
			else
			{
				cout << "[当前光标]位置非法 ";
			}

			switch (maction) {
				case MOUSE_ONLY_MOVED:
					cout << "移动          " << endl;
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					cout << "按下左键      " << endl;
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//双击左键
					cout << "双击左键      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					cout << "按下右键      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
					cout << "双击右键      " << endl;
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* 转到第9行进行打印 */
			cct_gotoxy(0, maxColHintCount + 4 + width * 4+1);
			switch (keycode1) {
				case 13:
				case 81:
				case 113:
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:
							cout << "上箭头    ";
							break;
						case KB_ARROW_DOWN:
							cout << "下箭头    ";
							break;
						case KB_ARROW_LEFT:
							cout << "左箭头    ";
							break;
						case KB_ARROW_RIGHT:
							cout << "右箭头    ";
							break;
					}
					break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT）
	}
}
void option_k()
{
	cct_setconsoleborder(1000, 1000, 1000, 1000);
	int width = inputwidth();
	bool grid[15][15] = { false }; // 存储0的分布
	bool marked_ball[15][15] = { false };//存储鼠标输入的0
	bool marked_fork[15][15] = { false };//存储鼠标输入的x
	Hint rowHints[15], colHints[15];
	int maxRowHintCount = 0;
	int maxColHintCount = 0;
	print_zero(3, 5, width, grid, 0); // 获取0的分布
	cct_setconsoleborder(1000, 1000, 1000, 1000);
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
	cct_setfontsize("楷体", 15, 0);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	print_first(width);
	cout << endl;
	for (int i = 0; i < maxColHintCount; i++)
	{
		cct_setcolor(0, 7);
		for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
		{
			cout << " ";
		}
		cct_setcolor(7, 0);
		cout << "║";
		for (int j = 0; j < width; j++)
		{
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout << "    " << colHints[j].nums[hintIdx] << "  ║";
			else
				cout << "       ║";
		}
		cout << " ";
		cout << endl;
	}
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	print_end_i(width);
	cout << endl;
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	for (int i = 0; i < width; i++)
	{
		cct_setcolor(7, 0);
		if (i == width - 1)
		{
			cout << "║    " << char('a' + i) << "  ║ ";
		}
		else
		{
			cout << "║    " << char('a' + i) << "  ";
		}
	}
	cct_setcolor(0, 7);
	cout << endl;
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╗";
	cout << "═══";
	print_end_i(width);
	cout << endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			print_middle1_i(width, j, maxRowHintCount, i, rowHints);
			cout << endl;
		}
		print_middle1_i_end(width, 0, maxRowHintCount, 1);
		cout << endl;
	}
	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	char end = 0;
	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);	//关闭光标
	char xx, yy;
	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

		if (ret == CCT_MOUSE_EVENT) {
			/* 转到第7行进行打印 */
			cct_gotoxy(0, maxColHintCount + 4 + width * 4);
			if (Y >= maxColHintCount + 4 && Y <= maxColHintCount + 4 + width * 4 - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 4 * 2 - 1)
			{
				if ((Y - maxColHintCount - 3) % 4 == 0 || (X - 7 - 2 * maxRowHintCount + 2) % 8 == 0)
				{
					cout << "[当前光标]位置非法 ";
				}
				else
				{
					
					yy = char('A' + (Y - maxColHintCount - 4) / 4);
					xx = char('a' + (X - 7 - 2 * maxRowHintCount) / 8);
					cout << "[当前光标]" << setw(2) << yy << " 行" << setw(2) << xx << " 列";
				}
			}
			else
			{
				cout << "[当前光标]位置非法 ";
			}

			switch (maction) {
				case MOUSE_ONLY_MOVED:
					cout << "移动          " << endl;
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					cout << "按下左键      " << endl;
					if (Y >= maxColHintCount + 4 && Y <= maxColHintCount + 4 + width * 4 - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 4 * 2 - 1)
					{
						if ((Y - maxColHintCount - 3) % 4 != 0 || (X - 7 - 2 * maxRowHintCount + 2) % 8 != 0)
						{
							if (marked_ball[xx - 'a'][yy - 'A'] == false)
							{
								print_marked((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_ball[xx - 'a'][yy - 'A'] = true;
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
							else
							{
								print_blank((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
						}
					}
					break;
				case MOUSE_LEFT_BUTTON_DOUBLE_CLICK:	//双击左键
					cout << "双击左键      " << endl;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					cout << "按下右键      " << endl;
					if (Y >= maxColHintCount + 4 && Y <= maxColHintCount + 4 + width * 4 - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 4 * 2 - 1)
					{
						if ((Y - maxColHintCount - 3) % 4 != 0 || (X - 7 - 2 * maxRowHintCount + 2) % 8 != 0)
						{
							if (marked_fork[xx - 'a'][yy - 'A'] == false)
							{
								print_fork((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_fork[xx - 'a'][yy - 'A'] = true;
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
							else
							{
								print_blank((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
						}
					}
					break;
				case MOUSE_RIGHT_BUTTON_DOUBLE_CLICK:	//双击右键
					cout << "双击右键      " << endl;
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			/* 转到第9行进行打印 */
			switch (keycode1) {
				case 113:
				case 81:
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
				case 122:
				case 90:
					option_k_cheat(width, grid, marked_ball, marked_fork, rowHints, colHints, maxRowHintCount, maxColHintCount);
					break;
				case 89:
				case 121:
					int temp = 1;
					for (int i = 0; i < 15; i++)
					{
						for (int j = 0; j < 15; j++)
						{
							if (grid[j][i] != marked_ball[i][j])
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
					break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT）
	}
}
void option_k_cheat(
	int& width,
	bool(&grid)[15][15],
	bool(&marked_ball)[15][15],
	bool(&marked_fork)[15][15],
	Hint(&rowHints)[15],
	Hint(&colHints)[15],
	int& maxRowHintCount,
	int& maxColHintCount)
{
	cct_setfontsize("楷体", 15, 0);
	cct_cls();
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	print_first(width);
	cout << endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			marked_ball[i][j] = false;
		}
	}
	for (int i = 0; i < maxColHintCount; i++)
	{
		cct_setcolor(0, 7);
		for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
		{
			cout << " ";
		}
		cct_setcolor(7, 0);
		cout << "║";
		for (int j = 0; j < width; j++)
		{
			int hintIdx = i - (maxColHintCount - colHints[j].count);
			if (hintIdx >= 0)
				cout << "    " << colHints[j].nums[hintIdx] << "  ║";
			else
				cout << "       ║";
		}
		cout << " ";
		cout << endl;
	}
	cct_setcolor(0, 7);
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	print_end_i(width);
	cout << endl;
	for (int i = 0; i < 2 * maxRowHintCount + 5; i++)
	{
		cout << " ";
	}
	for (int i = 0; i < width; i++)
	{
		cct_setcolor(7, 0);
		if (i == width - 1)
		{
			cout << "║    " << char('a' + i) << "  ║ ";
		}
		else
		{
			cout << "║    " << char('a' + i) << "  ";
		}
	}
	cct_setcolor(0, 7);
	cout << endl;
	cct_setcolor(7, 0);
	cout << "╔";
	for (int i = 0; i < 2 * maxRowHintCount; i++)
	{
		cout << "═";
	}
	cout << "╗";
	cout << "═══";
	print_end_i(width);
	cout << endl;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			print_middle1_i(width, j, maxRowHintCount, i, rowHints);
			cout << endl;
		}
		print_middle1_i_end(width, 0, maxRowHintCount, 1);
		cout << endl;
	}
	int temp = 0;
	if (maxRowHintCount % 2 == 0)
	{
		temp = maxRowHintCount;
	}
	else if (maxRowHintCount == 3)
	{
		temp = maxRowHintCount - 1;
	}
	else if (maxRowHintCount == 5)
	{
		temp = maxRowHintCount + 1;
	}
	if (width == 15 && maxRowHintCount == 5)
	{
		temp = maxRowHintCount - 1;
	}
	else if (width == 15 && maxRowHintCount == 7)
	{
		temp = maxRowHintCount + 1;
	}
	if (width == 10)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[j][i] == true)
				{
					print_marked_1(11 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}

	if (width == 5)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[j][i] == true)
				{
					print_marked_1(9 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}
	if (width == 15)
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (grid[j][i] == true)
				{
					print_marked_1(13 + temp + 8 * i, 4 + maxColHintCount + 4 * j);
				}
			}
		}
	}

	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	char end = 0;
	cct_enable_mouse();
	cct_setcursor(CCT_CURSOR_INVISIBLE);	//关闭光标
	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		char xx = 0, yy = 0;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);

		if (ret == CCT_MOUSE_EVENT) {
			/* 转到第7行进行打印 */
			cct_gotoxy(0, maxColHintCount + 4 + width * 4);
			if (Y >= maxColHintCount + 4 && Y <= maxColHintCount + 4 + width * 4 - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 4 * 2 - 1)
			{
				if ((Y - maxColHintCount - 3) % 4 == 0 || (X - 7 - 2 * maxRowHintCount + 2) % 8 == 0)
				{
					cout << "[当前光标]位置非法 ";
				}
				else
				{

					yy = char('A' + (Y - maxColHintCount - 4) / 4);
					xx = char('a' + (X - 7 - 2 * maxRowHintCount) / 8);
					cout << "[当前光标]" << setw(2) << yy << " 行" << setw(2) << xx << " 列";
				}
			}
			else
			{
				cout << "[当前光标]位置非法 ";
			}

			switch (maction) {
				case MOUSE_ONLY_MOVED:
					cout << "移动          " << endl;
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					cout << "按下左键      " << endl;
					if (Y >= maxColHintCount + 4 && Y <= maxColHintCount + 4 + width * 4 - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 4 * 2 - 1)
					{
						if (xx >= 'a' && xx <= 'o' && yy >= 'A' && yy <= 'O')
						{
							if (marked_ball[xx - 'a'][yy - 'A'] == false && grid[yy - 'A'][xx - 'a'] == true)
							{
								print_marked((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_ball[xx - 'a'][yy - 'A'] = true;
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_ball[xx - 'a'][yy - 'A'] == false && grid[yy - 'A'][xx - 'a'] != true)
							{
								print_marked_2((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_ball[xx - 'a'][yy - 'A'] = true;
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_ball[xx - 'a'][yy - 'A'] == true && grid[yy - 'A'][xx - 'a'] != true)
							{
								print_blank((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_ball[xx - 'a'][yy - 'A'] == true && grid[yy - 'A'][xx - 'a'] == true)
							{
								print_marked_1((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
						}
					}
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					cout << "按下右键      " << endl;
					if (Y >= maxColHintCount + 4 && Y <= maxColHintCount + 4 + width * 4 - 1 && X >= 7 + 2 * maxRowHintCount && X <= 7 + 2 * maxRowHintCount + width * 4 * 2 - 1)
					{
						if (xx >= 'a' && xx <= 'o' && yy >= 'A' && yy <= 'O')
						{
							if (marked_fork[xx - 'a'][yy - 'A'] == false)
							{
								print_fork((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_fork[xx - 'a'][yy - 'A'] = true;
								marked_ball[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_fork[xx - 'a'][yy - 'A'] == true && grid[yy - 'A'][xx - 'a'] != true)
							{
								print_blank((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
							else if (marked_fork[xx - 'a'][yy - 'A'] == true && grid[yy - 'A'][xx - 'a'] == true)
							{
								print_marked_1((int(xx) - 'a') * 8 + 7 + 2 * maxRowHintCount, (int(yy) - 'A') * 4 + 4 + maxColHintCount);
								marked_fork[xx - 'a'][yy - 'A'] = false;
							}
						}
					}
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			cct_gotoxy(0, maxColHintCount + 4 + width * 4+1);
			switch (keycode1) {
				case 113:
				case 81:
					loop = 0;
					cout << "本小题结束，请输入End继续..." << endl;
					cin >> end;
					cct_cls();
					cin.ignore(1024, '\n');
					cct_setfontsize("新宋体", 16, 0);
					main();
					break;
				case 89:
				case 121:
					int temp = 1;
					for (int i = 0; i < width; i++)
					{
						for (int j = 0; j < width; j++)
						{
							if (grid[j][i] != marked_ball[i][j])
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
					break;
			}
		}
	}
}