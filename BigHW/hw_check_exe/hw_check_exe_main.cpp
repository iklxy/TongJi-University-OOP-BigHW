
//2453359 信安 李欣洋
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
#include<string>
#include<sstream>
#include<ctime>
#include<fstream>
#include "../include/class_aat.h"
#include "../include/class_cft.h"
#include "student_info.h"
#include "make_path.h"
#include "exe_check.h"
#include "../include/class_tc.h"
using namespace std;
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
enum OPT_ARGS
{
	OPT_ARGS_HELP = 0,
	OPT_ARGS_DEBUG,
	OPT_ARGS_CHECKNAME,
	OPT_ARGS_CHECKCFGONLY,
	OPT_ARGS_CFGFILE
};

const char* title = "Usage: hw_check_exe_demo.exe(替换为自己的可执行程序名) --必选项 | --可选项(出现次序不限)\n\
\n\
必选项：指定操作\n\
	--checkname : 需要检查的项目名(配置文件的组名)\n\
\n\
必选项[--checkname]的可选参数：\n\
	--checkcfg_only : 仅打印配置文件检查结果\n\
\n\
可选项：指定操作(任选0~n个)\n\
	--help : 打印帮助信息\n\
	--debug : 设置Debug等级(可设等级 : warn / info / debug / trace，后项均包含前项，例：info必然包含warn信息)\n\
		fatal : 致命错误(不能设置，已打开)\n\
		error : 程序异常错误(不能设置，已打开)\n\
		warn : 提示潜在问题，但不影响程序运行(默认项)\n\
		info : 记录程序运行状态信息\n\
		debug : 打印程序调试过程中的详细信息(打开后会影响程序执行速度，仅建议Debug时使用)\n\
		trace : 比debug更详细的调试信息(打开后会影响程序执行速度，仅建议Debug时使用)\n\
	--cfgfile : 指定配置文件";

static void usage(const char* const full_procname)
{
	cout << title << endl;
	const int offset_len = 7;
	const char* procname = strrchr(full_procname, '\\');
	if (procname == NULL)
		procname = full_procname;

	/* 本程序的特殊示例 */
	cout << "e.g.  :" << endl;
	cout << setw(offset_len) << ' ' << procname << " --checkname 3-b3                 : 按配置文件[3-b3]组的设定检查exe的运行结果" << endl;
	cout << setw(offset_len) << ' ' << procname << " --checkname 3-b3 --checkcfg_only : 检查配置文件[3-b3]组的设定是否正确" << endl;
	cout << setw(offset_len) << ' ' << procname << " --checkname 3-b3 --debug trace   : 按配置文件[3-b3]组的设定检查exe的运行结果，打印所有调试信息" << endl;
	cout << endl;
}

//获取当前时间字符串
static string get_current_time_str()
{
	time_t now = time(0);
	tm* ltm = localtime(&now);

	char buffer[80];

	strftime(buffer, 80, "%Y-%m-%d-%H-%M-%S", ltm);

	return string(buffer);
}
static void save_analysis_report(
	const string& filename,
	const vector<Student>& students,
	config_file_tools& cfg,
	const string& group_name_raw, // 原始组名，如 "3-b3"
	int items_num
)
{
	string group_name = "[" + group_name_raw + "]";
	ofstream out(filename);
	if (!out.is_open()) {
		cout << "[错误] 无法创建报表文件: " << filename << endl;
		return;
	}

	// 1. 写入配置区 (Header Config)
	string val_exe_style, val_cmd_style, val_name_list, val_stu_exe_name;
	cfg.item_get_raw(group_name, "exe_style", val_exe_style);
	cfg.item_get_raw(group_name, "cmd_style", val_cmd_style);
	cfg.item_get_raw(group_name, "name_list", val_name_list);
	cfg.item_get_string(group_name, "stu_exe_name", val_stu_exe_name);

	out << "exe_style\t" << val_exe_style << endl;
	out << "cmd_style\t" << val_cmd_style << endl;
	out << "name_list\t" << val_name_list << endl;
	out << "stu_exe_name\t" << val_stu_exe_name << endl;
	out << "start_time\t" << get_current_time_str() << endl; // 实时时间
	out << endl; // 空一行

	// 2. 写入表头 (Table Header)

	out << "序号\t"
		<< "课号\t"
		<< "学号\t"
		<< "姓名\t"
		<< "正确运行\t"
		<< "定时器创建失败\t"
		<< "管道方式打开失败\t"
		<< "启动定时器失败\t"
		<< "超时\t"
		<< "超过输出上限\t"
		<< "死循环\t"
		<< "TC通过总数\t";

	// 测试项表头
	for (int i = 1; i <= items_num; i++) 
	{
		string suffix = "_" + to_string(i);
		string item_name = "";

		if (val_cmd_style == "pipe") 
		{
			cfg.item_get_raw(group_name, "item_gname" + suffix, item_name);
			if (!item_name.empty()) 
			{
				if (item_name.front() != '[') item_name = "[" + item_name;
				if (item_name.back() != ']') item_name = item_name + "]";
			}
		}
		else if (val_cmd_style == "redirection") 
		{
			cfg.item_get_raw(group_name, "item_fname" + suffix, item_name);
		}
		else if (val_cmd_style == "main_with_arguments") 
		{
			cfg.item_get_raw(group_name, "item_args" + suffix, item_name);
		}

		if (item_name.empty()) 
		{

			string idx_str = (i < 10 ? "0" : "") + to_string(i);
			item_name = "[" + group_name_raw + "-" + idx_str + "]";
		}

		out << item_name << "\t";
	}
	out << endl; // 表头结束换行

	// 3. 写入学生数据
	for (size_t i = 0; i < students.size(); i++) 
	{
		const auto& s = students[i];

		// --- 基础信息
		out << (i + 1) << "\t";
		out << "=text(\"" << s.class_num << "\", \"#\")\t";   // Excel公式保护长数字
		out << s.student_num << "\t"; 
		out << s.name << "\t";

		// --- 检查 exe 是否存在 ---
		if (!s.exe_exists) 
		{

			for (int k = 0; k < 7; k++) out << "/\t";

			for (int k = 0; k <= items_num; k++) out << "0\t";
		}
		else 
		{
			out << s.cnt_ok << "\t";
			out << s.cnt_timer_create_fail << "\t";
			out << s.cnt_pipe_fail << "\t";
			out << s.cnt_timer_start_fail << "\t";
			out << s.cnt_timeout << "\t";
			out << s.cnt_max_output << "\t";
			out << s.cnt_deadloop << "\t";
			out << s.cnt_tc_pass << "\t";

			// 输出详细测试点
			for (int res : s.item_results) 
			{
				out << res << "\t";
			}
		}

		out << endl; // 换行，开始下一个学生
	}
	out << endl;
	out.close();
	cout << "----------------------------------------------------------" << endl;
	cout << "分析报表已生成: " << filename << endl;
	cout << "----------------------------------------------------------" << endl;
}
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
int main(int argc, char** argv)
{
	const string loglevel_define[] = { "warn", "info", "debug", "trace", "" };

	args_analyse_tools args[] = {
		args_analyse_tools("--help",		   ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--debug",		   ST_EXTARGS_TYPE::str_with_set_default, 1, 0, loglevel_define),
		args_analyse_tools("--checkname",	   ST_EXTARGS_TYPE::str, 1, string("")),
		args_analyse_tools("--checkcfg_only", ST_EXTARGS_TYPE::boolean, 0, false),
		args_analyse_tools("--cfgfile",	   ST_EXTARGS_TYPE::str, 1, string("hw_check_exe.cfg")),
		args_analyse_tools()  //最后一个，用于结束
	};
	int cur_argc, ret = 0;

	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	/* 对help做特殊处理 */
	if (args[OPT_ARGS_HELP].existed()) {
		//只要有 --help，其它参数都忽略，显示帮助即可
		args_analyse_print(args);
		usage(argv[0]);
		return -1; //执行完成直接退出
	}

	//读取配置文件
	string config_name = args[OPT_ARGS_CFGFILE].existed() ? args[OPT_ARGS_CFGFILE].get_string() : "hw_check_exe.cfg";
	config_file_tools cfg(config_name);
	if (args[OPT_ARGS_CHECKCFGONLY].existed())
	{
		string temp = args[OPT_ARGS_CHECKNAME].get_string();
		string Group_name = "[" + temp + "]";
		cfg.print_item(Group_name.c_str(),true,false);
		return 0;
	}

	string temp = args[OPT_ARGS_CHECKNAME].get_string();
	string Group_name = "[" + temp + "]";
	if (cfg.print_item(Group_name.c_str(), true, true)!=0)
	{
		cout << endl;
		cerr << "[--严重错误--] 配置文件存在下列的错误：" << endl;
		return -1;
	}
	Student_Info S_F;

	int count = S_F.get_student_list(args[OPT_ARGS_CHECKNAME].get_string().c_str(), cfg);

	exe_path path;
	path.make_path(count, S_F.get_list_writable(), cfg, args[OPT_ARGS_CHECKNAME].get_string().c_str());

	//得到的是最初版本拼接好的路径
	vector<string> inital_path = path.get_path();


	string check_name_raw = args[OPT_ARGS_CHECKNAME].get_string();
	string group_name = "[" + check_name_raw + "]";

	string cmd_style = "";
	cfg.item_get_raw(group_name, "cmd_style", cmd_style);

	string stu_exe_suffix = path.get_stu_exe_name(); // 获取后缀，仅用于打印或逻辑参考

	// 2. 获取限制参数
	string max_time_str = "3";
	string max_output_str = "1024";
	cfg.item_get_raw(group_name, "timeout", max_time_str);
	cfg.item_get_raw(group_name, "max_output_len", max_output_str);

	// 3. 获取测试项数量
	string items_num_str = "1";
	cfg.item_get_raw(group_name, "items_num", items_num_str);

	int timeout = 3; // 默认值
	try {
		if (!max_time_str.empty()) timeout = stoi(max_time_str);
	}
	catch (...) {
		cout << "[警告] timeout 配置格式错误 (" << max_time_str << ")，将使用默认值 3" << endl;
		timeout = 3;
	}

	int max_out = 1024; // 默认值
	try {
		if (!max_output_str.empty()) max_out = stoi(max_output_str);
	}
	catch (...) {
		cout << "[警告] max_output_len 配置格式错误 (" << max_output_str << ")，将使用默认值 1024" << endl;
		max_out = 1024;
	}

	int items_num = 1; // 默认值
	try {
		if (!items_num_str.empty()) items_num = stoi(items_num_str);
	}
	catch (...) {
		cout << "[警告] items_num 配置格式错误 (" << items_num_str << ")，将使用默认值 1" << endl;
		items_num = 1;
	}

	//获取tc比较器的参数
	string tc_trim_type = "right";       // 默认值
	string tc_display_type = "none";     // 默认值
	int tc_lineskip = 0;
	int tc_lineoffset = 0;
	int tc_maxdiff = 10;
	int tc_maxline = 1000;
	bool tc_ignoreblank = false;          // 默认false
	bool tc_not_ignore_linefeed = false; // 默认 false (即忽略换行符差异)

	// 临时字符串用于转换
	string temp_val = "";

	// 1. 读取字符串类型的配置
	cfg.item_get_raw(group_name, "tc_trim", tc_trim_type);
	cfg.item_get_raw(group_name, "tc_display", tc_display_type);

	// 2. 读取整数类型的配置
	if (cfg.item_get_raw(group_name, "tc_lineskip", temp_val) == 1 && !temp_val.empty())
		tc_lineskip = stoi(temp_val);

	if (cfg.item_get_raw(group_name, "tc_lineoffset", temp_val) == 1 && !temp_val.empty())
		tc_lineoffset = stoi(temp_val);

	if (cfg.item_get_raw(group_name, "tc_maxdiff", temp_val) == 1 && !temp_val.empty())
		tc_maxdiff = stoi(temp_val);

	if (cfg.item_get_raw(group_name, "tc_maxline", temp_val) == 1 && !temp_val.empty())
		tc_maxline = stoi(temp_val);

	// 3. 读取布尔类型的配置 (配置文件通常用 0/1 或 true/false)
	if (cfg.item_get_raw(group_name, "tc_ignoreblank", temp_val) == 1 && !temp_val.empty())
		tc_ignoreblank = (temp_val == "1" || temp_val == "true");

	if (cfg.item_get_raw(group_name, "tc_not_ignore_linefeed", temp_val) == 1 && !temp_val.empty())
		tc_not_ignore_linefeed = (temp_val == "1" || temp_val == "true");

	//先生成demo的正确答案

	vector<string> std_answers; // 用于存储每个测试点的标准输出
	string demo_exe_path = path.get_demo_path(); // 确保你的 exe_path 类有这个接口

	string demo_exe_name = demo_exe_path;
	size_t last_slash_idx = demo_exe_path.find_last_of("\\/"); // 同时查找 \ 和 /

	if (last_slash_idx != string::npos) 
	{
		demo_exe_name = demo_exe_path.substr(last_slash_idx + 1);
	}

	cout << "正在生成标准参考答案..." << endl;

	for (int i = 1; i <= items_num; i++)
	{
		string full_cmd = "";
		string item_key_suffix = "_" + to_string(i);

		if (cmd_style == "pipe")
		{
			string gname = "";
			cfg.item_get_raw(group_name, "item_gname" + item_key_suffix, gname);
			if (!gname.empty()) 
			{
				if (gname.front() != '[') gname = "[" + gname;
				if (gname.back() != ']') gname = gname + "]";
			}
			full_cmd = path.get_pipe_get_input() + " "
				+ path.get_pipe_data_file() + " "
				+ gname + " | "
				+ demo_exe_path;
		}
		else if (cmd_style == "redirection")
		{
			string fname = "";
			cfg.item_get_raw(group_name, "item_fname" + item_key_suffix, fname);
			if (fname.empty()) 
			{
				std_answers.push_back(""); // 占位，防止索引错位
				cout << "  [Demo警告] 测试点 " << i << " 配置缺失，跳过。" << endl;
				continue;
			}
			string redir_dir = path.get_redirection_data_dirname();
			if (!redir_dir.empty() && redir_dir.back() != '\\') redir_dir += "\\";

			// Demo程序 < 输入文件
			full_cmd = demo_exe_path + " < " + redir_dir + fname;
		}
		else if (cmd_style == "main_with_arguments")
		{
			string args_val = "";
			cfg.item_get_raw(group_name, "item_args" + item_key_suffix, args_val);
			// Demo程序 + 参数
			full_cmd = demo_exe_path + " " + args_val;
		}
		else // normal
		{
			full_cmd = demo_exe_path;
		}

		st_CheckExec demo_run(full_cmd, demo_exe_name, max_out, timeout);
		demo_run.running();

		if (demo_run.get_errno() == CheckExec_Errno::ok) 
		{

			std_answers.push_back(demo_run.msg.str());
		}
		else 
		{
			std_answers.push_back(""); // 失败则存空字符串，或做特殊标记
			cout << "  [Demo错误] 测试点 " << i << " 运行失败: " << demo_run.get_errno() << endl;
		}
	}
	cout << "标准答案生成完毕，共 " << std_answers.size() << " 个。" << endl;
	cout << "============================================================================================================" << endl;

	cout << "============================================================================================================" << endl;
	cout << "检查模式: " << cmd_style << " | 测试项数: " << items_num << " | 超时: " << timeout << "s" << endl;
	cout << "============================================================================================================" << endl;

	cout << "正在检查学生作业..." << endl;

	vector<Student>& students = S_F.get_list_writable();//返回一个可写的接口

	for (size_t s_idx = 0; s_idx < inital_path.size(); s_idx++)
	{
		string s_path = inital_path[s_idx];

		Student& cur_stu = students[s_idx];
		cur_stu.item_results.clear();

		cur_stu.item_results.reserve(items_num);//初始化学生的测试点数组

		//在该学生exe文件路径不存在时，直接跳过检查
		if (!cur_stu.exe_exists)
		{
			for (int k = 0; k < items_num; k++) 
			{
				cur_stu.item_results.push_back(0);
			}

			cout << "[Student " << left << setw(2) << s_idx + 1 << "] ";
			cout << cur_stu.name;

			cout << "未找到可执行文件 (跳过检查)";
			cout << "  -> " << s_path << endl << endl;

			continue;
		}
		// 内层循环：遍历测试点
		for (int i = 1; i <= items_num; i++)
		{
			string full_cmd = "";
			string item_key_suffix = "_" + to_string(i);

			if (cmd_style == "pipe")
			{
				string gname = "";
				cfg.item_get_raw(group_name, "item_gname" + item_key_suffix, gname);
				if (!gname.empty()) {
					if (gname.front() != '[') gname = "[" + gname;
					if (gname.back() != ']') gname = gname + "]";
				}
				full_cmd = path.get_pipe_get_input() + " " + path.get_pipe_data_file() + " " + gname + " | " + s_path;
			}
			else if (cmd_style == "redirection")
			{
				string fname = "";
				cfg.item_get_raw(group_name, "item_fname" + item_key_suffix, fname);
				if (fname.empty()) 
				{
					//fail_count++;
					continue;
				}
				string redir_dir = path.get_redirection_data_dirname();
				if (!redir_dir.empty() && redir_dir.back() != '\\') redir_dir += "\\";
				full_cmd = s_path + " < " + redir_dir + fname;
			}
			else if (cmd_style == "main_with_arguments")
			{
				string args_val = "";
				cfg.item_get_raw(group_name, "item_args" + item_key_suffix, args_val);
				full_cmd = s_path + " " + args_val;
			}
			else
			{
				full_cmd = s_path;
			}

			// 2. 运行学生程序
			st_CheckExec stu_run(full_cmd, stu_exe_suffix, max_out, timeout);
			stu_run.running();


			CheckExec_Errno err = stu_run.get_errno();

			bool is_pass = false;
			if (err == CheckExec_Errno::ok)
			{
				// --- 情况1：程序正常运行结束 ---
				cur_stu.cnt_ok++;

				// 开始比对答案
				string std_str = std_answers[i - 1];
				istringstream ss_std(std_str);

				string stu_str = stu_run.msg.str();
				istringstream ss_stu(stu_str);

				// 实例化比较器 (参数：忽略空格，不显示详细)
				txt_compare tc(
					"", "",                // 文件名置空
					tc_trim_type,          // tc_trim
					tc_display_type,       // tc_display
					tc_lineskip,           // tc_lineskip
					tc_lineoffset,         // tc_lineoffset
					tc_maxdiff,            // tc_maxdiff
					tc_maxline,            // tc_maxline
					tc_ignoreblank,        // tc_ignoreblank
					tc_not_ignore_linefeed,// tc_not_ignore_linefeed
					false                  // debug
				);

				int diff_count = tc.compare(ss_std, ss_stu);

				if (diff_count == 0)
				{
					// 答案正确
					is_pass = true;
					cur_stu.cnt_tc_pass++;
				}
				else
				{
					// 答案错误 (WA)
					is_pass = false;
					//fail_count++;
				}
			}
			else if (err == CheckExec_Errno::timeout)
			{
				// --- 情况2：超时 ---
				cur_stu.cnt_timeout++;
				is_pass = false;
				//fail_count++;
			}
			else if (err == CheckExec_Errno::popen_faliled)
			{
				// --- 情况3：管道打开失败 ---
				cur_stu.cnt_pipe_fail++;
				is_pass = false;
				//fail_count++;
			}
			else if (err == CheckExec_Errno::create_timer_id_failed)
			{
				// --- 情况4：创建定时器失败 ---
				cur_stu.cnt_timer_create_fail++;
				is_pass = false;
				//fail_count++;
			}
			else if (err == CheckExec_Errno::start_timer_failed)
			{
				// --- 情况5：启动定时器失败 ---
				cur_stu.cnt_timer_start_fail++;
				is_pass = false;
				//fail_count++;
			}
			else if (err == CheckExec_Errno::max_output)
			{
				// --- 情况6：超过输出上限 ---
				cur_stu.cnt_max_output++;
				is_pass = false;
				//fail_count++;
			}
			else if (err == CheckExec_Errno::killed_by_callback)
			{
				// --- 情况7：死循环 (被强制杀死) ---
				cur_stu.cnt_deadloop++;
				is_pass = false;
				//fail_count++;
			}
			else
			{
				// --- 其他未知错误 ---
				is_pass = false;
				//fail_count++;
			}
			cur_stu.item_results.push_back(is_pass ? 1 : 0);
		}

		cout << "[Student " << left << setw(2) << s_idx + 1 << "] ";

		cout << cur_stu.name;
		for (int i : cur_stu.item_results)
		{
			cout << i << " ";
		}
		cout << endl;
		cout << "   [Debug Info] "
			<< "SUC:" << cur_stu.cnt_ok << " | "
			<< "AC:" << cur_stu.cnt_tc_pass << " | "
			<< "TO:" << cur_stu.cnt_timeout << " | "
			<< "Pipe:" << cur_stu.cnt_pipe_fail << " | "
			<< "Max:" << cur_stu.cnt_max_output << " | "
			<< "Loop:" << cur_stu.cnt_deadloop
			<< endl;
		cout << "   [System Err] "
			<< "TimerCreate:" << cur_stu.cnt_timer_create_fail << " | "
			<< "TimerStart:" << cur_stu.cnt_timer_start_fail
			<< endl;

		cout << endl;
	}
	string timestamp = get_current_time_str();

	string val_exe_style = "";
	cfg.item_get_raw(group_name, "exe_style", val_exe_style);
	string val_name_list = "";
	cfg.item_get_raw(group_name, "name_list", val_name_list);
	string namelist_tag = (val_name_list == "database") ? "database" : "txt";
	string report_filename = "check-result-2453359-" +
		timestamp + "-" +
		val_exe_style + "-" +
		cmd_style + "-" +
		namelist_tag + "-" +
		stu_exe_suffix + ".xls";

	save_analysis_report(
		report_filename,
		students,          // 传入学生列表
		cfg,               // 传入配置工具
		args[OPT_ARGS_CHECKNAME].get_string(), // 传入组名 (如 "3-b3")
		items_num          // 传入测试点总数
	);
	return 0;
}
