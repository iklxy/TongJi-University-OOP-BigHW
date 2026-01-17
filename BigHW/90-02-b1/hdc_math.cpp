/*2453359 李欣洋 信安*/
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cmath>
#include<conio.h>
#include <Windows.h>
using namespace std;

#include "../include/cmd_hdc_tools.h"

/*
	数学曲线名称：玫瑰线（Rose Curve）
	操作方式：输入n可控制玫瑰花瓣的生成数量
	参数方程：r = a·cos(kθ) 或 r = a·sin(kθ)
 */
//定义PI
static const double PI = 3.14159;
//使用的颜色
static const int Rose_red = RGB(255, 0, 127);
static const int Rose_gold = RGB(255, 215, 0);
//原点
const int o_x = 300;
const int o_y = 300;
//玫瑰线的最大半径
const int max_radius = 200;
//构成叶子的点的粗细程度
const int Rose_thickness = 8;
//最远点的粗细程度 这里给出最远点是方便进行观察
const int Rose_thickness_far = 5;

void hdc_draw_Mathematical_curve_2453359()
{
	
	while (1)
	{
		//基本值
		int choice;         //输入叶子的数量
		double init_angle;  //起始角度（第一个叶子最远点的角度）
		double circ_angle;  //相邻叶子之间的角度差

		hdc_cls();
		cct_showstr(2, 1, "<可选择叶子数量的玫瑰线（1-6）>", 7, 0);
		cct_showstr(2, 2, "输入叶子的数量（1-6,输入非法则重输）", 7, 0);
		cct_showstr(2, 3, "说明:（1）当输入为1时，玫瑰线退化为一个圆（2）代码中使用的方程是r = a·sin(kθ)(3)查阅知，正常在输入6片叶子时，得到的图形也应该是3片叶子，但程序为突出叶子数量增加，采取了判断r>=0的做法，可以绘制出6片叶子", 7, 0);
		cct_gotoxy(2, 3);

		//获取用户输入（1-6）
		while (1)
		{
			choice = _getch();
			if (choice >= 49 && choice <= 54)
				break;
		}

		int n = choice - '0';  //转换为数字1-6

		//计算起始角度和相邻叶子角度差
		init_angle = 90.0 / n;
		circ_angle = 360.0 / n;

		hdc_cls();

		//绘制坐标轴
		hdc_line(100, o_y, 500, o_y, 1);  //x轴 缺省为黑色
		hdc_line(o_x, 100, o_x, 500, 1);  //y轴 缺省为黑色
		hdc_point(o_x, o_y, 3,Rose_red);           //原点

		//对于玫瑰线 r = max_radius * sin(n*θ)
		for (int angle = 0; angle <= 360; angle++)
		{
			//绘制当前叶子
			int old_x = INT_MAX, old_y = INT_MAX;
			int x, y;

			double theta = angle * PI / 180.0;
			double r = max_radius * sin(n * theta);  //极坐标半径

			if (r >= 0)  //只绘制r >= 0的部分
			{
				//极坐标转笛卡尔坐标
				x = o_x + (int)(r * cos(theta));
				y = o_y - (int)(r * sin(theta));

				if (x != old_x || y != old_y)
				{
					hdc_point(x, y, Rose_thickness,Rose_red);  //玫红色
					old_x = x;
					old_y = y;
				}
			}
		}
		//突出最远点
		for (int leaf = 0; leaf < n; leaf++)
		{
			double current_angle = init_angle + leaf * circ_angle;  //当前叶子的角度

			//标注每个叶子的最远点
			double theta_max = current_angle * PI / 180.0;
			int x_max = o_x + (int)(max_radius * cos(theta_max));
			int y_max = o_y - (int)(max_radius * sin(theta_max));
			hdc_circle(x_max, y_max, 3, 1, Rose_thickness_far,Rose_gold);  //黄色圆圈标注
		}
		//显示信息
		char title[100];
		sprintf(title, "玫瑰线：%d 叶", n);
		cct_showstr(2, 0, title, 7, 0);
		cct_showstr(2, 1, "按任意键继续或ESC退出", 7, 0);

		int exit_key = _getch();
		if (exit_key == 27)  //ESC键退出
			break;
	}

	hdc_release();
}