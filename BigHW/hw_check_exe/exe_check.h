//2453359 李欣洋 信安
#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <Windows.h>

// 默认使用新版定时器
#define TIMER_NEW_VERSION 1

#if !TIMER_NEW_VERSION
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#endif

using namespace std;

/* 定义运行状态枚举 */
enum class CheckExec_Errno {
    ok = 0,
    create_timer_id_failed,     // 创建定时器ID失败
    popen_faliled,              // 管道方式打开可执行文件失败
    start_timer_failed,         // 启动定时器失败
    timeout,                    // 超时
    max_output,                 // 达到设定的输出上限
    killed_by_callback,         // 死循环（超时且fgetc阻塞），由回调强杀
    max
};

// 重载 << 运算符，方便直接打印错误信息
ostream& operator<<(ostream& out, const CheckExec_Errno& eno);

class st_CheckExec {
protected:
    /* 核心配置参数 */
    string full_exec_cmd;   // 完整的执行命令（如 "demo.exe < in.txt"）
    string exec_name;       // 纯exe文件名（用于taskkill，如 "demo.exe"）
    int    cfg_timeout;     // 超时设置（秒）
    int    max_output_len;  // 最大输出长度（字节）

    FILE* fp_exe;           // 管道文件指针
    int   time_count;       // 当前已运行秒数

    // 定时器相关句柄
#if TIMER_NEW_VERSION
    HANDLE timer_id;
#else
    MMRESULT timer_id;
#endif
    LARGE_INTEGER time_tick;
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;

    CheckExec_Errno eno;    // 当前运行状态

    // 内部使用的私有函数
    int  start_timer();
    void stop_timer();
    int  stop(CheckExec_Errno eno);

public:
    ostringstream msg;      // [对外公开] 存放程序的所有标准输出内容

    // 构造函数：初始化运行参数
    st_CheckExec(const string& full_exec_cmd, const string& exec_name, int max_output_len, int timeout_second);

    // 析构函数：负责清理资源
    ~st_CheckExec();

    // [核心接口] 开始运行程序
    // 返回值：0表示正常结束，-1表示异常（超时、超长等）
    int running();

    // 辅助接口
    double          get_running_time() const; // 获取实际运行耗时(秒)
    string          get_full_cmd_exec() const;
    CheckExec_Errno get_errno() const;        // 获取当前的错误状态
    int             reset();                  // 重置对象状态，以便重复使用

    /* 声明回调函数为友元，以便它可以访问私有成员 */
#if TIMER_NEW_VERSION
    friend void CALLBACK timeout_process(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
#else
    friend void CALLBACK timeout_process(UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
#endif
};

