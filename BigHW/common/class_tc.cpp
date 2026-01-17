/*2453359 李欣洋 信安*/
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include "../include/class_tc.h"
#include "../include/cmd_console_tools.h"
using namespace std;

//阅读提示
const char* read_promot =
"阅读提示：\n\
\t1、空行用<EMPTY>标出\n\
\t2、文件结束用<EOF>标出\n\
\t3、两行相同列位置的差异字符用亮色标出\n\
\t4、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)\n\
\t5、每行尾的多余的字符用亮色标出，CR/LF/VT/BS/BEL用亮色X标出(方便看清隐含字符)\n\
\t6、每行最后用<CR>标出(方便看清隐含字符)\n\
\t7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出\n";

const char* read_promot_detailed =
"阅读提示：\n\
\t1、空行用<EMPTY>标出\n\
\t2、文件结束用<EOF>标出\n\
\t3、两行相同列位置的差异字符用亮色标出\n\
\t4、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)\n\
\t5、每行尾的多余的字符用亮色标出，CR/LF/VT/BS/BEL用亮色X标出(方便看清隐含字符)\n\
\t6、每行最后用<CR>标出(方便看清隐含字符)\n\
\t7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出\n\
\t8、用--display detailed可以得到更详细的信息\n";

txt_compare::txt_compare(string filename1, string filename2, string trim_type, string display_type, int line_skip,
	int line_offset, int line_max_diffnum, int line_max_linenum, bool ignore_blank,
	bool CR_CRLF_not_equal, bool debug)
{
	this->filename1 = filename1;
	this->filename2 = filename2;
	this->trim_type = trim_type;
	this->display_type = display_type;
	this->line_skip = line_skip;
	this->line_offset = line_offset;
	this->line_max_diffnum = line_max_diffnum;
	this->line_max_linenum = line_max_linenum;
	this->ignore_blank = ignore_blank;
	this->CR_CRLF_not_equal = CR_CRLF_not_equal;
	this->debug = debug;
	this->line_maxlen = 0;
	this->is_equal = false;
	this->diff_count = 0;
}

void txt_compare::compare()
{
	ifstream file1_in(this->filename1 , ios::binary);
	ifstream file2_in(this->filename2 , ios::binary);

	if (!file1_in.is_open())
		return;
	if (!file2_in.is_open())
		return;

	int current_line_1 = 0;
	int current_line_2 = 0;
	string temp_line_1;
	string temp_line_2;
	bool has_next_line_1;
	bool has_next_line_2;

	if (this->line_offset != 0)
	{
		if (this->line_offset < 0)
		{
			for (int i = 0; i < fabs(line_offset); i++)
			{
				if (!get_next_line(file1_in, temp_line_1, current_line_1))
					break;
			}
		}
		if (this->line_offset > 0)
		{
			for (int i = 0; i < fabs(line_offset); i++)
			{
				if (!get_next_line(file2_in, temp_line_2, current_line_2))
					break;
			}
		}
	}

	if (this->line_skip!= 0)
	{
		for (int i = 0; i < this->line_skip; i++)
		{
			has_next_line_1 = get_next_line(file1_in, temp_line_1, current_line_1);
			has_next_line_2 = get_next_line(file2_in, temp_line_2, current_line_2);

			// 安全检查：如果在跳过过程中两个文件都读完了，就没必要继续跳了
			if (!has_next_line_1 && !has_next_line_2)
				break;
		}

		//当skip把两个文件的所有行数都跳过的时候说明文件比较正确
		if (!has_next_line_1 && !has_next_line_2)
		{
			result_buffer << "文件相同" << endl;
			return;
		}
	}

	//核心比较逻辑
	while (true)
	{
		//读取当前行，判断是否读取到文件尾部
		has_next_line_1 = get_next_line(file1_in, temp_line_1, current_line_1);
		has_next_line_2 = get_next_line(file2_in, temp_line_2, current_line_2);
		
		if (!has_next_line_1 && !has_next_line_2)
		{
			break;
		}

		if (this->line_max_linenum > 0 && ((current_line_1 > this->line_max_linenum) || (current_line_2 > this->line_max_linenum)))
			break;

		string str1 = has_next_line_1 ? temp_line_1 : "";
		string str2 = has_next_line_2 ? temp_line_2 : "";

		if (has_next_line_1)
			str1 = trim(temp_line_1);
		if (has_next_line_2)
			str2 = trim(temp_line_2);

		//维护读取到的行最长长度 用于辅助输出===
		int temp_maxlen = (str1.length() > str2.length()) ? str1.length() : str2.length();
		this->line_maxlen = this->line_maxlen > temp_maxlen ? this->line_maxlen : temp_maxlen;

		if (!CR_CRLF_not_equal)
		{
			if (!str1.empty() && str1.back() == '\r')
				str1.pop_back();
			if (!str2.empty() && str2.back() == '\r') 
				str2.pop_back();
		}

		bool line_match = false;

		line_match = (str1 == str2);//进行比较
		if ((has_next_line_1 && !has_next_line_2) || (!has_next_line_1 && has_next_line_2))
			line_match = false;

		if (!line_match)
		{
			this->diff_count += 1;
			size_t temp_maxlen = (str1.length() > str2.length()) ? str1.length() : str2.length();
			size_t temp_minlen = (str1.length() > str2.length()) ? str2.length() : str1.length();
			size_t the_first_diff = string::npos;
			//查找第一个不相符的字符
			for (size_t i = 0; i < temp_minlen; i++)
			{
				if (str1[i] != str2[i])
				{
					the_first_diff = i;
					break;
				}
			}
			if (the_first_diff == string::npos)
				the_first_diff = temp_minlen + 1;

			if (!has_next_line_1) current_line_1 += 1;
			if (!has_next_line_2) current_line_2 += 1;

			// 调整 EOF 时的差异起始位置
			if (!has_next_line_1 || !has_next_line_2)
				the_first_diff = 0;

			// 判断是否满足“前缀一致但长度不同” (两边都有行的情况)
			bool is_trailing_diff = (has_next_line_1 && has_next_line_2 &&
				the_first_diff == temp_minlen + 1 &&
				str1.length() != str2.length());
			string title;
			if (!has_next_line_1 && has_next_line_2)
			{
				// 文件1结束，文件2还有行。检查文件2该行是否有内容。
				if (str2.empty())
				{
					// 情况 1: 文件2也是空行（或被trim为空），显示“文件1已结束...”
					title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
						+ "文件1已结束/文件2仍有内容：";
				}
				else
				{
					// 情况 2: 文件2有实际内容，显示“尾部有多余字符”
					title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
						+ "文件2的尾部有多余字符：";
				}
			}
			else if (has_next_line_1 && !has_next_line_2)
			{
				// 文件2结束，文件1还有行。检查文件1该行是否有内容。
				if (str1.empty())
				{
					title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
						+ "文件2已结束/文件1仍有内容：";
				}
				else
				{
					title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
						+ "文件1的尾部有多余字符：";
				}
			}
			else if (is_trailing_diff)
			{
				// 两边都有行，但一边长一边短
				string file_name = (str1.length() > str2.length()) ? "文件1" : "文件2";
				title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
					+ file_name + "的尾部有多余字符：";
			}
			else
			{
				// 普通字符差异
				title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - 第["
					+ to_string(the_first_diff) + "]个字符开始有差异：";
			}
			result_buffer << title << endl;

			if (this->display_type == "detailed")
			{
				string promot_line = "        ";//--提示符
				for (size_t i = 0; i <= temp_maxlen / 10; i++)
					promot_line += "----------";
				result_buffer << promot_line << endl;

				string promot_num = "        ";//数字提示符
				for (size_t i = 0; i <= temp_maxlen / 10; i++)
					promot_num = promot_num + to_string(i) + "         ";
				result_buffer << promot_num << endl;

				string promot_ruler = "        ";//数字尺提示符
				for (size_t i = 0; i <= temp_maxlen / 10; i++)
					promot_ruler += "0123456789";
				result_buffer << promot_ruler << endl;

				result_buffer << promot_line << endl;//封最后一行
			}

			if (has_next_line_1)
			{
				if (str1.empty()) {
					result_buffer << "<EMPTY>" << endl;
				}
				else
				{
					result_buffer << str1 << endl;
				}
			}
			else
				result_buffer << "<EOF>" << endl;

			if (has_next_line_2)
			{
				if (str2.empty()) 
				{
					result_buffer << "<EMPTY>" << endl;
				}
				else
				{
					result_buffer << str2 << endl;
				}
			}
			else
				result_buffer << "<EOF>" << endl;
		}
		if (line_max_diffnum > 0 && diff_count >= line_max_diffnum) 
		{
			break; // 达到最大差异行数，退出循环
		}

		if (!has_next_line_1 || !has_next_line_2)
			break;
	}
	if (this->diff_count == 0)
		this->is_equal = true;
}

int txt_compare::compare(istream& stream1, istream& stream2,  bool silent)
{
	this->is_equal = false;
	this->diff_count = 0;
	this->result_buffer.str("");
	this->line_maxlen = 0;

	// -------------------------------------------------------
	// 下面是核心比较逻辑
	// 请将原 compare() 函数中 "int current_line_1 = 0;" 
	// 开始直到 "if (this->diff_count == 0) this->is_equal = true;" 
	// 结束的所有代码 **完整复制** 到这里。
	// -------------------------------------------------------

	// [复制开始]
	int current_line_1 = 0;
	int current_line_2 = 0;
	string temp_line_1;
	string temp_line_2;
	bool has_next_line_1;
	bool has_next_line_2;

	if (this->line_offset != 0)
	{
		if (this->line_offset < 0)
		{
			for (int i = 0; i < fabs(line_offset); i++)
			{
				if (!get_next_line(stream1, temp_line_1, current_line_1))
					break;
			}
		}
		if (this->line_offset > 0)
		{
			for (int i = 0; i < fabs(line_offset); i++)
			{
				if (!get_next_line(stream2, temp_line_2, current_line_2))
					break;
			}
		}
	}

	if (this->line_skip != 0)
	{
		for (int i = 0; i < this->line_skip; i++)
		{
			has_next_line_1 = get_next_line(stream1, temp_line_1, current_line_1);
			has_next_line_2 = get_next_line(stream2, temp_line_2, current_line_2);

			// 安全检查：如果在跳过过程中两个文件都读完了，就没必要继续跳了
			if (!has_next_line_1 && !has_next_line_2)
				break;
		}

		//当skip把两个文件的所有行数都跳过的时候说明文件比较正确
		if (!has_next_line_1 && !has_next_line_2)
		{
			result_buffer << "文件相同" << endl;
			return 0;
		}
	}
	while (true)
	{
		// 这里会自动调用上面新加的重载版本，因为 file1_in 是 istream 类型
		has_next_line_1 = get_next_line(stream1, temp_line_1, current_line_1);
		has_next_line_2 = get_next_line(stream2, temp_line_2, current_line_2);

		if (!has_next_line_1 && !has_next_line_2)
		{
			break;
		}

		if (this->line_max_linenum > 0 && ((current_line_1 > this->line_max_linenum) || (current_line_2 > this->line_max_linenum)))
			break;

		string str1 = has_next_line_1 ? temp_line_1 : "";
		string str2 = has_next_line_2 ? temp_line_2 : "";

		if (has_next_line_1)
			str1 = trim(temp_line_1);
		if (has_next_line_2)
			str2 = trim(temp_line_2);

		//维护读取到的行最长长度 用于辅助输出===
		int temp_maxlen = (str1.length() > str2.length()) ? str1.length() : str2.length();
		this->line_maxlen = this->line_maxlen > temp_maxlen ? this->line_maxlen : temp_maxlen;

		if (!CR_CRLF_not_equal)
		{
			if (!str1.empty() && str1.back() == '\r')
				str1.pop_back();
			if (!str2.empty() && str2.back() == '\r')
				str2.pop_back();
		}

		bool line_match = false;

		line_match = (str1 == str2);//进行比较
		if ((has_next_line_1 && !has_next_line_2) || (!has_next_line_1 && has_next_line_2))
			line_match = false;

		if (line_match && has_next_line_1 && has_next_line_2 && !this->ignore_blank)
		{
			// 如果两边文本内容完全一样，但一个流已经 EOF (无换行结束)，另一个流没 EOF (有换行结束)
			if (stream1.eof() != stream2.eof())
			{
				line_match = false; // 判定为不同
			}
		}

		if (!line_match)
		{
			this->diff_count += 1;
			size_t temp_maxlen = (str1.length() > str2.length()) ? str1.length() : str2.length();
			size_t temp_minlen = (str1.length() > str2.length()) ? str2.length() : str1.length();
			size_t the_first_diff = string::npos;
			//查找第一个不相符的字符
			for (size_t i = 0; i < temp_minlen; i++)
			{
				if (str1[i] != str2[i])
				{
					the_first_diff = i;
					break;
				}
			}
			if (the_first_diff == string::npos)
				the_first_diff = temp_minlen + 1;

			if (!has_next_line_1) current_line_1 += 1;
			if (!has_next_line_2) current_line_2 += 1;

			// 调整 EOF 时的差异起始位置
			if (!has_next_line_1 || !has_next_line_2)
				the_first_diff = 0;

			// 判断是否满足“前缀一致但长度不同” (两边都有行的情况)
			bool is_trailing_diff = (has_next_line_1 && has_next_line_2 &&
				the_first_diff == temp_minlen + 1 &&
				str1.length() != str2.length());
			string title;
			if (!has_next_line_1 && has_next_line_2)
			{
				// 文件1结束，文件2还有行。检查文件2该行是否有内容。
				if (str2.empty())
				{
					// 情况 1: 文件2也是空行（或被trim为空），显示“文件1已结束...”
					title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
						+ "文件1已结束/文件2仍有内容：";
				}
				else
				{
					// 情况 2: 文件2有实际内容，显示“尾部有多余字符”
					title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
						+ "文件2的尾部有多余字符：";
				}
			}
			else if (has_next_line_1 && !has_next_line_2)
			{
				// 文件2结束，文件1还有行。检查文件1该行是否有内容。
				if (str1.empty())
				{
					title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
						+ "文件2已结束/文件1仍有内容：";
				}
				else
				{
					title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
						+ "文件1的尾部有多余字符：";
				}
			}
			else if (is_trailing_diff)
			{
				// 两边都有行，但一边长一边短
				string file_name = (str1.length() > str2.length()) ? "文件1" : "文件2";
				title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - "
					+ file_name + "的尾部有多余字符：";
			}
			else
			{
				// 普通字符差异
				title = "第[" + to_string(current_line_1) + " / " + to_string(current_line_2) + "]行 - 第["
					+ to_string(the_first_diff) + "]个字符开始有差异：";
			}
			result_buffer << title << endl;

			if (this->display_type == "detailed")
			{
				string promot_line = "        ";//--提示符
				for (size_t i = 0; i <= temp_maxlen / 10; i++)
					promot_line += "----------";
				result_buffer << promot_line << endl;

				string promot_num = "        ";//数字提示符
				for (size_t i = 0; i <= temp_maxlen / 10; i++)
					promot_num = promot_num + to_string(i) + "         ";
				result_buffer << promot_num << endl;

				string promot_ruler = "        ";//数字尺提示符
				for (size_t i = 0; i <= temp_maxlen / 10; i++)
					promot_ruler += "0123456789";
				result_buffer << promot_ruler << endl;

				result_buffer << promot_line << endl;//封最后一行
			}

			if (has_next_line_1)
			{
				if (str1.empty()) {
					result_buffer << "<EMPTY>" << endl;
				}
				else
				{
					result_buffer << str1 << endl;
				}
			}
			else
				result_buffer << "<EOF>" << endl;

			if (has_next_line_2)
			{
				if (str2.empty())
				{
					result_buffer << "<EMPTY>" << endl;
				}
				else
				{
					result_buffer << str2 << endl;
				}
			}
			else
				result_buffer << "<EOF>" << endl;
		}
		if (line_max_diffnum > 0 && diff_count >= line_max_diffnum)
		{
			break; // 达到最大差异行数，退出循环
		}

		if (!has_next_line_1 || !has_next_line_2)
			break;
	}
	if (this->diff_count == 0)
		this->is_equal = true;

	return this->diff_count;
}

void txt_compare::result()
{
	int width = (this->line_maxlen / 10 + 1) * 10 + 8 + 2;
	if (this->is_equal)
	{
		if (this->display_type != "none")
		{
			cout << "比较结果输出：" << endl;
			for (int i = 0; i < width; i++)
				cout << "=";
			cout << endl;
			cout << "在指定检查条件下完全一致." << endl;
			for (int i = 0; i < width; i++)
				cout << "=";
			cout << endl;
		}
		else
		{
			cout << "文件相同." << endl;
		}
	}
	else
	{
		if (this->display_type != "none")
		{
			cout << "比较结果输出：" << endl;
			for (int i = 0; i < width; i++)
				cout << "=";
			cout << endl;
			string all_content = this->result_buffer.str();
			bool is_detailed = (this->display_type == "detailed");
			if (this->display_type == "detailed")
			{
				printGroup(all_content, 7, is_detailed, this->ignore_blank);
			}
			else
			{
				printGroup(all_content, 3, is_detailed, this->ignore_blank);
			}
			for (int i = 0; i < width; i++)
				cout << "=";
			cout << endl;
			cout << "在指定检查条件下共" << this->diff_count << "行有差异." << endl;
			if (this->display_type == "detailed")
			{
				cout << read_promot;
			}
			else
			{
				cout << read_promot_detailed;
			}
			for (int i = 0; i < width; i++)
				cout << "=";
			cout << endl;
		}
		else
		{
			cout << "文件不同." << endl;
		}
	}
}

string txt_compare::trim(const string& line)
{
	string space = " \t\n";

	if (!this->CR_CRLF_not_equal)
		space += "\r";

	string temp_line = line;

	if (this->trim_type == "none" || this->trim_type == "")
		return temp_line;

	size_t first = 0;
	size_t last = string::npos; 

	// 处理 Left
	if (this->trim_type == "left" || this->trim_type == "all")
	{
		first = temp_line.find_first_not_of(space);
		if (first == string::npos) // 全是空格
			return "";
	}

	// 处理 Right
	if (this->trim_type == "right" || this->trim_type == "all")
	{
		last = temp_line.find_last_not_of(space);
		if (last == string::npos) // 全是空格
			return "";
	}

	if (this->trim_type == "right") {
		// last 是索引，长度是 last + 1
		return temp_line.substr(0, last + 1);
	}
	else if (this->trim_type == "left") {
		// 从 first 开始截取到末尾
		return temp_line.substr(first);
	}
	else if (this->trim_type == "all") {
		// 从 first 开始，长度是 last - first + 1
		return temp_line.substr(first, last - first + 1);
	}

	return temp_line;
}

bool txt_compare::get_next_line(ifstream& in, string& line_out, int& line_count)
{
	string temp_line;
	
	while (getline(in, temp_line))
	{
		line_count++;
		if (!this->CR_CRLF_not_equal)
		{
			if (!temp_line.empty() && temp_line.back() == '\r')
				temp_line.pop_back();
		}
		if (this->ignore_blank)
		{
			string trim_line = trim(temp_line);

			if (trim_line.empty() || trim_line=="\n")
				continue;
		}

		line_out = temp_line;
		return true;
	}

	return false;
}

bool txt_compare::get_next_line(istream& in, string& line_out, int& line_count)
{
	string temp_line;
	// getline 支持 istream，所以文件流和字符串流都能用
	while (getline(in, temp_line))
	{
		line_count++;
		if (!this->CR_CRLF_not_equal)
		{
			if (!temp_line.empty() && temp_line.back() == '\r')
				temp_line.pop_back();
		}
		if (this->ignore_blank)
		{
			string trim_line = trim(temp_line);
			if (trim_line.empty() || trim_line == "\n")
				continue;
		}

		line_out = temp_line;
		return true;
	}
	return false;
}
void printGroup(const string& all_content, int group_size, bool is_detailed, bool is_ignore_blank) 
{
	istringstream iss(all_content);
	vector<string>Agroup;
	Agroup.reserve(group_size);

	string line;
	int line_count = 0;

	while (getline(iss, line))
	{
		Agroup.push_back(line);
		line_count++;
		if (line_count != group_size)
			continue;
		//找到最短行的长度 方便后续输出有差异的高亮字符
		size_t min_len = (Agroup[group_size - 1].length() > Agroup[group_size - 2].length()) ? Agroup[group_size - 2].length() : Agroup[group_size - 1].length();

		for (size_t i = 0; i < Agroup.size(); i++) 
		{
			if(i!=Agroup.size()-1 && i!=Agroup.size()-2)
				cout << Agroup[i] << endl;
			else if (i==Agroup.size()-2)
			{
				cout << "文件1 : ";
				for (size_t j = 0; j < min_len; j++)
				{
					if (Agroup[Agroup.size() - 2][j] == Agroup[Agroup.size() - 1][j])
						cout << Agroup[Agroup.size() - 2][j];
					else if (Agroup[Agroup.size() - 2][j] != Agroup[Agroup.size() - 1][j])
					{
						cct_setcolor(14, 4);
						cout << Agroup[Agroup.size() - 2][j];
						cct_setcolor(0, 7);
					}
				}
				if (Agroup[Agroup.size() - 2].length() != min_len)
				{
					for (size_t k = min_len; k < Agroup[Agroup.size() - 2].length(); k++)
					{
						cct_setcolor(14, 4);
						char ch = Agroup[Agroup.size() - 2][k];
						if (ch == '\r')
							cout << 'X';
						else
							cout << ch;
						cct_setcolor(0, 7);
					}
				}
				if (Agroup[Agroup.size() - 2] != "<EOF>" && Agroup[Agroup.size() - 2] != "\n")
					cout << "<CR>" << endl;
				else if (Agroup[Agroup.size() - 2] == "\n")
				{
					cct_setcolor(14, 4);
					cout << "<EMPTY>" << endl;
					cct_setcolor(0, 7);
				}
				else
					cout << endl;
			}
			else if (i == Agroup.size() - 1)
			{
				cout << "文件2 : ";
				for (size_t j = 0; j < min_len; j++)
				{
					if (Agroup[Agroup.size() - 2][j] == Agroup[Agroup.size() - 1][j])
						cout << Agroup[Agroup.size() - 1][j];
					else if (Agroup[Agroup.size() - 2][j] != Agroup[Agroup.size() - 1][j])
					{
						cct_setcolor(14, 4);
						cout << Agroup[Agroup.size() - 1][j];
						cct_setcolor(0, 7);
					}
				}
				if (Agroup[Agroup.size() - 1].length() != min_len)
				{
					for (size_t k = min_len; k < Agroup[Agroup.size() - 1].length(); k++)
					{
						cct_setcolor(14, 4);
						char ch = Agroup[Agroup.size() - 1][k];
						if (ch == '\r')
							cout << 'X';
						else
							cout << ch;
						cct_setcolor(0, 7);
					}
				}
				if (Agroup[Agroup.size() - 1] != "<EOF>" && Agroup[Agroup.size() - 1] != "\n")
					cout << "<CR>" << endl;
				else if ((Agroup[Agroup.size() - 1] == "\n" && is_ignore_blank) || Agroup[Agroup.size() - 1]=="")
				{
					cct_setcolor(14, 4);
					cout << "<EMPTY>" << endl;
					cct_setcolor(0, 7);
				}
				else
					cout << endl;
			}
		}

		if (is_detailed)
		{
			cout << "文件1(HEX) :" << endl;
			if (Agroup[Agroup.size() - 2] == "<EOF>") 
			{
				cout << "<EOF>" << endl; // 按照要求直接输出 EOF
			}
			else if(Agroup[Agroup.size() - 2] == "<EMPTY>")
			{
				cout << "<EMPTY>" << endl;
			}
			else 
			{
				string hex1 = get_hex_output(Agroup[Agroup.size() - 2]);
				if (!hex1.empty()) 
					cout << hex1;
			}

			cout << "文件2(HEX) :" << endl;
			if (Agroup[Agroup.size() - 1] == "<EOF>") 
			{
				cout << "<EOF>" << endl;
			}
			else if (Agroup[Agroup.size() - 1] == "<EMPTY>")
			{
				cout << "<EMPTY>" << endl;
			}
			else 
			{
				string hex2 = get_hex_output(Agroup[Agroup.size() - 1]);
				if (!hex2.empty()) 
					cout << hex2;
			}
		}
		cout << endl;

		// 清空当前组，准备下一组
		Agroup.clear();
		line_count = 0;
	}
}

//单行转hex输出辅助函数
string get_hex_output(const string& data)
{
	if (data.empty()) return "";

	ostringstream oss;
	int len = data.length();

	//oss << uppercase; // 设为大写 (如 0A 而非 0a)

	// 每行处理 16 个字节
	for (int i = 0; i < len; i += 16)
	{
		oss << setfill('0') << setw(8) << hex << i << " : ";

		for (int j = 0; j < 16; j++)
		{
			if (i + j < len)
			{
				oss << setfill('0') << setw(2) << hex << (int)(unsigned char)data[i + j] << " ";
			}
			else
			{
				oss << "   "; // 不足 16 字节补空格
			}

			if (j == 7) {
				if (i + 8 < len)
					oss << "- ";
				else
					oss << "  ";
			}
		}

		oss << " "; // HEX区和ASCII区之间的间隔
		for (int j = 0; j < 16; j++)
		{
			if (i + j < len)
			{
				unsigned char c = (unsigned char)data[i + j];
				if (c > 32 && c <= 126)
					oss << (char)c;
				else
					oss << ".";
			}
		}

		oss << endl;
	}

	// 恢复流状态
	oss << nouppercase << dec;
	return oss.str();
}