//2453359 李欣洋 信安
#include "student_info.h"
#include<sstream>
#include<fstream>
#include<iostream>
using namespace std;

Student::Student()
{
	cnt_ok = 0;
	cnt_timer_create_fail = 0;
	cnt_pipe_fail = 0;
	cnt_timer_start_fail = 0;
	cnt_timeout = 0;
	cnt_max_output = 0;
	cnt_deadloop = 0;
	cnt_tc_pass = 0;
	exe_exists = 0;
}

bool Student_Info::is_duplicate(const string& id)
{
	for (const auto& s : this->S_Info) 
	{
		if (s.student_num == id) return true;
	}
	return false;
}

int Student_Info::get_student_list(const char* homework_name,config_file_tools& cfg)
{
	string list = "";
	string temp = homework_name;
	string h_name = "[" + temp + "]";
	cfg.item_get_raw(h_name, "name_list", list);

	//名单不从数据库中取
	if (list != "database")
	{
		fstream f_list(list);
		if (!f_list.is_open())
		{
			cout << "错误" << endl;
			return -1;
		}

		string line;
		int count = 0;
		while (getline(f_list, line))
		{
			if (line.empty())
				continue;
			stringstream ss(line);
			Student temp;
			string class_num, student_num, name;
			ss >> class_num >> student_num >> name;

			temp.class_num = class_num;
			temp.student_num = student_num;
			temp.name = name;
			if (!is_duplicate(temp.student_num)) 
			{
				this->S_Info.push_back(temp);
				count++;
			}
		}
		f_list.close();
		return count;
	}
	else
	{

		MYSQL* mysql;
		MYSQL_RES* result;
		MYSQL_ROW  row;

		string dbserver = "";
		string dbuser = "";
		string dbpasswd = "";
		string dbname = "";
		string db_cno_list = "";
		string db_curr_term = "";
		cfg.item_get_raw("[数据库]", "db_host", dbserver);
		cfg.item_get_raw("[数据库]", "db_username", dbuser);
		cfg.item_get_raw("[数据库]", "db_passwd", dbpasswd);
		cfg.item_get_raw("[数据库]", "db_name", dbname);
		cfg.item_get_raw("[数据库]", "db_cno_list", db_cno_list);
		cfg.item_get_raw("[数据库]", "db_curr_term", db_curr_term);

		if ((mysql = mysql_init(NULL)) == NULL) {
			cout << "mysql_init failed" << endl;
			return -1;
		}

		if (mysql_real_connect(mysql, dbserver.c_str(), dbuser.c_str(), dbpasswd.c_str(), dbname.c_str(), 0, NULL, 0) == NULL) {
			cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
			return -1;
		}

		/* 设置字符集，否则读出的字符乱码 */
		mysql_set_character_set(mysql, "gbk");

		string sql = "call proc_hwapp_get_stulist_from_view_student_cno('"
			+ db_curr_term + "','"
			+ db_cno_list + "','sno',NULL)";

		if (mysql_query(mysql, sql.c_str())){
			cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
			return -1;
		}

		/* 将查询结果存储起来，出现错误则返回NULL
			注意：查询结果为NULL，不会返回NULL */
		if ((result = mysql_store_result(mysql)) == NULL) {
			cout << "mysql_store_result failed" << endl;
			return -1;
		}

		int count = 0;
		while ((row = mysql_fetch_row(result))) {
			Student stu;

			// 1. 提取学号 (stu_no -> row[2])
			if (row[1] != nullptr) {
				stu.student_num = row[1];
			}

			// 2. 提取姓名 (stu_name -> row[3])
			if (row[2] != nullptr) {
				stu.name = row[2];
			}

			// 3. 提取课号 (stu_cno -> row[12])
			if (row[10] != nullptr) {
				try {
					// 数据库中可能是字符串 "5000..."，转为 long long
					stu.class_num = row[10];
				}
				catch (...) {
					stu.class_num = "0"; // 转换失败置0
				}
			}
			else {
				stu.class_num = "0";
			}

			// 存入列表
			// 为了防止空数据，稍微检查一下学号或姓名是否为空
			if (!stu.student_num.empty() && !stu.name.empty()) {
				this->S_Info.push_back(stu);
				count++;
			}
		}

		// 8. 清理资源
		mysql_free_result(result);
		mysql_close(mysql);

		cout << "数据库读取完毕，共获取 " << count << " 名学生。" << endl;
		return count;
	}
}