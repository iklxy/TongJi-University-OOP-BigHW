/*2453359 李欣洋 信安*/
#pragma once
#include <string>
#include <sstream>
using namespace std;

class txt_compare
{
private:
	string filename1;
	string filename2;
	string trim_type;//指定每行忽略空字符的方式
	string display_type;//指定显示信息的方式
	int line_skip;//指定文件的前s行不进行比较
	int line_offset;//指定文件错位的行数
	int line_max_diffnum;//指定文件错位的行数 表示满m个不同行则结束运行
	int line_max_linenum;//指定文件比较的最大行数 表示满m行则结束比较
	bool ignore_blank;//忽略文件trim之后的所有空行
	bool CR_CRLF_not_equal;//不忽略Linux和Windows下的换行差异
	bool debug;
	ostringstream result_buffer;//存储最后的输出结果
	int line_maxlen;//差异行之中的最长长度
	bool is_equal;//两个文件是否相同的标志位 方便在result函数中进行打印输出
	int diff_count;//维护一个int型变量用于记录两个文件有差异的行总数
	string trim(const string& line);//公共删除空格的函数
	bool get_next_line(ifstream& in, string& line_out, int& line_count);//读取单行字符串的函数
	bool get_next_line(istream& in, string& line_out, int& line_count);
public:
	//构造函数
	txt_compare(string f1, string f2, string trim_type, string display_type, int line_skip,
		int line_offset, int line_max_diffnum, int line_max_linenum, bool ignore_blank,
		bool CR_CRLF_not_equal, bool debug);
	//不涉及指针操作，析构函数系统自己调用
	
	//compare
	void compare();

	//重载compare用来支持exe_check
	int compare(istream& stream1, istream& stream2, bool silent = false);
	//result
	void result();
	bool get_is_equal() { return this->is_equal; }
};
// //打印函数
void printGroup(const string& all_content, int group_size, bool is_detailed, bool is_ignore_blank);

string get_hex_output(const string& data);

