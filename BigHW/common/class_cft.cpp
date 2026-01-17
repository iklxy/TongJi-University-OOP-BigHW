/*2453359 李欣洋 信安*/ 
#include <iostream>
/* 添加自己需要的头文件，注意限制 */
#include <fstream>
#include <string>
#include <sstream>
#include<iomanip>
#include<sys/stat.h>
#include "../include/class_cft.h"
using namespace std;

/* 给出各种自定义函数的实现（已给出的内容不全） */
//trim函数的实现，用于去除字符串首尾的Tab和空格
string config_file_tools::trim(const string& str)
{
	if (str == "")
		return "";//如果是空串 返回空串
	size_t first = str.find_first_not_of(" \t");//查找str中第一个不为空格或Tab的字符的位置
	if (first == string::npos)
		return "";//说明全是空格或者Tab
	size_t last = str.find_last_not_of(" \t");//倒序查找str中第一个不为空格或Tab的字符的位置
	//生成字串
	return str.substr(first, (last - first + 1));
}

//parse_line_content函数
string config_file_tools::parse_line_content(const string& line, bool& is_too_long)
{
	is_too_long = false;//初始置为false，代表长度未超过1024
	
	//判断是否过长
	if (line.length() > MAX_LINE)
	{
		is_too_long = true;
		return "";
	}

	//处理单行内容
	size_t comment_pos = string::npos;//注释符的位置

	//查找;
	size_t pos_semicolon = line.find(';');
	if (pos_semicolon != string::npos)
	{
		comment_pos = pos_semicolon;
	}

	//查找#
	size_t pos_hash = line.find('#');
	if (pos_hash != string::npos)
	{
		if (comment_pos == string::npos || pos_hash < comment_pos) 
		{
			comment_pos = pos_hash;
		}
	}

	//查找//
	size_t pos_double_slash = line.find("//");
	if (pos_double_slash != string::npos) 
	{
		if (comment_pos == string::npos || pos_double_slash < comment_pos) 
		{
			comment_pos = pos_double_slash;
		}
	}

	string effective_content;
	if (comment_pos != string::npos)//注释标记在字符串尾前
		effective_content = line.substr(0, comment_pos);
	else//无注释标记
		effective_content = line;
	//返回最终的字符串
	return trim(effective_content);
}

//字符串转小写函数
string config_file_tools::to_lower_str(const string& str) 
{
	string lower_str = str;

	for (char& c : lower_str) 
	{
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	}

	return lower_str;
}

//判断字符串是否相等 case_sensitive用于选择是否大小写敏感
bool config_file_tools::is_string_equal(const string& s1, const string& s2, bool case_sensitive) 
{
	if (case_sensitive) 
	{
		return s1 == s2;
	}
	else 
	{
		return to_lower_str(s1) == to_lower_str(s2);
	}
}

//查找组名函数
int config_file_tools::find_group_index(const string& gname) 
{
	for (size_t i = 0; i < groups.size(); ++i)
	{
		if (is_string_equal(groups[i].name, gname, false)) 
		{
			return (int)i;
		}
	}
	return -1;
}

//查找key和value的辅助函数
string config_file_tools::find_item_value(const string& group_name, const string& item_name, const bool g_cs, const bool i_cs)
{
	int group_idx = -1;//组名的位置

	for (size_t i = 0; i < this->groups.size(); i++)
	{
		if (is_string_equal(groups[i].name, group_name, g_cs))
		{
			group_idx = i;
			break;
		}
	}

	if (group_idx == -1)
		return "";//说明没有找到

	const Group& current_group = groups[group_idx];

	for (Item item : current_group.items)
	{
		if (!item.has_separator)
			continue;

		if (is_string_equal(item.key, item_name, i_cs))
			return item.value;
	}
	return "";
}
//查找辅助函数
bool config_file_tools::item_exists_in_group(const string& group_name, const string& item_name,const bool g_cs, const bool i_cs)
{
	// 1. 查找组
	int group_idx = -1;
	for (size_t i = 0; i < groups.size(); ++i) {
		if (is_string_equal(groups[i].name, group_name, g_cs)) {
			group_idx = (int)i;
			break;
		}
	}

	if (group_idx == -1) {
		return false; // 组名不存在
	}

	// 2. 在找到的组中查找项
	const Group& current_group = groups[group_idx];

	for (const auto& item : current_group.items) {
		// 查找必须是有效的键值对 (必须包含分隔符)
		if (!item.has_separator) {
			continue;
		}

		// 匹配项名
		if (is_string_equal(item.key, item_name, i_cs)) {
			return true; // 找到键名，返回存在
		}
	}

	return false; // 项名不存在
}

//递归辅助函数
void config_file_tools::resolve_include(int current_idx, vector<int>& status)
{
	// 如果已经处理过（2），直接返回
	if (status[current_idx] == 2) return;

	// 标记为正在处理（1），防止循环引用死循环
	status[current_idx] = 1;

	string include_group_name = "";
	Group& curr_g = this->groups[current_idx];

	for (const auto& item : curr_g.items) 
	{
		if (item.has_separator && is_string_equal(item.key, "include", false)) 
		{
			include_group_name = item.value;
			break; // 找到即止
		}
	}

	if (include_group_name != "") 
	{
		// 查找被 include 的组索引
		int parent_idx = find_group_index(include_group_name);
		if (parent_idx == -1 && include_group_name.length() > 0 && include_group_name[0] != '[') 
		{
			parent_idx = find_group_index("[" + include_group_name + "]");
		}

		if (parent_idx != -1 && parent_idx != current_idx) 
		{

			resolve_include(parent_idx, status);

			//把父组的所有配置项，追加到当前组的末尾
			Group& parent_g = this->groups[parent_idx];
			for (const auto& p_item : parent_g.items) 
			{
				// 跳过 include 项本身，不需复制
				if (is_string_equal(p_item.key, "include", false)) continue;

				// 将父组的项加入当前组
				curr_g.items.push_back(p_item);
			}
		}
	}

	// 标记为已完成
	status[current_idx] = 2;
}

//打印函数
static bool key_exists_helper(const vector<string>& existing_keys, const string& key_to_find)
{
	for (size_t i = 0; i < existing_keys.size(); i++) {
		if (existing_keys[i] == key_to_find) return true;
	}
	return false;
}

// =========================================================================
// [新增] 静态辅助函数：打印单个键值对 (含格式化和目录补全逻辑)
// =========================================================================
static void print_kv_helper(const vector<Item>& items, const string& key, int max_len, bool add_brackets, const string& display_key = "")
{
	string val_to_print = "<EMPTY>";
	bool found = false;

	// 1. 查找值 (First Match 策略)
	for (size_t i = 0; i < items.size(); i++) {
		if (items[i].key == key && items[i].has_separator) {
			val_to_print = items[i].value;
			found = true;
			break;
		}
	}

	// 2. 格式化处理：加 []
	if (found && add_brackets) {
		if (!val_to_print.empty() && val_to_print.front() != '[') {
			val_to_print = "[" + val_to_print + "]";
		}
	}

	// 3. [目录补全]：强制补全反斜杠 (\)
	if (key == "single_exe_dirname" ||
		key == "multi_exe_main_dirname" ||
		key == "multi_exe_sub_dirname" ||
		key == "redirection_data_dirname")
	{
		if (val_to_print.length() > 0 && val_to_print.back() != '\\') {
			val_to_print += "\\";
		}
	}

	// 4. 打印
	// [新增逻辑] 如果指定了 display_key，则打印 display_key，否则打印原始 key
	string key_to_show = (display_key.empty()) ? key : display_key;

	cout << "  ";
	cout << left << setw(max_len) << key_to_show << " = " << val_to_print << endl;
}

static bool check_path_open(const string& path)
{
	if (path.empty()) return false;

	struct stat buffer;

	return (stat(path.c_str(), &buffer) == 0);
}
// =========================================================================
// 主函数：print_item
// =========================================================================
bool config_file_tools::print_item(const char* Group_name, bool show_error, bool silent)
{
	// 1. 转换输入参数
	string target_name = (Group_name == nullptr) ? "" : Group_name;
	if (!target_name.empty() && target_name.front() != '[') {
		target_name = "[" + target_name + "]";
	}

	for (size_t g_idx = 0; g_idx < groups.size(); g_idx++)
	{
		const Group& group = groups[g_idx];

		if (is_string_equal(group.name, target_name, false))
		{

			for (size_t i = 0; i < group.items.size(); i++) 
			{
				if (group.items[i].key == "include" && group.items[i].has_separator) 
				{
					string parent_name = group.items[i].value;
					if (!parent_name.empty()) {
						if (!is_string_equal(parent_name, group.name, false)) 
						{
							print_item(parent_name.c_str(), false); // 递归调用，父类不报错
						}
					}
					break;
				}
			}

			string current_exe_style = "";
			string current_cmd_style = "";
			string current_name_list = "";
			int current_items_num = 0;
			vector<string> existing_keys;

			string val_demo_exe = "";
			string val_single_dir = "";
			string val_multi_main = "";
			string val_multi_sub = "";
			string val_pipe_exe = "";
			string val_pipe_data = "";
			string val_redir_dir = "";

			for (size_t i = 0; i < group.items.size(); i++) 
			{
				const Item& item = group.items[i];
				existing_keys.push_back(item.key);
				if (current_exe_style == "" && item.key == "exe_style") current_exe_style = item.value;
				else if (current_cmd_style == "" && item.key == "cmd_style") current_cmd_style = item.value;
				else if (current_name_list == "" && item.key == "name_list") current_name_list = item.value;
				else if (current_items_num == 0 && item.key == "items_num") 
				{
					try 
					{ current_items_num = stoi(item.value); 
					}
					catch (...) 
					{ current_items_num = 0;
					}
				}
				else if (val_demo_exe == "" && item.key == "demo_exe_name") val_demo_exe = item.value;
				else if (val_single_dir == "" && item.key == "single_exe_dirname") val_single_dir = item.value;
				else if (val_multi_main == "" && item.key == "multi_exe_main_dirname") val_multi_main = item.value;
				else if (val_multi_sub == "" && item.key == "multi_exe_sub_dirname") val_multi_sub = item.value;
				else if (val_pipe_exe == "" && item.key == "pipe_get_input_data_exe_name") val_pipe_exe = item.value;
				else if (val_pipe_data == "" && item.key == "pipe_data_file") val_pipe_data = item.value;
				else if (val_redir_dir == "" && item.key == "redirection_data_dirname") val_redir_dir = item.value;
			}

			vector<string> error_report;

			if (!key_exists_helper(existing_keys, "demo_exe_name")) 
			{
				error_report.push_back("缺少参考程序路径 (demo_exe_name)");
			}
			else 
			{
				// 存在则检查是否可打开
				if (!check_path_open(val_demo_exe))
					error_report.push_back("参考程序路径无效或无法打开: " + val_demo_exe);
			}

			// 2. 检查 exe_style 相关
			if (current_exe_style != "none") 
			{
				if (!key_exists_helper(existing_keys, "name_list")) 
				{
					error_report.push_back("缺少学生名单配置 (name_list)");
				}
				else 
				{
					if (!is_string_equal(current_name_list, "database", false)) 
					{
						if (!check_path_open(current_name_list))
							error_report.push_back("学生名单文件路径无效或无法打开: " + current_name_list);
					}
				}

				if (current_exe_style == "single") 
				{
					if (!key_exists_helper(existing_keys, "single_exe_dirname")) 
					{
						error_report.push_back("缺少Single目录 (single_exe_dirname)");
					}
					else 
					{
						if (!check_path_open(val_single_dir))
							error_report.push_back("Single目录无效或无法打开: " + val_single_dir);
					}

					if (!key_exists_helper(existing_keys, "stu_exe_name"))
						error_report.push_back("缺少学生EXE命名 (stu_exe_name)");
				}
				else if (current_exe_style == "multi") 
				{
					if (!key_exists_helper(existing_keys, "multi_exe_main_dirname")) 
					{
						error_report.push_back("缺少Multi主目录 (multi_exe_main_dirname)");
					}
					else 
					{
						if (!check_path_open(val_multi_main))
							error_report.push_back("Multi主目录无效或无法打开: " + val_multi_main);
					}

					//if (!key_exists_helper(existing_keys, "multi_exe_sub_dirname")) {
						//error_report.push_back("缺少Multi子目录 (multi_exe_sub_dirname)");
					//}
					//else 
					//{
						//if (!check_path_open(val_multi_sub))
							//error_report.push_back("Multi子目录无效或无法打开: " + val_multi_sub);
					//}

					if (!key_exists_helper(existing_keys, "stu_exe_name"))
						error_report.push_back("缺少学生EXE命名 (stu_exe_name)");
				}
			}

			if (current_cmd_style == "pipe") 
			{
				if (!key_exists_helper(existing_keys, "pipe_get_input_data_exe_name")) 
				{
					error_report.push_back("缺少Pipe工具 (pipe_get_input_data_exe_name)");
				}
				else 
				{
					if (!check_path_open(val_pipe_exe))
						error_report.push_back("Pipe工具路径无效或无法打开: " + val_pipe_exe);
				}

				if (!key_exists_helper(existing_keys, "pipe_data_file")) 
				{
					error_report.push_back("缺少Pipe数据文件 (pipe_data_file)");
				}
				else 
				{
					if (!check_path_open(val_pipe_data))
						error_report.push_back("Pipe数据文件无效或无法打开: " + val_pipe_data);
				}
			}
			else if (current_cmd_style == "redirection") 
			{
				if (!key_exists_helper(existing_keys, "redirection_data_dirname")) 
				{
					error_report.push_back("缺少重定向目录 (redirection_data_dirname)");
				}
				else 
				{
					if (!check_path_open(val_redir_dir))
						error_report.push_back("重定向目录无效或无法打开: " + val_redir_dir);
				}
			}

			string item_prefix = "";
			if (current_cmd_style == "pipe") item_prefix = "item_gname_";
			else if (current_cmd_style == "redirection") item_prefix = "item_fname_";
			else if (current_cmd_style == "main_with_arguments") item_prefix = "item_args_";

			if (current_items_num > 0) 
			{
				if (!key_exists_helper(existing_keys, "items_begin")) error_report.push_back("缺少起始项配置 (items_begin)");
				if (!key_exists_helper(existing_keys, "items_end")) error_report.push_back("缺少结束项配置 (items_end)");
				if (!item_prefix.empty()) 
				{
					for (int i = 1; i <= current_items_num; ++i) 
					{
						if (!key_exists_helper(existing_keys, item_prefix + to_string(i)))
							error_report.push_back("缺少测试项细节 (" + item_prefix + to_string(i) + ")");
					}
				}
			}

			int calculated_max_len = 0;

			for (size_t i = 0; i < group.items.size(); i++) 
			{
				if (!group.items[i].has_separator) continue;
				string key = group.items[i].key;

				if (key == "include") continue;

				if (key == "demo_exe_name") continue; // 稍后在注入项中计算
				if (key == "name_list" || key == "namelist" || key == "single_exe_dirname" ||
					key == "multi_exe_main_dirname" || key == "multi_exe_sub_dirname" || key == "stu_exe_name") continue;
				if (key == "timeout" || key == "max_output_len" ||
					key == "pipe_get_input_data_exe_name" || key == "pipe_data_file" ||
					key == "redirection_data_dirname") continue;
				if (key == "items_begin" || key == "items_end") continue;
				if (key.find("item_gname_") == 0 || key.find("item_fname_") == 0 || key.find("item_args_") == 0) continue;

				// 只有未被拦截的项，才计入长度
				if ((int)key.length() > calculated_max_len) calculated_max_len = (int)key.length();
			}

			if (current_exe_style != "none")
			{
				int l = (int)string("name_list").length();
				if (l > calculated_max_len) calculated_max_len = l;

				if (current_exe_style == "single") 
				{
					l = (int)string("single_exe_dirname").length();
					if (l > calculated_max_len) calculated_max_len = l;
					l = (int)string("stu_exe_name").length();
					if (l > calculated_max_len) calculated_max_len = l;
				}
				else if (current_exe_style == "multi") 
				{
					l = (int)string("multi_exe_main_dirname").length();
					if (l > calculated_max_len) calculated_max_len = l;
					l = (int)string("multi_exe_sub_dirname").length();
					if (l > calculated_max_len) calculated_max_len = l;
					l = (int)string("stu_exe_name").length();
					if (l > calculated_max_len) calculated_max_len = l;
				}
			}
			// demo_exe_name 始终注入
			int l_demo = (int)string("demo_exe_name").length();
			if (l_demo > calculated_max_len) calculated_max_len = l_demo;

			int l_max = (int)string("max_output_len").length();
			if (l_max > calculated_max_len) calculated_max_len = l_max;
			int l_tout = (int)string("timeout").length();
			if (l_tout > calculated_max_len) calculated_max_len = l_tout;

			if (current_cmd_style == "pipe") 
			{
				int l = (int)string("pipe_get_input_data_exe_name").length();
				if (l > calculated_max_len) calculated_max_len = l;
				l = (int)string("pipe_data_file").length();
				if (l > calculated_max_len) calculated_max_len = l;
			}
			else if (current_cmd_style == "redirection") 
			{
				int l = (int)string("redirection_data_dirname").length();
				if (l > calculated_max_len) calculated_max_len = l;
			}

			if (current_items_num > 0) 
			{
				int l = (int)string("items_begin").length();
				if (l > calculated_max_len) calculated_max_len = l;
				l = (int)string("items_end").length();
				if (l > calculated_max_len) calculated_max_len = l;

				if (!item_prefix.empty()) 
				{
					string max_item_key = "item_name_" + to_string(current_items_num);
					if ((int)max_item_key.length() > calculated_max_len)
						calculated_max_len = (int)max_item_key.length();
				}
			}
			//废版长度计算逻辑 暂时不删
			/*const char* missing_candidates[] = {
				"demo_exe_name", "max_output_len", "timeout", "name_list", "single_exe_dirname",
				"multi_exe_main_dirname", "multi_exe_sub_dirname", "stu_exe_name",
				"pipe_get_input_data_exe_name", "pipe_data_file", "redirection_data_dirname",
				"items_begin", "items_end"
			};
			for (int k = 0; k < sizeof(missing_candidates) / sizeof(missing_candidates[0]); k++) {
				int len = (int)string(missing_candidates[k]).length();
				if (len > calculated_max_len) calculated_max_len = len;
			}
			if (!item_prefix.empty()) {
				for (int i = 1; i <= current_items_num; ++i) {
					int len = (int)(item_prefix + to_string(i)).length();
					if (len > calculated_max_len) calculated_max_len = len;
				}
			}*/
			if (silent == true)
			{
				return error_report.size();
			}

			cout << endl;
			cout << setfill('=') << setw(100) << "" << endl;
			cout << setfill(' ');
			cout << group.name << "配置信息如下：" << endl;
			cout << setfill('=') << setw(100) << "" << endl;
			cout << setfill(' ');

			bool need_db_info = (current_exe_style != "none" && current_name_list =="database");
			bool db_printed = false;

			if (need_db_info)
			{
				// 查找数据库组
				int db_idx = -1;
				for (size_t k = 0; k < groups.size(); k++) 
				{
					if (is_string_equal(groups[k].name, "[数据库]", false)) 
					{
						db_idx = (int)k;
						break;
					}
				}

				if (db_idx != -1)
				{
					cout << "[数据库]：" << endl;

					// 计算数据库项的最大长度，以便对齐
					int db_max_len = 0;
					const Group& db_group = groups[db_idx];
					for (size_t k = 0; k < db_group.items.size(); k++) 
					{
						if (db_group.items[k].has_separator) 
						{
							if ((int)db_group.items[k].key.length() > db_max_len)
								db_max_len = (int)db_group.items[k].key.length();
						}
					}

					// 打印项
					for (size_t k = 0; k < db_group.items.size(); k++) 
					{
						const Item& item = db_group.items[k];
						if (item.has_separator) 
						{
							if (item.key == "db_passwd") continue;
							print_kv_helper(db_group.items, item.key, db_max_len, false);
						}
					}

					cout << endl; // 数据库项打印后换行
					db_printed = true;
				}
			}
			cout << group.name << "：" << endl; // 打印当前组名

			vector<string> processed_keys;

			for (size_t i = 0; i < group.items.size(); i++)
			{
				const Item& temp = group.items[i];
				if (!temp.has_separator) continue;
				string key = temp.key;

				if (key == "include") continue;
				if (key_exists_helper(processed_keys, key)) continue;
				processed_keys.push_back(key);

				// 拦截逻辑
				if (key == "demo_exe_name") continue;
				if (key == "name_list" || key == "namelist" || key == "single_exe_dirname" ||
					key == "multi_exe_main_dirname" || key == "multi_exe_sub_dirname" || key == "stu_exe_name") continue;
				if (key == "timeout" || key == "max_output_len" ||
					key == "pipe_get_input_data_exe_name" || key == "pipe_data_file" ||
					key == "redirection_data_dirname") continue;
				if (key == "items_begin" || key == "items_end") continue;
				if (key.find("item_gname_") == 0 || key.find("item_fname_") == 0 || key.find("item_args_") == 0) continue;

				// 正常打印
				if (key == "tc_trim" || key == "items_num") cout << endl;
				print_kv_helper(group.items, key, calculated_max_len, false);

				// 注射逻辑
				if (key == "exe_style") 
				{
					if (current_exe_style != "none") 
					{
						print_kv_helper(group.items, "name_list", calculated_max_len, false);
						if (current_exe_style == "single") 
						{
							print_kv_helper(group.items, "single_exe_dirname", calculated_max_len, false);
							print_kv_helper(group.items, "stu_exe_name", calculated_max_len, false);
						}
						else if (current_exe_style == "multi") 
						{
							print_kv_helper(group.items, "multi_exe_main_dirname", calculated_max_len, false);
							print_kv_helper(group.items, "multi_exe_sub_dirname", calculated_max_len, false);
							print_kv_helper(group.items, "stu_exe_name", calculated_max_len, false);
						}
					}
					print_kv_helper(group.items, "demo_exe_name", calculated_max_len, false);
				}

				if (key == "cmd_style") 
				{
					print_kv_helper(group.items, "max_output_len", calculated_max_len, false);
					print_kv_helper(group.items, "timeout", calculated_max_len, false);
					if (current_cmd_style == "pipe")
					{
						print_kv_helper(group.items, "pipe_get_input_data_exe_name", calculated_max_len, false);
						print_kv_helper(group.items, "pipe_data_file", calculated_max_len, false);
					}
					else if (current_cmd_style == "redirection") 
					{
						print_kv_helper(group.items, "redirection_data_dirname", calculated_max_len, false);
					}
				}

				if (key == "items_num") 
				{
					print_kv_helper(group.items, "items_begin", calculated_max_len, false);
					print_kv_helper(group.items, "items_end", calculated_max_len, false);

					if (!item_prefix.empty()) 
					{
						for (int k = 1; k <= current_items_num; ++k) 
						{
							string display_name = "item_name_" + to_string(k);
							string real_key = item_prefix + to_string(k);

							bool need_brackets = (item_prefix != "item_args_" && item_prefix != "item_fname_");

							// 调用时传入 display_name
							print_kv_helper(group.items, real_key, calculated_max_len, need_brackets, display_name);
						}
					}
				}
			}

			cout << setfill('=') << setw(100) << "" << endl;
			cout << setfill(' ');

			cout << endl;
			if (show_error && !error_report.empty()) 
			{
				cerr << endl;
				cerr << "[--严重错误--] 配置文件存在下列的错误：" << endl;
				for (size_t k = 0; k < error_report.size(); k++) 
				{
					cerr << "  - " << error_report[k] << endl;
				}
			}
			break;
		}
	}
	return 0;//无用返回值
}
/***************************************************************************
  函数名称：构造函数
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
config_file_tools::config_file_tools(const char* const _cfgname, const enum BREAK_CTYPE _ctype) : file_read_succeeded(false)
{
	/* 按需完成 */
	this->cfgname = _cfgname;
	this->item_separate_character_type = _ctype;

	//读取逻辑
	ifstream cfg_file(this->cfgname.c_str());//需要将string转为c语言风格的字符串来打开文件

	if (!cfg_file.is_open())
	{
		this->file_read_succeeded = false;
		return;
	}
	this->file_read_succeeded = true;//到这里说明读成功了
	string current_group_name = "";
	int line_count = 0;

	//逐行读取
	string line;
	while (getline(cfg_file, line))
	{
		line_count++;
		bool is_too_long = false;
		string content = parse_line_content(line, is_too_long);

		//长度过长
		if (is_too_long)
		{
			cout << "非法格式的的配置文件," << "第[" << line_count << "]行超过最大长度1024" << endl;
			this->file_read_succeeded = false;
			cfg_file.close();
			return;
		}

		if (content == "")
			continue;//跳过空行的处理

		if (content.front() == '[' && content.back() == ']')//说明此行是组名
		{
			string group_name = line.substr(1, line.length() - 2);
			group_name = trim(group_name);
			
			if (group_name == "")
				continue;//对空名不处理

			current_group_name = "[" + group_name + "]";

			int group_idx = find_group_index(current_group_name); 
			if (group_idx == -1)
			{
				Group new_group;
				new_group.name = current_group_name;
				this->groups.push_back(new_group);
			}
		}
		else//说明是项
		{
			Item new_item;
			new_item.full_line = content;
			new_item.has_separator = false;

			string key;
			string value;
			size_t separator_pos = string::npos;

			if (this->item_separate_character_type == BREAK_CTYPE::Equal)
				separator_pos = content.find('=');
			else if (this->item_separate_character_type == BREAK_CTYPE::Space)
				separator_pos = content.find_first_of(" \t");

			if (separator_pos != string::npos)//说明找到了分隔符
			{
				key = content.substr(0, separator_pos);
				value = content.substr(separator_pos + 1);
				
				key = trim(key);
				value = trim(value);

				if (!key.empty())
				{
					new_item.key = key;
					new_item.value = value;
					new_item.has_separator = true;
				}
			}

			int group_idx = find_group_index(current_group_name);

			if (group_idx == -1)
			{
				Group new_group;
				new_group.name = current_group_name;
				groups.push_back(new_group);
				group_idx = (int)groups.size() - 1;
			}
			//更新长度
			groups[group_idx].max_item_length = (groups[group_idx].max_item_length > (int)new_item.key.length()) ? groups[group_idx].max_item_length : (int)new_item.key.length();
			groups[group_idx].items.push_back(new_item);
		}
	}
	cfg_file.close();
	if (this->groups.size() > 0) 
	{
		// 初始化状态数组：0 表示未处理
		vector<int> status(this->groups.size(), 0);

		// 遍历所有组，触发解析
		for (size_t i = 0; i < this->groups.size(); i++) {
			resolve_include((int)i, status);
		}
	}
}

/***************************************************************************
  函数名称：构造函数
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
config_file_tools::config_file_tools(const string& _cfgname, const enum BREAK_CTYPE _ctype) : file_read_succeeded(false)
{
	/* 按需完成 */
	this->cfgname = _cfgname;
	this->item_separate_character_type = _ctype;

	//读取逻辑
	ifstream cfg_file(this->cfgname.c_str());//需要将string转为c语言风格的字符串来打开文件

	if (!cfg_file.is_open())
	{
		this->file_read_succeeded = false;
		return;
	}
	this->file_read_succeeded = true;//到这里说明读成功了
	string current_group_name = "";
	int line_count = 0;

	//逐行读取
	string line;
	while (getline(cfg_file, line))
	{
		line_count++;
		bool is_too_long = false;
		string content = parse_line_content(line, is_too_long);

		//长度过长
		if (is_too_long)
		{
			cout << "非法格式的的配置文件," << "第[" << line_count << "]行超过最大长度1024" << endl;
			this->file_read_succeeded = false;
			cfg_file.close();
			return;
		}

		if (content == "")
			continue;//跳过空行的处理

		if (content.front() == '[' && content.back() == ']')//说明此行是组名
		{
			string group_name = line.substr(1, line.length() - 2);
			group_name = trim(group_name);

			if (group_name == "")
				continue;//对空名不处理

			current_group_name = "[" + group_name + "]";

			int group_idx = find_group_index(current_group_name);
			if (group_idx == -1)
			{
				Group new_group;
				new_group.name = current_group_name;
				this->groups.push_back(new_group);
			}
		}
		else//说明是项
		{
			Item new_item;
			new_item.full_line = content;
			new_item.has_separator = false;

			string key;
			string value;
			size_t separator_pos = string::npos;

			if (this->item_separate_character_type == BREAK_CTYPE::Equal)
				separator_pos = content.find('=');
			else if (this->item_separate_character_type == BREAK_CTYPE::Space)
				separator_pos = content.find_first_of(" \t");

			if (separator_pos != string::npos)//说明找到了分隔符
			{
				key = content.substr(0, separator_pos);
				value = content.substr(separator_pos + 1);

				key = trim(key);
				value = trim(value);

				if (!key.empty())
				{
					new_item.key = key;
					new_item.value = value;
					new_item.has_separator = true;
				}
			}

			int group_idx = find_group_index(current_group_name);

			if (group_idx == -1)
			{
				Group new_group;
				new_group.name = current_group_name;
				groups.push_back(new_group);
				group_idx = (int)groups.size() - 1;
			}

			groups[group_idx].items.push_back(new_item);
		}
	}
	cfg_file.close();
	if (this->groups.size() > 0) 
	{
		// 初始化状态数组：0 表示未处理
		vector<int> status(this->groups.size(), 0);

		// 遍历所有组，触发解析
		for (size_t i = 0; i < this->groups.size(); i++) {
			resolve_include((int)i, status);
		}
	}
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
config_file_tools::~config_file_tools()
{
	/* 按需完成 */
}


/***************************************************************************
  函数名称：
  功    能：判断读配置文件是否成功
  输入参数：
  返 回 值：true - 成功，已读入所有的组/项
		   false - 失败，文件某行超长/文件全部是注释语句
  说    明：
***************************************************************************/
bool config_file_tools::is_read_succeeded() const
{
	/* 按需完成，根据需要改变return的值 */
	return file_read_succeeded;
}

/***************************************************************************
  函数名称：
  功    能：返回配置文件中的所有组
  输入参数：vector <string>& ret : vector 中每项为一个组名
  返 回 值：读到的组的数量（简单配置文件的组数量为1，组名为"）
  说    明：
***************************************************************************/
int config_file_tools::get_all_group(vector <string>& ret)
{
	/* 按需完成，根据需要改变return的值 */
	ret.clear();

	// 如果读取失败，直接返回0
	if (!file_read_succeeded) {
		return 0;
	}

	for (const auto& group : groups)
	{
		ret.push_back(group.name);
	}

	return (int)ret.size();
}

/***************************************************************************
  函数名称：
  功    能：查找指定组的所有项并返回项的原始内容
  输入参数：const char* const group_name：组名
		   vector <string>& ret：vector 中每项为一个项的原始内容
		   const bool is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：项的数量，0表示空
  说    明：
***************************************************************************/
int config_file_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	ret.clear();
	if (group_name == nullptr)
	{
		return 0;
	}

	int item_count = 0;
	string gname_str = group_name;

	int group_idx = -1;
	for (size_t i = 0; i < groups.size(); ++i) 
	{
		if (is_string_equal(groups[i].name, gname_str, is_case_sensitive)) 
		{
			group_idx = i;
			break;
		}
	}

	if (group_idx == -1) 
	{
		return 0; // 组名不存在
	}

	const Group& current_group = groups[group_idx];

	for (Item item : current_group.items)
	{
		ret.push_back(item.full_line);
		item_count++;
	}

	return item_count;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::get_all_item(const string& group_name, vector <string>& ret, const bool is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	return this->get_all_item(group_name.c_str(),ret,is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的原始内容（=后的所有字符，string方式）
  输入参数：const char* const group_name
		   const char* const item_name
		   string &ret
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_raw(const char* const group_name, const char* const item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	if (group_name == nullptr || item_name == nullptr) {
		ret = DEFAULT_STRING_VALUE;
		return 0;
	}

	// 1. 检查项是否存在
	if (item_exists_in_group(group_name, item_name, group_is_case_sensitive, item_is_case_sensitive)) {

		// 2. 如果存在，则调用 find_item_value 获取其值 (值可能为 "" )
		string raw_value = find_item_value(group_name, item_name,
			group_is_case_sensitive, item_is_case_sensitive);

		ret = raw_value;
		return 1; // 找到，并成功设置了值（即使值是空串也成功）
	}
	else {
		ret = DEFAULT_STRING_VALUE;
		return 0; // 找不到
	}
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_raw(const string& group_name, const string& item_name, string& ret, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_raw(group_name.c_str(), item_name.c_str(), ret, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为char型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：1 - 该项的项名存在
		   0 - 该项的项名不存在
  说    明：
***************************************************************************/
int config_file_tools::item_get_null(const char* const group_name, const char* const item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 按需完成，根据需要改变return的值 */
	if (group_name == nullptr || item_name == nullptr) {
		return 0;
	}

	// 直接调用新的辅助函数判断键名是否存在，不依赖值是否为空。
	return item_exists_in_group(group_name, item_name,
		group_is_case_sensitive, item_is_case_sensitive) ? 1 : 0;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_null(const string& group_name, const string& item_name, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_null(group_name.c_str(), item_name.c_str(), group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为char型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   char& value                                ：读到的char的值（返回1时可信，返回0则不可信）
		   const char* const choice_set = nullptr     ：合法的char的集合（例如："YyNn"表示合法输入为Y/N且不分大小写，该参数有默认值nullptr，表示全部字符，即不检查）
		   const char def_value = DEFAULT_CHAR_VALUE  ：读不到/读到非法的情况下的默认值，该参数有默认值DEFAULT_CHAR_VALUE，分两种情况
															当值是   DEFAULT_CHAR_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_CHAR_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：1 - 取到正确值
			   未取到值/未取到正确值，设置了缺省值（包括设为缺省值）
		   0 - 未取到（只有为未指定默认值的情况下才会返回0）
  说    明：
***************************************************************************/
int config_file_tools::item_get_char(const char* const group_name, const char* const item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_val;
	bool fetch_success = false;

	if (item_get_raw(group_name, item_name, raw_val, group_is_case_sensitive, item_is_case_sensitive) == 1) 
	{
		if (!raw_val.empty()) 
		{
			istringstream iss(raw_val);
			char temp_c;
			iss >> temp_c;

			if (!iss.fail())
			{
				
				if (choice_set == nullptr) 
				{
					value = temp_c;
					fetch_success = true;
				}
				else 
				{
					// 检查字符是否在 choice_set 中
					string valid_chars = choice_set;
					if (valid_chars.find(temp_c) != string::npos) 
					{
						value = temp_c;
						fetch_success = true;
					}
				}
			}
		}
	}

	if (fetch_success) 
	{
		return 1;
	}

	if (def_value == DEFAULT_CHAR_VALUE) 
	{
		return 0;
	}
	else 
	{
		value = def_value;
		return 1;
	}
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_char(const string& group_name, const string& item_name, char& value,
						const char* const choice_set, const char def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_char(group_name.c_str(), item_name.c_str(), value, choice_set, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为int型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   int& value                                 ：读到的int的值（返回1时可信，返回0则不可信）
		   const int min_value = INT_MIN              : 期望数据范围的下限，默认为INT_MIN
		   const int max_value = INT_MAX              : 期望数据范围的上限，默认为INT_MAX
		   const int def_value = DEFAULT_INT_VALUE    ：读不到/读到非法的情况下的默认值，该参数有默认值 DEFAULT_INT_VALUE，分两种情况
															当值是   DEFAULT_INT_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_INT_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_int(const char* const group_name, const char* const item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_val;
	bool fetch_success = false;

	if (item_get_raw(group_name, item_name, raw_val, group_is_case_sensitive, item_is_case_sensitive) == 1) 
	{
		if (!raw_val.empty()) 
		{
			istringstream iss(raw_val);
			int temp_val;
			iss >> temp_val;

				if (!iss.fail())
				{
					if (temp_val >= min_value && temp_val <= max_value) 
					{
						value = temp_val;
						fetch_success = true;
					}
				}
		}
	}

	if (fetch_success) return 1;

	if (def_value == DEFAULT_INT_VALUE) return 0;
	value = def_value;
	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_int(const string& group_name, const string& item_name, int& value,
						const int min_value, const int max_value, const int def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_int(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为double型
  输入参数：const char* const group_name                  ：组名
		   const char* const item_name                   ：项名
		   double& value                                 ：读到的int的值（返回1时可信，返回0则不可信）
		   const double min_value = __DBL_MIN__          : 期望数据范围的下限，默认为INT_MIN
		   const double max_value = __DBL_MAX__          : 期望数据范围的上限，默认为INT_MAX
		   const double def_value = DEFAULT_DOUBLE_VALUE ：读不到/读到非法的情况下的默认值，该参数有默认值DEFAULT_DOUBLE_VALUE，分两种情况
																当值是   DEFAULT_DOUBLE_VALUE 时，返回0（值不可信）
																当值不是 DEFAULT_DOUBLE_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false     : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false      : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_double(const char* const group_name, const char* const item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_val;
	bool fetch_success = false;

	if (item_get_raw(group_name, item_name, raw_val, group_is_case_sensitive, item_is_case_sensitive) == 1) 
	{
		if (!raw_val.empty()) 
		{
			istringstream iss(raw_val);
			double temp_val;
			iss >> temp_val;

				if (!iss.fail())
				{
					if (temp_val >= min_value && temp_val <= max_value) 
					{
						value = temp_val;
						fetch_success = true;
					}
				}
		}
	}

	if (fetch_success) return 1;

	if (def_value == DEFAULT_DOUBLE_VALUE) return 0;
	value = def_value;
	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_double(const string& group_name, const string& item_name, double& value,
						const double min_value, const double max_value, const double def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_double(group_name.c_str(), item_name.c_str(), value, min_value, max_value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为char * / char []型
  输入参数：const char* const group_name                  ：组名
		   const char* const item_name                   ：项名
		   char *const value                             ：读到的C方式的字符串（返回1时可信，返回0则不可信）
		   const int str_maxlen                          ：指定要读的最大长度（含尾零）
																如果<1则返回空串(不是DEFAULT_CSTRING_VALUE，虽然现在两者相同，但要考虑default值可能会改)
																如果>MAX_STRLEN 则上限为MAX_STRLEN
		   const char* const def_str                     ：读不到情况下的默认值，该参数有默认值DEFAULT_CSTRING_VALUE，分两种情况
																当值是   DEFAULT_CSTRING_VALUE 时，返回0（值不可信）
																当值不是 DEFAULT_CSTRING_VALUE 时，令value=def_value并返回1（注意，不是直接=）
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：1、为简化，未对\"等做转义处理，均按普通字符
		   2、含尾零的最大长度为str_maxlen，调用时要保证有足够空间
		   3、如果 str_maxlen 超过了系统预设的上限 MAX_STRLEN，则按 MAX_STRLEN 取
***************************************************************************/
int config_file_tools::item_get_cstring(const char* const group_name, const char* const item_name, char* const value,
	const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	// 安全检查：value 指针不能为空
	if (value == nullptr) return 0;
	value[0] = '\0'; // 预先清空目标缓冲区

	string raw_val;
	string final_value = "";
	bool fetch_success = false;

	if (item_get_raw(group_name, item_name, raw_val, group_is_case_sensitive, item_is_case_sensitive) == 1) {

		
		if (!raw_val.empty()) 
		{
			istringstream iss(raw_val);
			iss >> final_value; 

			if (!iss.fail()) 
			{
				fetch_success = true;
			}
		}
	}

	if (fetch_success) 
	{

		int actual_maxlen = str_maxlen;
		if (actual_maxlen > MAX_STRLEN) 
		{
			actual_maxlen = MAX_STRLEN;
		}

		if (actual_maxlen > 0) 
		{
			// 安全拷贝 final_value 到 value
			size_t copy_len = final_value.length();
			if (copy_len > (size_t)(actual_maxlen - 1)) 
			{
				copy_len = (size_t)(actual_maxlen - 1);
			}

			memcpy(value, final_value.c_str(), copy_len);
			value[copy_len] = '\0'; // 确保尾零

			return 1; // 成功
		}
		else 
		{
			// actual_maxlen <= 0 且 fetch_success = true，返回空串（已在函数开头处理）
			return 0; // 长度非法，视为不可信
		}
	}

	// 4. 处理失败/默认值逻辑
	if (def_value == DEFAULT_CSTRING_VALUE) 
	{
		return 0;
	}
	else 
	{
		// 拷贝默认值 (与前面拷贝逻辑相同)
		int actual_maxlen = str_maxlen;
		if (actual_maxlen > MAX_STRLEN) actual_maxlen = MAX_STRLEN;

		if (actual_maxlen > 0 && def_value != nullptr) 
		{
			size_t copy_len = strlen(def_value);
			if (copy_len > (size_t)(actual_maxlen - 1)) 
			{
				copy_len = (size_t)(actual_maxlen - 1);
			}
			memcpy(value, def_value, copy_len);
			value[copy_len] = '\0';
		}
		else if (actual_maxlen > 0) 
		{
			value[0] = '\0';
		}
		return 1;
	}
}
/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_cstring(const string& group_name, const string& item_name, char* const value,
						const int str_maxlen, const char* const def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)

{
	/* 本函数已实现 */
	return item_get_cstring(group_name.c_str(), item_name.c_str(), value, str_maxlen, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为 string 型
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   string &value                              ：读到的string方式的字符串（返回1时可信，返回0则不可信）
		   const string &def_value                    ：读不到情况下的默认值，该参数有默认值DEFAULT_STRING_VALUE，分两种情况
															当值是   DEFAULT_STRING_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_STRING_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：为简化，未对\"等做转义处理，均按普通字符
***************************************************************************/
int config_file_tools::item_get_string(const char* const group_name, const char* const item_name, string& value,
	const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_val;

	if (item_get_raw(group_name, item_name, raw_val, group_is_case_sensitive, item_is_case_sensitive) == 1) 
	{

		if (!raw_val.empty()) 
		{
			istringstream iss(raw_val);
			string temp_str;
			iss >> temp_str; // 只读取第一个由空格/Tab分隔的词

			if (!iss.fail()) 
			{
				value = temp_str;
				return 1;
			}
		}
	}

	if (def_value == DEFAULT_STRING_VALUE) 
	{
		return 0; // 值不可信
	}
	else 
	{
		value = def_value;
		return 1; // 默认值可信
	}
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_string(const string& group_name, const string& item_name, string& value,
						const string& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_string(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}

/***************************************************************************
  函数名称：
  功    能：取某项的内容，返回类型为 IPv4 地址的32bit整型（主机序）
  输入参数：const char* const group_name               ：组名
		   const char* const item_name                ：项名
		   unsigned int &value                        ：读到的IP地址，32位整型方式（返回1时可信，返回0则不可信）
		   const unsigned int &def_value              ：读不到情况下的默认值，该参数有默认值DEFAULT_IPADDR_VALUE，分两种情况
															当值是   DEFAULT_IPADDR_VALUE 时，返回0（值不可信）
															当值不是 DEFAULT_IPADDR_VALUE 时，令value=def_value并返回1
		   const bool group_is_case_sensitive = false : 组名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
		   const bool item_is_case_sensitive = false  : 项名是否大小写敏感，true-大小写敏感 / 默认false-大小写不敏感
  返 回 值：
  说    明：
***************************************************************************/
int config_file_tools::item_get_ipaddr(const char* const group_name, const char* const item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	string raw_val;
	bool fetch_success = false;

	if (item_get_raw(group_name, item_name, raw_val, group_is_case_sensitive, item_is_case_sensitive) == 1) 
	{
		if (!raw_val.empty()) 
		{
			istringstream iss(raw_val);
			int i1, i2, i3, i4;
			char dot1, dot2, dot3;

			iss >> i1 >> dot1 >> i2 >> dot2 >> i3 >> dot3 >> i4;

			if (!iss.fail() && dot1 == '.' && dot2 == '.' && dot3 == '.') 
			{
				// 验证每个段是否在 0-255 之间
				if (i1 >= 0 && i1 <= 255 &&
					i2 >= 0 && i2 <= 255 &&
					i3 >= 0 && i3 <= 255 &&
					i4 >= 0 && i4 <= 255) {

					value = ((unsigned int)i1 << 24) |
						((unsigned int)i2 << 16) |
						((unsigned int)i3 << 8) |
						(unsigned int)i4;

					fetch_success = true;
				}
			}
		}
	}

	if (fetch_success) return 1;

	// 默认值逻辑
	if (def_value == DEFAULT_IPADDR_VALUE) return 0;
	value = def_value;
	return 1;
}

/***************************************************************************
  函数名称：
  功    能：组名/项目为string方式，其余同上
  输入参数：
  返 回 值：
  说    明：因为工具函数一般在程序初始化阶段被调用，不会在程序执行中被高频次调用，
		   因此这里直接套壳，会略微影响效率，但不影响整体性能（对高频次调用，此方法不适合）
***************************************************************************/
int config_file_tools::item_get_ipaddr(const string& group_name, const string& item_name, unsigned int& value,
						const unsigned int& def_value, const bool group_is_case_sensitive, const bool item_is_case_sensitive)
{
	/* 本函数已实现 */
	return this->item_get_ipaddr(group_name.c_str(), item_name.c_str(), value, def_value, group_is_case_sensitive, item_is_case_sensitive);
}
