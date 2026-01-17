/*2453359 李欣洋 信安*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */
#include<cmath>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
//用于获取Windows控制台句柄，之后可以使用hwnd对控制台窗口进行操作
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
/***************************************************************************
  函数名称：fill_triangle
  功    能：填充三角形和矩形 因为矩形可以视作是两个三角形
  输入参数：const int x1		：第1个点的x
            const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
static void fill_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int thickness, const int RGB_value)
{
	//因为不考虑三点发生共线 所以可以直接计算斜率
	// 计算第2、3点到第1点的距离（作为参考）
	int dist_13 = (int)sqrt((double)(x3 - x1) * (x3 - x1) + (double)(y3 - y1) * (y3 - y1));

	// 从第1个点出发，使用相似三角形进行填充
	for (int step = 0; step < dist_13; step++)
	{
		// 计算当前的比例
		double ratio = (step == 0) ? 0 : (double)step / dist_13;

		// 通过相似三角形，计算两个新的点
		int new_x2 = x1 + (int)((x2 - x1) * ratio);
		int new_y2 = y1 + (int)((y2 - y1) * ratio);

		int new_x3 = x1 + (int)((x3 - x1) * ratio);
		int new_y3 = y1 + (int)((y3 - y1) * ratio);

		// 在这两个新点之间画线
		hdc_line(new_x2, new_y2, new_x3, new_y3, thickness, RGB_value);
	}
}

/***************************************************************************
  函数名称：fill_arc
  功    能：填充扇形和圆
  输入参数：const int point_x		：圆心x
			const int point_y		：圆心y
			const int radius		：半径
			const int angle_begin	：起始角度
			const int angle_end		：终止角度
			const int thickness		：线的粗细
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void fill_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end,const int thickness, const int RGB_value)
{
	for (int r = 1; r <= radius; r++)
	{
		int x1 = point_x + (int)(r * sin(angle_begin * PI / 180.0));
		int y1 = point_y - (int)(r * cos(angle_begin * PI / 180.0));

		int x2 = point_x + (int)(r * sin(angle_end * PI / 180.0));
		int y2 = point_y - (int)(r * cos(angle_end * PI / 180.0));

		hdc_arc(point_x, point_y, r, angle_begin, angle_end, thickness, RGB_value);
		
		if ((angle_end - angle_begin) % 360 != 0)
		{
			hdc_line(point_x, point_y, x1, y1, thickness + 2, RGB_value);
			hdc_line(point_x, point_y, x2, y2, thickness + 2, RGB_value);
		}
	}
}

/***************************************************************************
  函数名称：angle_change
  功    能：处理绘制圆弧时的角度问题
  输入参数：angle_begin 和 angle_end的引用
  返 回 值：
  说    明：
***************************************************************************/
void angle_change(int &angle_begin,int &angle_end)
{
	angle_begin = angle_begin % 360;
	angle_end = angle_end % 360;
}

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
            const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;
	//获取DC，在hwnd上进行绘图
	hdc = GetDC(hWnd);

	cct_setcursor(CCT_CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("点阵字体", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		cct_setcursor(CCT_CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
    /* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
            const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
			const int y1：起点y坐标，左上角为(0,0)
			const int x2：终点y坐标，左上角为(0,0)
			const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
            const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：画线段
  输入参数：const int x1		：起点的x
            const int y1		：起点的y
			const int x2		：终点的x
			const int y2		：终点的y
			const int thickness	：线段的粗细（有缺省值）
			const int RGB_value	：线段的颜色（有缺省值）
  返 回 值：
  说    明：本函数可以用基于hdc_point的组合（速度慢）
                  也可以用hdc_base_point/hdc_base_line的组合（速度快）
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	//设置起始点和终止点
	int begin_x = x1, begin_y = y1;
	int end_x = x2, end_y = y2;
	
	hdc_point(begin_x, begin_y, thickness, RGB_value);
	hdc_point(end_x, end_y, thickness, RGB_value);

	int th = thickness;
	if (th < 1)
		th = 1;
	const int th_end = (th <= 1) ? 1 : th / 2;
	int x_1, y_1, x_2, y_2;
	for (int angle = 0; angle <= 180; angle++)
	{
		x_1 = begin_x + (int)(th_end * sin(angle * PI / 180));
		y_1 = begin_y - (int)(th_end * cos(angle * PI / 180));

		x_2 = end_x + (int)(th_end * sin(angle * PI / 180));
		y_2 = end_y - (int)(th_end * cos(angle * PI / 180));
		
		hdc_base_line(x_1, y_1, x_2, y_2);
	}
}

/***************************************************************************
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
            const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	if (!filled)
	{
		hdc_line(x1, y1, x2, y2, thickness, RGB_value);
		hdc_line(x1, y1, x3, y3, thickness, RGB_value);
		hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	}
	else
	{
		fill_triangle(x1, y1, x2, y2, x3, y3, thickness, RGB_value);
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
            const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	// 将角度转换为弧度
	double angle_rad = rotation_angles * PI / 180.0;
	double cos_a = cos(angle_rad);
	double sin_a = sin(angle_rad);

	int x2 = left_up_x + (int)(width * cos_a);
	int y2 = left_up_y + (int)(width * sin_a);

	int x3 = x2 - (int)(high * sin_a);
	int y3 = y2 + (int)(high * cos_a);

	int x4 = left_up_x - (int)(high * sin_a);
	int y4 = left_up_y + (int)(high * cos_a);

	if (!filled)
	{
		hdc_line(left_up_x, left_up_y, x2, y2, thickness, RGB_value);
		hdc_line(x2, y2, x3, y3, thickness, RGB_value);
		hdc_line(x3, y3, x4, y4, thickness, RGB_value);
		hdc_line(x4, y4, left_up_x, left_up_y, thickness, RGB_value);
	}
	else
	{
		fill_triangle(left_up_x, left_up_y, x2, y2, x3, y3, thickness, RGB_value);
		fill_triangle(left_up_x, left_up_y, x4, y4, x3, y3, thickness, RGB_value);
	}
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及边长，画出一个长方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int angle_begin_ = angle_begin;
	int angle_end_ = angle_end;
	angle_change(angle_begin_, angle_end_);

	// 记录上一个点的坐标
	int old_x = INT_MAX, old_y = INT_MAX;
	int x, y;
	//特判
	if (angle_begin_ == angle_end_)
	{
		for (int angle = 0; angle < 360; angle++)
		{
			x = point_x + (int)(radius * sin(angle * PI / 180.0));
			y = point_y - (int)(radius * cos(angle * PI / 180.0));

			if (x != old_x || y != old_y)
			{
				hdc_point(x, y, thickness + 2, RGB_value);
				old_x = x;
				old_y = y;
			}
		}
	}

	// 判断是否需要顺序遍历或反向遍历
	// 当 begin < end 时，直接遍历；当 begin > end 时，可能跨越0°，需要特殊处理
	if (angle_begin_ < angle_end_)
	{
		// 正常情况：从 begin 到 end
		for (int angle = angle_begin_; angle <= angle_end_; angle++)
		{
			x = point_x + (int)(radius * sin(angle * PI / 180.0));
			y = point_y - (int)(radius * cos(angle * PI / 180.0));

			if (x != old_x || y != old_y)
			{
				hdc_point(x, y, thickness+2, RGB_value);
				old_x = x;
				old_y = y;
			}
		}
	}
	else
	{
		// 跨越0°的情况：从 begin 到 360，再从 0 到 end
		for (int angle = angle_begin_; angle <= 360; angle++)
		{
			x = point_x + (int)(radius * sin(angle * PI / 180.0));
			y = point_y - (int)(radius * cos(angle * PI / 180.0));

			if (x != old_x || y != old_y)
			{
				hdc_point(x, y, thickness+2, RGB_value);
				old_x = x;
				old_y = y;
			}
		}

		for (int angle = 0; angle <= angle_end_; angle++)
		{
			x = point_x + (int)(radius * sin(angle * PI / 180.0));
			y = point_y - (int)(radius * cos(angle * PI / 180.0));

			if (x != old_x || y != old_y)
			{
				hdc_point(x, y, thickness+2, RGB_value);
				old_x = x;
				old_y = y;
			}
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	if (filled)
	{
		fill_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);
	}
	else
	{
		hdc_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);

		int x1 = point_x + (int)(radius * sin(angle_begin * PI / 180.0));
		int y1 = point_y - (int)(radius * cos(angle_begin * PI / 180.0));

		int x2 = point_x + (int)(radius * sin(angle_end * PI / 180.0));
		int y2 = point_y - (int)(radius * cos(angle_end * PI / 180.0));
		if ((angle_end - angle_begin) % 360 != 0)
		{
			hdc_line(point_x, point_y, x1, y1, thickness + 2, RGB_value);
			hdc_line(point_x, point_y, x2, y2, thickness + 2, RGB_value);
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 360, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	// 将旋转角度转换为弧度
	double rotation_rad = rotation_angles * PI / 180.0;
	double cos_rot = cos(rotation_rad);
	double sin_rot = sin(rotation_rad);

	int old_x = INT_MAX, old_y = INT_MAX;
	int x, y;

	if (!filled)
	{
		// 绘制椭圆边界
		for (int angle = 0; angle <= 360; angle++)
		{
			// 标准椭圆上的点（未旋转）
			double x_ellipse = radius_a * sin(angle * PI / 180.0);
			double y_ellipse = -radius_b * cos(angle * PI / 180.0);

			// 应用旋转变换
			double x_rotated = x_ellipse * cos_rot - y_ellipse * sin_rot;
			double y_rotated = x_ellipse * sin_rot + y_ellipse * cos_rot;

			// 平移到圆心
			x = point_x + (int)x_rotated;
			y = point_y + (int)y_rotated;

			if (x != old_x || y != old_y)
			{
				hdc_point(x, y, thickness, RGB_value);
				old_x = x;
				old_y = y;
			}
		}
	}
	else
	{
		// 填充椭圆：从中心向外辐射填充
		for (int r = 1; r <= radius_a; r++)
		{
			for (int angle = 0; angle <= 360; angle++)
			{
				// 计算当前半径下的椭圆参数
				double ratio = (double)r / radius_a;
				double current_a = r;
				double current_b = (int)(radius_b * ratio);

				// 标准椭圆上的点
				double x_ellipse = current_a * sin(angle * PI / 180.0);
				double y_ellipse = -current_b * cos(angle * PI / 180.0);

				// 应用旋转变换
				double x_rotated = x_ellipse * cos_rot - y_ellipse * sin_rot;
				double y_rotated = x_ellipse * sin_rot + y_ellipse * cos_rot;

				// 平移到圆心
				x = point_x + (int)x_rotated;
				y = point_y + (int)y_rotated;

				if (x != old_x || y != old_y)
				{
					hdc_point(x, y, thickness, RGB_value);
					old_x = x;
					old_y = y;
				}
			}
		}
	}
}

#endif !HDC_SERIES_BY_TEACHER
