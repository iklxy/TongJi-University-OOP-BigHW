/*2453359 李欣洋 信安*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstring>
#include "../include/class_aat.h"
//如有必要，可以加入其它头文件
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //不使用lib才有效

/* ---------------------------------------------------------------
	 允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */
static string format_double(double x)
{
	stringstream ss;
	ss << fixed << setprecision(6) << x;
	return ss.str();
}

static bool is_strict_int(const char* s)
{
	if (s == NULL || *s == '\0')
		return false;

	int len = strlen(s);
	int start = 0;
	bool negative = false;

	// ---- 判断符号 ----
	if (s[0] == '+' || s[0] == '-')
	{
		if (len == 1) return false; // "+" 或 "-" 不合法

		if (s[0] == '-') negative = true;
		start = 1;
	}

	// ---- 检查是否全为数字 ----
	for (int i = start; i < len; i++)
	{
		if (s[i] < '0' || s[i] > '9')
			return false;
	}

	// ---- 提取数字部分 ----
	const char* digits = s + start;
	int digit_len = len - start;

	// ---- 跳过前导零 ----
	int pos = 0;
	while (pos < digit_len && digits[pos] == '0')
		pos++;

	// 全部为 0，例如 "0000"
	if (pos == digit_len)
		return true;

	// ---- 去掉前导零后的有效长度 ----
	int effective_len = digit_len - pos;
	const char* eff_digits = digits + pos;

	// ---- 判断是否可能溢出 ----
	if (effective_len > 10)
		return false;

	if (effective_len == 10)
	{
		if (negative)
		{
			// INT_MIN = -2147483648
			if (strcmp(eff_digits, "2147483648") > 0)
				return false;
		}
		else
		{
			// INT_MAX = 2147483647
			if (strcmp(eff_digits, "2147483647") > 0)
				return false;
		}
	}

	return true;
}


static bool is_strict_double(const char* s)
{
	if (s == NULL || *s == '\0')
		return false;

	int len = strlen(s);
	int start = 0;
	bool negative = false;

	// ---- 处理符号 ----
	if (s[0] == '+' || s[0] == '-')
	{
		if (len == 1) return false; // "+" 或 "-" 不合法

		if (s[0] == '-')
			negative = true;

		start = 1;
	}

	if (start >= len)
		return false;

	int dot_pos = -1;

	// ---- 扫描一次，记录小数点，并检查数字是否合法 ----
	for (int i = start; i < len; i++)
	{
		if (s[i] == '.')
		{
			if (dot_pos != -1) // 多个小数点非法
				return false;
			dot_pos = i;
		}
		else if (s[i] < '0' || s[i] > '9')
		{
			return false; // 非数字非法
		}
	}


	return true;
}

/***************************************************************************
  函数名称：空构造函数，在打印输出时遇到这个名字即终止
  功    能：
  输入参数：
  返 回 值：
  说    明：null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	extargs_int_set = nullptr;
	extargs_double_set = nullptr;
	extargs_string_set = nullptr;

	args_name = "<NULL>";
	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	extargs_int_set = nullptr;
	extargs_double_set = nullptr;
	extargs_string_set = nullptr;

	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_bool_default = def;

	args_existed = 0;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_default、int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	extargs_int_set = nullptr;
	extargs_double_set = nullptr;
	extargs_string_set = nullptr;

	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_int_default = def;
	extargs_int_min = _min;
	extargs_int_max = _max;

	args_existed = 0;
	extargs_int_value = extargs_int_default;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_set_default、int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	extargs_double_set = nullptr;
	extargs_string_set = nullptr;


	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	
	//不存在由const int指针转换为int指针 需要在内部进行数组的复制
	int count = 0;
	while (set[count] != INVALID_INT_VALUE_OF_SET)
		count++;
	extargs_int_set = new(nothrow) int[count + 1];
	if (extargs_int_set == NULL)
		return;
	for (int i = 0; i < count; i++)
		extargs_int_set[i] = set[i];
	extargs_int_set[count] = INVALID_INT_VALUE_OF_SET;
	extargs_int_default = extargs_int_set[def_of_set_pos];

	args_existed = 0;
	extargs_int_value = extargs_int_default;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str、ipaddr_with_default、ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	extargs_int_set = nullptr;
	extargs_double_set = nullptr;
	extargs_string_set = nullptr;

	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	if (type == ST_EXTARGS_TYPE::str)
	{
		extargs_string_default = def;
	}
	else
	{
		if (def != "")
		{
			extargs_string_default = def;
		}
		else
		{
			extargs_string_default = "0.0.0.0";
		}
	}
	
	if (type == ST_EXTARGS_TYPE::ipaddr_with_default || type == ST_EXTARGS_TYPE::ipaddr_with_error)
	{
		stringstream ss(extargs_string_default);
		int a, b, c, d;
		char dot;

		ss >> a >> dot >> b >> dot >> c >> dot >> d;
		extargs_ipaddr_default = ipv4ToUint(a, b, c, d);
	}
	extargs_ipaddr_value = extargs_ipaddr_default;
	args_existed = 0;
	extargs_string_value = extargs_string_default;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str_with_set_default、str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	extargs_int_set = nullptr;
	extargs_double_set = nullptr;

	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	//不存在由const string指针转换为string指针 需要在内部进行数组的复制
	int count = 0;
	while (set[count] != "")
		count++;
	extargs_string_set = new(nothrow) string[count + 1];
	if (extargs_string_set == NULL)
		return;
	for (int i = 0; i < count; i++)
		extargs_string_set[i] = set[i];
	extargs_string_set[count] = "";
	extargs_string_default = extargs_string_set[def_of_set_pos];

	args_existed = 0;
	extargs_string_value = extargs_string_default;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：double_with_default、double_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const double	def, const double _min, const double _max)
{
	extargs_int_set = nullptr;
	extargs_double_set = nullptr;
	extargs_string_set = nullptr;

	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	extargs_double_default = def;
	extargs_double_min = _min;
	extargs_double_max = _max;

	args_existed = 0;
	extargs_double_value = extargs_double_default;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：double_with_set_default、double_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const double* const set)
{
	extargs_int_set = nullptr;
	extargs_string_set = nullptr;

	args_name = name;
	extargs_type = type;
	extargs_num = ext_num;
	//不存在由const string指针转换为string指针 需要在内部进行数组的复制
	int count = 0;
	while (set[count] != INVALID_DOUBLE_VALUE_OF_SET)
		count++;
	extargs_double_set = new(nothrow) double[count + 1];
	if (extargs_double_set == NULL)
		return;
	for (int i = 0; i < count; i++)
		extargs_double_set[i] = set[i];
	extargs_double_set[count] = INVALID_DOUBLE_VALUE_OF_SET;
	extargs_double_default = extargs_double_set[def_of_set_pos];

	args_existed = 0;
	extargs_double_value = extargs_double_default;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
	delete[]extargs_int_set;
	delete[]extargs_double_set;
	delete[]extargs_string_set;
}

/* ---------------------------------------------------------------
	 允许AAT中自定义成员函数的实现（private）
   ---------------------------------------------------------------- */
unsigned int args_analyse_tools::ipv4ToUint(int a, int b, int c, int d)
{
	unsigned int ip =
		((unsigned int)a << 24) |
		((unsigned int)b << 16) |
		((unsigned int)c << 8) |
		(unsigned int)d;

	return ip;
}

bool args_analyse_tools::is_valid_ipv4(const string& ip)
{
	int n = ip.length();
	if (n == 0) return false;

	int dot_count = 0;
	string part;

	for (int i = 0; i <= n; i++)
	{
		if (i == n || ip[i] == '.')
		{
			if (part.empty()) return false;

			for (int k = 0; k < (int)part.size(); k++)
				if (part[k] < '0' || part[k] > '9')
					return false;

			int value = stoi(part);

			if (value < 0 || value > 255)
				return false;

			part.clear();
			dot_count++;

			continue;
		}

		part += ip[i];
	}

	if (dot_count != 4)
		return false;

	return true;
}

const char* args_analyse_tools::to_string() const
{
	ST_EXTARGS_TYPE t = this->extargs_type;
	switch (t)
	{
		case ST_EXTARGS_TYPE::null: return "null";
		case ST_EXTARGS_TYPE::boolean: return "Bool";
		case ST_EXTARGS_TYPE::int_with_default: return "IntWithDefault";
		case ST_EXTARGS_TYPE::int_with_error: return "IntWithError";
		case ST_EXTARGS_TYPE::int_with_set_default: return "IntSETWithDefault";
		case ST_EXTARGS_TYPE::int_with_set_error: return "IntSETWithError";
		case ST_EXTARGS_TYPE::double_with_default: return "DoubleWithDefault";
		case ST_EXTARGS_TYPE::double_with_error: return "DoubleWithError";
		case ST_EXTARGS_TYPE::double_with_set_default: return "DoubleSETWithDefault";
		case ST_EXTARGS_TYPE::double_with_set_error: return "DoubleSETWithError";
		case ST_EXTARGS_TYPE::str: return "String";
		case ST_EXTARGS_TYPE::str_with_set_default: return "StringSETWithDefault";
		case ST_EXTARGS_TYPE::str_with_set_error: return "StringSETWithError";
		case ST_EXTARGS_TYPE::ipaddr_with_default: return "IPAddrWithDefault";
		case ST_EXTARGS_TYPE::ipaddr_with_error: return "IPAddrWithError";
		case ST_EXTARGS_TYPE::tmax: return "tmax";
	}

	return "unknown";
}

 const string args_analyse_tools::get_range()const
{
	 string range("");
	 if (this->extargs_type == ST_EXTARGS_TYPE::int_with_default || this->extargs_type == ST_EXTARGS_TYPE::int_with_error
		 || this->extargs_type == ST_EXTARGS_TYPE::double_with_default || this->extargs_type == ST_EXTARGS_TYPE::double_with_error)
	 {
		 range += "[";
		 if (this->extargs_type == ST_EXTARGS_TYPE::int_with_default || this->extargs_type == ST_EXTARGS_TYPE::int_with_error)
		 {
			 stringstream ss;
			 ss << this->extargs_int_min;
			 string temp = ss.str();
			 range += temp;
		 }
		 else if (this->extargs_type == ST_EXTARGS_TYPE::double_with_default || this->extargs_type == ST_EXTARGS_TYPE::double_with_error)
		 {
			 stringstream ss;
			 ss << fixed << setprecision(6) << this->extargs_double_min;
			 string temp = ss.str();
			 range += temp;
		 }
		 range += "..";
		 if (this->extargs_type == ST_EXTARGS_TYPE::int_with_default || this->extargs_type == ST_EXTARGS_TYPE::int_with_error)
		 {
			 stringstream ss;
			 ss << this->extargs_int_max;
			 string temp = ss.str();
			 range += temp;
		 }
		 else if (this->extargs_type == ST_EXTARGS_TYPE::double_with_default || this->extargs_type == ST_EXTARGS_TYPE::double_with_error)
		 {
			 stringstream ss;
			 ss << fixed << setprecision(6) << this->extargs_double_max;
			 string temp = ss.str();
			 range += temp;
		 }
		 range += "]";
	 }
	 else if (this->extargs_type == ST_EXTARGS_TYPE::int_with_set_default || this->extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
	 {
		 int count_setvalue = 0;
		 while (this->extargs_int_set[count_setvalue] != INVALID_INT_VALUE_OF_SET)
			 count_setvalue++;
		 for (int i = 0; i < count_setvalue; i++)
		 {
			 stringstream ss;
			 ss << this->extargs_int_set[i];
			 string temp = ss.str();
			 range += temp;
			 if (i != count_setvalue - 1)
				 range += "/";
		 }
	 }
	 else if (this->extargs_type == ST_EXTARGS_TYPE::double_with_set_default || this->extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
	 {
		 int count_setvalue = 0;
		 while (this->extargs_double_set[count_setvalue] != INVALID_DOUBLE_VALUE_OF_SET)
			 count_setvalue++;
		 for (int i = 0; i < count_setvalue; i++)
		 {
			 stringstream ss;
			 ss << fixed << setprecision(6) << this->extargs_double_set[i];
			 string temp = ss.str();
			 range += temp;
			 if (i != count_setvalue - 1)
				 range += "/";
		 }
	 }
	 else if (this->extargs_type == ST_EXTARGS_TYPE::str_with_set_default || this->extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
	 {
		 int count_setvalue = 0;
		 while (this->extargs_string_set[count_setvalue] != "")
			 count_setvalue++;
		 for (int i = 0; i < count_setvalue; i++)
		 {
			 range += extargs_string_set[i];
			 if (i != count_setvalue - 1)
				 range += "/";
		 }
	 }
	 if (range == "")
		 return "/";
	 else
		 return range;
}

 const string args_analyse_tools::get_range_double()const
 {
	 string range("");
	 if (this->extargs_type == ST_EXTARGS_TYPE::double_with_default || this->extargs_type == ST_EXTARGS_TYPE::double_with_error)
	 {
		 range += "[";
		 if (this->extargs_type == ST_EXTARGS_TYPE::double_with_default || this->extargs_type == ST_EXTARGS_TYPE::double_with_error)
		 {
			 stringstream ss;
			 ss << this->extargs_double_min;
			 string temp = ss.str();
			 range += temp;
		 }
		 range += "..";
		 if (this->extargs_type == ST_EXTARGS_TYPE::double_with_default || this->extargs_type == ST_EXTARGS_TYPE::double_with_error)
		 {
			 stringstream ss;
			 ss << this->extargs_double_max;
			 string temp = ss.str();
			 range += temp;
		 }
		 range += "]";
	 }
	 else if (this->extargs_type == ST_EXTARGS_TYPE::double_with_set_default || this->extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
	 {
		 int count_setvalue = 0;
		 while (this->extargs_double_set[count_setvalue] != INVALID_DOUBLE_VALUE_OF_SET)
			 count_setvalue++;
		 for (int i = 0; i < count_setvalue; i++)
		 {
			 stringstream ss;
			 ss << this->extargs_double_set[i];
			 string temp = ss.str();
			 range += temp;
			 if (i != count_setvalue - 1)
				 range += "/";
		 }
	 }
	 if (range == "")
		 return "/";
	 else
		 return range;
 }
/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return this->args_name;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：加!!后，只能是0/1
			已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!this->args_existed;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return this->extargs_int_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const double args_analyse_tools::get_double() const
{
	return this->extargs_double_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return this->extargs_string_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return this->extargs_ipaddr_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将 extargs_ipaddr_value 的值从 0x7f000001 转为 "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	return this->extargs_string_value; //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：follow_up_args：是否有后续参数
			0  ：无后续参数
			1  ：有后续参数
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_process(const int argc, const char* const *const argv, args_analyse_tools* const args, const int follow_up_args)
{
	int cmd_count = 1;

	int count_arg = 0;
	while (args[count_arg].args_name != "<NULL>")
		count_arg++;

	for (int i = 1; i < argc;)
	{
		//先判断参数格式是否合法
		string cur(argv[i]);
		if (cur.rfind("--", 0) != 0)
		{
			if (follow_up_args == 0)
			{
				cout << "参数[" << argv[i] << "]格式非法(不是--开头的有效内容)." << endl;
				return -1;
			}
			else if (follow_up_args == 1)
			{
				return cmd_count;
			}
		}
		for (int j = 0; j < count_arg; j++)
		{
			if (cur == args[j].args_name)
			{
				string next_;
				if(i+1 < argc)
					next_=argv[i + 1];

				bool matched = false;
				for (int j2 = 0; j2 < count_arg; j2++)
				{
					if (next_ == args[j2].args_name)
					{
						matched = true;
						break;
					}
				}
				if (matched && args[j].extargs_type != ST_EXTARGS_TYPE::boolean)
				{
					cout << "参数[" << cur << "]缺少附加参数. (类型:";
					if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error
						|| args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
					{
						cout << "int, ";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)
						{
							cout << "范围" << args[j].get_range();
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
						{
							cout << " 可取值[" << args[j].get_range() << "]";
						}
						if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
						{
							cout << " 缺省:" << args[j].extargs_int_default;
						}
						cout << ")" << endl;
					}
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error
						|| args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
					{
						cout << "double, ";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error)
						{
							cout << "范围" << args[j].get_range_double();
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
						{
							cout << " 可取值[" << args[j].get_range_double() << "]";
						}
						if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default)
						{
							cout << " 缺省:" << args[j].extargs_double_default;
						}
						cout << ")" << endl;
					}
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::str || args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default ||
						args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
					{
						cout << "string";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
						{
							cout << ", 可取值[" << args[j].get_range() << "]";
						}
						if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[j].args_name == "--str2")
						{
							cout << " 缺省:" << args[j].extargs_string_default;
						}
						cout << ")" << endl;
					}
					else
					{
						cout << "IP地址";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
						{
							cout << " 缺省:" << args[j].extargs_string_value;
						}
						cout << ")" << endl;
					}
					return -1;
				}
				//判断参数是否重复
				if (args[j].args_existed == 1)
				{
					cout << "参数[" << cur << "]重复." << endl;
					return -1;
				}
				//help和bool参数
				if (args[j].extargs_type == ST_EXTARGS_TYPE::boolean)
				{
					args[j].args_existed = 1;
					i += 1;
					cmd_count += 1;
					break;
				}
				//除help和bool以外的参数需要后续后续的额外参数 需要判断额外参数是否存在
				if (i + 1 >= argc)
				{
					cout << "参数[" << cur << "]的附加参数不足. (类型:";
					if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error
						|| args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
					{
						cout << "int, ";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)
						{
							cout << "范围" << args[j].get_range();
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
						{
							cout << " 可取值[" << args[j].get_range() << "]";
						}
						if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
						{
							cout << " 缺省:" << args[j].extargs_int_default;
						}
						cout << ")" << endl;
					}
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error
						|| args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
					{
						cout << "double, ";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error)
						{
							cout << "范围" << args[j].get_range_double();
						}
						else if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
						{
							cout << " 可取值[" << args[j].get_range_double() << "]";
						}
						if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default)
						{
							cout << " 缺省:" << args[j].extargs_double_default;
						}
						cout << ")" << endl;
					}
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::str|| args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || 
						args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
					{
						cout << "string";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
						{
							cout << ", 可取值[" << args[j].get_range() << "]";
						}
						if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[j].args_name == "--str2")
						{
							cout << " 缺省:" << args[j].extargs_string_default;
						}
						cout << ")" << endl;
					}
					else
					{
						cout << "IP地址";
						if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
						{
							cout << " 缺省:" << args[j].extargs_string_value;
						}
						cout << ")" << endl;
					}
					return -1;
				}
				//intdef和interr参数
				if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)
				{
					args[j].args_existed = 1;
					if (!is_strict_int(argv[i + 1]) && args[j].extargs_type == ST_EXTARGS_TYPE::int_with_error)
					{
						cout << "参数[" << cur << "]的附加参数不是整数. (类型:int, 范围" << args[j].get_range() << ")" << endl;
						return -1;
					}
					else if (!is_strict_int(argv[i + 1]) && args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default)
					{
						cout << "参数[" << cur << "]的附加参数不是整数. (类型:int, 范围" << args[j].get_range() <<" 缺省:"<< args[j].extargs_int_default<< ")" << endl;
						return -1;
					}
					int value = 0;
					if (is_strict_int(argv[i + 1])==true)
						value = stoi(argv[i + 1]);
					else if((int)is_strict_int(argv[i + 1]) == 2)
						value = args[j].extargs_int_default;
					if (value >= args[j].extargs_int_min && value <= args[j].extargs_int_max)
					{
						args[j].extargs_int_value = value;
					}
					else
					{
						if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_default)
							args[j].extargs_int_value = args[j].extargs_int_default;
						else
						{
							cout << "参数[" << cur << "]的附加参数值(" << value << ")非法. (类型:int, 范围" << args[j].get_range()<< ")" << endl;
							return -1;
						}
					}
					i += 2;
					cmd_count += 2;
					break;
				}
				//intsetdef和intseterr参数
				if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
				{
					args[j].args_existed = 1;
					if (!is_strict_int(argv[i + 1]) && args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
					{
						cout << "参数[" << cur << "]的附加参数不是整数. (类型:int, 范围[" << args[j].get_range()<< "])" << endl;
						return -1;
					}
					else if (!is_strict_int(argv[i + 1]) && args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
					{
						cout << "参数[" << cur << "]的附加参数不是整数. (类型:int, 范围" << args[j].get_range() << " 缺省:" << args[j].extargs_int_default << ")" << endl;
						return -1;
					}
					int value = 0;
					if (is_strict_int(argv[i + 1])==true)
						value = stoi(argv[i + 1]);
					else if ((int)is_strict_int(argv[i + 1]) == 2)
						value = args[j].extargs_int_default;

					if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_default)
						args[j].extargs_int_value = args[j].extargs_int_default;
					else if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
						args[j].extargs_int_value = -1;
					for (int k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
					{
						if (value == args[j].extargs_int_set[k])
						{
							args[j].extargs_int_value = value;
							break;
						}
					}
					if (args[j].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
					{
						bool jud = false;
						for (int k = 0; args[j].extargs_int_set[k] != INVALID_INT_VALUE_OF_SET; k++)
						{
							if (value == args[j].extargs_int_set[k])
							{
								jud = true;
								break;
							}
						}
						if (!jud)
						{
							cout << "参数[" << cur << "]的附加参数值(" << value << ")非法. (类型:int, 可取值["<<args[j].get_range()<<"])" << endl;
							return -1;
						}
					}
					i += 2;
					cmd_count += 2;
					break;
				}
				//doubledef和doubleerr参数
				if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error)
				{
					args[j].args_existed = 1;
					if (!is_strict_double(argv[i + 1]) && args[j].extargs_type == ST_EXTARGS_TYPE::double_with_error)
					{
						cout << "参数[" << cur << "]的附加参数不是浮点数. (类型:double, 范围" << args[j].get_range_double()  << ")" << endl;
						return -1;
					}
					else if (!is_strict_double(argv[i + 1]) && args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default)
					{
						cout << "参数[" << cur << "]的附加参数不是浮点数. (类型:double, 范围" << args[j].get_range() << " 缺省:" << args[j].extargs_double_default << ")" << endl;
						return -1;
					}
					double value = 0;
					if (is_strict_double(argv[i + 1]))
						value = stod(argv[i + 1]);
					else
						value = args[j].extargs_int_default;
					if (value >= args[j].extargs_double_min && value <= args[j].extargs_double_max)
					{
						args[j].extargs_double_value = value;
					}
					else
					{
						if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default)
							args[j].extargs_double_value = args[j].extargs_double_default;
						else
						{
							cout << "参数[" << cur << "]的附加参数值(" << value << ")非法. (类型:double, 范围" << args[j].get_range_double() << ")" << endl;
							return -1;
						}
					}
					i += 2;
					cmd_count += 2;
					break;
				}
				//doublesetdef和doubleseterr参数
				if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
				{
					args[j].args_existed = 1;
					if (!is_strict_double(argv[i + 1]) && args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_default)
					{
						cout << "参数[" << cur << "]的附加参数不是浮点数. (类型:double, 范围[" << args[j].get_range_double() <<"])" << endl;
						return -1;
					}
					else if (!is_strict_double(argv[i + 1]) && args[j].extargs_type == ST_EXTARGS_TYPE::double_with_default)
					{
						cout << "参数[" << cur << "]的附加参数不是浮点数. (类型:double, 范围" << args[j].get_range() << " 缺省:" << args[j].extargs_double_default << ")" << endl;
						return -1;
					}
					double value = 0;
					if (is_strict_double(argv[i + 1]))
						value = stod(argv[i + 1]);
					else
						value = args[j].extargs_int_default;
					args[j].extargs_double_value = args[j].extargs_double_default;
					for (int k = 0; args[j].extargs_double_set[k] != INVALID_DOUBLE_VALUE_OF_SET; k++)
					{
						if (value == args[j].extargs_double_set[k])
						{
							args[j].extargs_double_value = value;
							break;
						}
					}
					if (args[j].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
					{
						bool jud = false;
						for (int k = 0; args[j].extargs_double_set[k] != INVALID_INT_VALUE_OF_SET; k++)
						{
							if (value == args[j].extargs_double_set[k])
							{
								jud = true;
								break;
							}
						}
						if (!jud)
						{
							cout << "参数[" << cur << "]的附加参数值(" << value << ")非法. (类型:double, 可取值[" << args[j].get_range() << "])" << endl;
							return -1;
						}
					}
					i += 2;
					cmd_count += 2;
					break;
				}
				//str1和str2参数
				if (args[j].extargs_type == ST_EXTARGS_TYPE::str)
				{
					args[j].args_existed = 1;
					string value = argv[i + 1];
					args[j].extargs_string_value = value;
					i += 2;
					cmd_count += 2;
					break;
				}
				//strsetdef和strseterr参数
				if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_default || args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
				{
					args[j].args_existed = 1;
					string value = argv[i + 1];

					args[j].extargs_string_value = args[j].extargs_string_default;
					for (int k = 0; args[j].extargs_string_set[k] != ""; k++)
					{
						if (value == args[j].extargs_string_set[k])
						{
							args[j].extargs_string_value = value;
							break;
						}
					}
					if (args[j].extargs_type == ST_EXTARGS_TYPE::str_with_set_error)
					{
						bool jud = false;
						for (int k = 0; args[j].extargs_string_set[k] != ""; k++)
						{
							if (value == args[j].extargs_string_set[k])
							{
								jud = true;
								break;
							}
						}
						if (!jud)
						{
							cout << "参数[" << cur << "]的附加参数值(" << value << ")非法. (类型:string, 可取值[" << args[j].get_range() << "])" << endl;
							return -1;
						}
					}
					i += 2;
					cmd_count += 2;
					break;
				}
				//ipdef和iperr参数
				if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default || args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
				{
					args[j].args_existed = 1;
					string value = argv[i + 1];
					if (args[j].is_valid_ipv4(value))
					{
						args[j].extargs_string_value = value;
						stringstream ss(value);
						int a, b, c, d;
						char dot;
						ss >> a >> dot >> b >> dot >> c >> dot >> d;
						args[j].extargs_ipaddr_value = args[j].ipv4ToUint(a, b, c, d);
						i += 2;
						cmd_count += 2;
						break;
					}
					else
					{
						if (args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default)
						{
							args[j].extargs_string_value = args[j].extargs_string_default;
							i += 2;
							cmd_count += 2;
							break;
						}
						else if(args[j].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
						{
							cout << "参数[" << cur << "]的附加参数值(" << value << ")非法. (类型:IP地址)" << endl;
							return -1;
						}
					}
				}
			}
		}
		bool matched = false;
		for (int j2 = 0; j2 < count_arg; j2++)
		{
			if (cur == args[j2].args_name)
			{
				matched = true;
				break;
			}
		}
		if (!matched)
		{
			cout << "参数[" << cur << "]非法." << endl;
			return -1;
		}
	}
	return cmd_count; //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	int w1, w2, w3 = 15, w4, w5, w6;

	int count_arg = 0;
	while (args[count_arg].args_name != "<NULL>")
		count_arg++;

	size_t length_1 = 4;
	for (int i = 0; i < count_arg; i++)
	{
		string temp(args[i].args_name);
		length_1 = length_1 > temp.length() ? length_1 : temp.length();
	}
	w1 = length_1 + 1;

	size_t length_2 = 4;
	for (int i = 0; i < count_arg; i++)
	{
		string temp(args[i].to_string());
		length_2 = length_2 > temp.length() ? length_2 : temp.length();
	}
	w2 = length_2 + 1;

	size_t length_3 = 7;
	for (int i = 0; i < count_arg; i++)
	{
		if (args[i].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_error ||
			args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
		{
			stringstream ss;
			ss << args[i].extargs_int_default;
			string temp = ss.str();
			length_3 = length_3 > temp.length() ? length_3 : temp.length();
		}
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_error ||
			args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
		{
			stringstream ss;
			ss << fixed << setprecision(6) << args[i].extargs_double_default;
			string temp = ss.str();
			length_3 = length_3 > temp.length() ? length_3 : temp.length();
		}
		else if (args[i].extargs_type == ST_EXTARGS_TYPE::str || args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_default ||
			args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_error || args[i].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default ||
			args[i].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
		{
			length_3 = length_3 > args[i].extargs_string_default.length() ? length_3 : args[i].extargs_string_default.length();
		}
	}
	w3 = length_3 + 1;

	size_t length_4 = 6;
	w4 = length_4 + 1;
	size_t length_5 = 5;
	for (int i = 0; i < count_arg; i++)
	{
		if (args[i].args_existed == 0)
			continue;
		else
		{
			if (args[i].extargs_type == ST_EXTARGS_TYPE::int_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_error ||
				args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::int_with_set_error)
			{
				stringstream ss;
				ss << args[i].extargs_int_value;
				string temp = ss.str();
				length_5 = length_5 > temp.length() ? length_5 : temp.length();
			}
			else if (args[i].extargs_type == ST_EXTARGS_TYPE::double_with_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_error ||
				args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_default || args[i].extargs_type == ST_EXTARGS_TYPE::double_with_set_error)
			{
				stringstream ss;
				ss << fixed << setprecision(6) << args[i].extargs_double_value;
				string temp = ss.str();
				length_5 = length_5 > temp.length() ? length_5 : temp.length();
			}
			else if (args[i].extargs_type == ST_EXTARGS_TYPE::str || args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_default ||
				args[i].extargs_type == ST_EXTARGS_TYPE::str_with_set_error || args[i].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_default ||
				args[i].extargs_type == ST_EXTARGS_TYPE::ipaddr_with_error)
			{
				length_5 = length_5 > args[i].extargs_string_value.length() ? length_5 : args[i].extargs_string_value.length();
			}
		}
	}
	w5 = length_5 + 1;

	size_t length_6 = 9;
	for (int i = 0; i < count_arg; i++)
	{
		string temp = args[i].get_range();
		length_6 = length_6 > temp.length() ? length_6 : temp.length();
	}
	w6 = length_6 + 2;

	cout << setiosflags(ios::left);
	cout << setw(w1 + w2 + w3 + w4 + w5 + w6) << setfill('=') << '=' << setfill(' ') << endl;
	cout << " " << setw(w1) << "name" << setw(w2) << "type" << setw(w3) << "default" << setw(w4) << "exists" << setw(w5) << "value" << setw(w6) << "range/set" << endl;
	cout << setw(w1 + w2 + w3 + w4 + w5 + w6) << setfill('=') << '=' << setfill(' ') << endl;
	for (int i = 0; i < count_arg; i++)
	{
		cout << " " << setw(w1) << args[i].args_name << setw(w2) << args[i].to_string();
		switch (args[i].extargs_type)
		{
			case ST_EXTARGS_TYPE::boolean:
				cout << setw(w3) << boolalpha << args[i].extargs_bool_default << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? string("true") : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::int_with_default:
				cout << setw(w3) << args[i].extargs_int_default << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? string(to_string(args[i].extargs_int_value)) : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::int_with_error:
				cout << setw(w3) << "/" << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? string(to_string(args[i].extargs_int_value)) : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::int_with_set_default:
				cout << setw(w3) << args[i].extargs_int_default << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? string(to_string(args[i].extargs_int_value)) : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::int_with_set_error:
				cout << setw(w3) << "/" << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? string(to_string(args[i].extargs_int_value)) : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::double_with_default:
				cout << setw(w3) << format_double(args[i].extargs_double_default)
					<< setw(w4) << args[i].args_existed
					<< setw(w5) << (args[i].args_existed ? format_double(args[i].extargs_double_value) : "/")
					<< setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::double_with_error:
				cout << setw(w3) << "/"
					<< setw(w4) << args[i].args_existed
					<< setw(w5) << (args[i].args_existed ? format_double(args[i].extargs_double_value) : "/")
					<< setw(w6) << args[i].get_range() << endl;
				break;

			case ST_EXTARGS_TYPE::double_with_set_default:
				cout << setw(w3) << format_double(args[i].extargs_double_default)
					<< setw(w4) << args[i].args_existed
					<< setw(w5) << (args[i].args_existed ? format_double(args[i].extargs_double_value) : "/")
					<< setw(w6) << args[i].get_range() << endl;
				break;

			case ST_EXTARGS_TYPE::double_with_set_error:
				cout << setw(w3) << "/"
					<< setw(w4) << args[i].args_existed
					<< setw(w5) << (args[i].args_existed ? format_double(args[i].extargs_double_value) : "/")
					<< setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::str:
				cout << setw(w3) << (args[i].extargs_string_default == "" ? "/" : args[i].extargs_string_default) << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? args[i].extargs_string_value : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::str_with_set_default:
				cout << setw(w3) << args[i].extargs_string_default << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? args[i].extargs_string_value : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::str_with_set_error:
				cout << setw(w3) << "/" << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? args[i].extargs_string_value : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::ipaddr_with_default:
				cout << setw(w3) << args[i].extargs_string_default << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? args[i].extargs_string_value : "/") << setw(w6) << args[i].get_range() << endl;
				break;
			case ST_EXTARGS_TYPE::ipaddr_with_error:
				cout << setw(w3) << "/" << setw(w4) << args[i].args_existed << setw(w5) << (args[i].args_existed ? args[i].extargs_string_value : "/") << setw(w6) << args[i].get_range() << endl;
				break;
		}
	}
	cout << setw(w1 + w2 + w3 + w4 + w5 + w6) << setfill('=') << '=' << setfill(' ') << endl << endl;
	return 0; //此句根据需要修改
}

#endif // !ENABLE_LIB_COMMON_TOOLS
