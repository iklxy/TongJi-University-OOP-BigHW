//2453359 李欣洋 信安
#pragma once
#include<string>
#include<vector>
#include "../include_mariadb_x86/mysql/mysql.h"
#include "../include/class_cft.h"
using namespace std;

struct Student
{
public:
	string  class_num;//课号
	string student_num;//学号
	string name;//姓名

    int cnt_ok;                 // 正确运行 (CheckExec_Errno::ok)
    int cnt_timer_create_fail;  // 定时器创建失败
    int cnt_pipe_fail;          // 管道方式打开失败
    int cnt_timer_start_fail;   // 启动定时器失败
    int cnt_timeout;            // 超时
    int cnt_max_output;         // 超过输出上限
    int cnt_deadloop;           // 死循环 (Killed by callback)

    int cnt_tc_pass;            // TC通过总数

    bool exe_exists;//代表拼接出的路径是否存在
    // 存放 0 或 1，索引对应测试点序号
    vector<int> item_results;
	Student();
};

class Student_Info
{
private:
	vector<Student>S_Info;
    bool is_duplicate(const string& id);//辅助去重函数
public:
	int get_student_list(const char* homework_name,config_file_tools& cfg);
    vector<Student>& get_list_writable() { return S_Info; }//用于修改学生内部存储的信息
	const vector<Student>& get_list() const { return S_Info; }//只有R权限，用于只读
};