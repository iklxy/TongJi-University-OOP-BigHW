//2453359 李欣洋 信安
#include "make_path.h"
using namespace std;

static bool check_file_exists(const string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

static void manual_swap(Student& a, Student& b) {
    Student temp = a;
    a = b;
    b = temp;
}

//快排
static int partition(vector<Student>& arr, int low, int high) 
{
    string pivot = arr[high].student_num;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) 
    {
        if (arr[j].student_num < pivot) 
        {
            i++;
            manual_swap(arr[i], arr[j]);
        }
    }
    // 将基准元素放到正确的位置 (i+1)
    manual_swap(arr[i + 1], arr[high]);
    return (i + 1);
}

static void my_quick_sort(vector<Student>& arr, int low, int high) 
{
    if (low < high) 
    {
        int pi = partition(arr, low, high);

        // 分别对基准左边和右边的子数组进行递归排序
        my_quick_sort(arr, low, pi - 1);
        my_quick_sort(arr, pi + 1, high);
    }
}

void exe_path::make_path(int count, vector<Student>& s,config_file_tools& cfg,const char* homework_name)
{
    if (!s.empty())
    {
        my_quick_sort(s, 0, s.size() - 1);
    }

	string exe_style = "";
	string temp = homework_name;
	string h_name = "[" + temp + "]";
	cfg.item_get_raw(h_name, "exe_style", exe_style);
    cfg.item_get_string(h_name, "stu_exe_name", this->stu_exe_name);
    cfg.item_get_string(h_name, "pipe_get_input_data_exe_name", this->pipe_get_input_data_exe_name);
    cfg.item_get_string(h_name, "pipe_data_file", this->pipe_data_file);
    cfg.item_get_string(h_name, "redirection_data_dirname", this->redirection_data_dirname);
    cfg.item_get_raw(h_name, "demo_exe_name", demo_exe_path);//拿到demo的文件路径

	if (exe_style == "none")
	{
		cfg.item_get_raw(h_name, "demo_exe_name", demo_exe_path);//拿到demo的文件路径
	}
	else if (exe_style == "single")
	{
        cfg.item_get_string(h_name, "single_exe_dirname", this->signle_exe_dirname);

        string dir_path = this->signle_exe_dirname;
        if (!dir_path.empty() && dir_path.back() != '\\' && dir_path.back() != '/') {
            dir_path += "\\";
        }

        for (auto& stu : s)
        {
            string file_name = stu.student_num + "-" + stu.class_num + "-" + this->stu_exe_name;

            // 拼接完整绝对路径
            string full_path = dir_path + file_name;
            if (check_file_exists(full_path)) 
            {
                stu.exe_exists = true;
            }
            else 
            {
                stu.exe_exists = false;
            }
            // 存入结果向量
            this->Student_exe_path.push_back(full_path);
        }
	}
	else if (exe_style == "multi")
	{
        cfg.item_get_string(h_name, "multi_exe_main_dirname", this->multi_exe_main_dirname);
        cfg.item_get_string(h_name, "multi_exe_sub_dirname", this->multi_exe_sub_dirname);

        string main_dir = this->multi_exe_main_dirname;
        string sub_dir = this->multi_exe_sub_dirname;

        if (!main_dir.empty() && main_dir.back() != '\\') main_dir += "\\";
        if (!sub_dir.empty() && sub_dir.back() != '\\') sub_dir += "\\";

        for (auto& stu : s)
        {
            string full_path = main_dir + stu.student_num + "-" + stu.class_num + "\\" + sub_dir + this->stu_exe_name;
            if (check_file_exists(full_path)) 
            {
                stu.exe_exists = true;
            }
            else 
            {
                stu.exe_exists = false;

            }
            this->Student_exe_path.push_back(full_path);
        }
	}
}