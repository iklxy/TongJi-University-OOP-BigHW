//2453359 李欣洋 信安
#pragma once
#include<vector>
#include<string>
#include "student_info.h"
#include "../include/class_cft.h"
#include <sys/stat.h>
using namespace std;

class exe_path
{
private:
	vector<string>Student_exe_path;//需要进行拼接的路径
	string demo_exe_path;//直接指定的demo exe的完整路径
	string signle_exe_dirname;//指定signle条件下存放所有学生exe文件的目录名
	string multi_exe_main_dirname;//指定multi条件下的根目录
	string multi_exe_sub_dirname;//指定multi条件下的子目录
	string stu_exe_name;//指定学生的exe文件名
	string pipe_get_input_data_exe_name;//指定get_input_data的全路径
	string pipe_data_file;//指定get_input_data所用数据路径
	string redirection_data_dirname;//指定重定向输入文件的路径

public:
	void make_path(int count, vector<Student>& s,config_file_tools&cfg,const char* homework_name);
	vector<string>& get_path() { return Student_exe_path; }
	string get_pipe_get_input() { return this->pipe_get_input_data_exe_name; }
	string get_pipe_data_file() { return this->pipe_data_file; }
	string get_stu_exe_name() { return this->stu_exe_name; }
	string get_redirection_data_dirname() { return this->redirection_data_dirname; }
	string get_demo_path() { return demo_exe_path; }
};
