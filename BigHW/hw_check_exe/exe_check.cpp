// 2453359 李欣洋 信安
#include "exe_check.h"

// ---------------------------------------------------------
// 友元/全局回调函数定义
// ---------------------------------------------------------

#if TIMER_NEW_VERSION
static void CALLBACK timeout_process(PVOID ExtParameter, BOOLEAN TimerOrWaitFired)
#else
static void CALLBACK timeout_process(UINT uTimerID, UINT uMsg, DWORD ExtParameter, DWORD dw1, DWORD dw2)
#endif
{
    st_CheckExec* my_exe = (st_CheckExec*)ExtParameter;

    /* 1. 计数器 +1 */
    if (++my_exe->time_count >= my_exe->cfg_timeout) {

        /* 2. 标记超时状态 */
        my_exe->eno = CheckExec_Errno::timeout;

        /* 3. 死锁防御机制：
           如果超时后主程序仍然卡在 fgetc 没有退出，说明子进程可能卡死且无输出。
           此时需要在此线程强制杀进程。
        */
        int delta = my_exe->cfg_timeout < 5 ? my_exe->cfg_timeout * 2 : 5;
        if (my_exe->time_count >= my_exe->cfg_timeout + delta) {
            my_exe->stop(CheckExec_Errno::killed_by_callback);
        }
    }
}

// ---------------------------------------------------------
// CheckExec_Errno 输出重载实现
// ---------------------------------------------------------
ostream& operator<<(ostream& out, const CheckExec_Errno& eno)
{
    switch (eno) {
        case CheckExec_Errno::ok: out << "正确运行"; break;
        case CheckExec_Errno::create_timer_id_failed: out << "定时器创建失败"; break;
        case CheckExec_Errno::popen_faliled: out << "管道方式打开失败"; break;
        case CheckExec_Errno::start_timer_failed: out << "启动定时器失败"; break;
        case CheckExec_Errno::timeout: out << "超时"; break;
        case CheckExec_Errno::max_output: out << "超过输出上限"; break;
        case CheckExec_Errno::killed_by_callback: out << "死循环(强制终止)"; break;
        default: out << "未知的错误"; break;
    }
    return out;
}

// ---------------------------------------------------------
// st_CheckExec 类成员函数实现
// ---------------------------------------------------------

st_CheckExec::st_CheckExec(const string& full_exec_cmd, const string& exec_name, int max_output_len, int timeout_second)
{
    this->full_exec_cmd = full_exec_cmd;
    this->exec_name = exec_name;
    this->max_output_len = max_output_len;
    this->cfg_timeout = timeout_second;

    this->time_count = 0;
    this->fp_exe = NULL;
    this->timer_id = NULL;
    this->eno = CheckExec_Errno::ok;
    this->msg.str("");
}

st_CheckExec::~st_CheckExec()
{
    if (this->timer_id != NULL)
        this->stop_timer();

    /* 析构时如果状态异常，确保清理残留进程 */
    if (this->eno != CheckExec_Errno::ok) {
        char cmd_taskkill[512];
        sprintf_s(cmd_taskkill, "taskkill /f /t /im %s 1>nul 2>&1", this->exec_name.c_str());
        system(cmd_taskkill);
    }

    if (this->fp_exe) {
        _pclose(this->fp_exe);
        this->fp_exe = NULL;
    }
}

int st_CheckExec::start_timer()
{
#if TIMER_NEW_VERSION
    // 创建每1000ms触发一次的定时器
    if (CreateTimerQueueTimer(&this->timer_id, NULL, timeout_process, this, 1000, 1000, NULL) == 0) {
        this->eno = CheckExec_Errno::create_timer_id_failed;
        return -1;
    }
#else
    this->timer_id = timeSetEvent(1000, 100, timeout_process, (DWORD_PTR)this, TIME_PERIODIC);
    if (timer_id == NULL) {
        this->eno = CheckExec_Errno::create_timer_id_failed;
        return -1;
    }
#endif

    this->time_count = 0;
    QueryPerformanceFrequency(&this->time_tick);
    QueryPerformanceCounter(&this->begin_time);
    return 0;
}

void st_CheckExec::stop_timer()
{
#if TIMER_NEW_VERSION
    if (this->timer_id != NULL) {
        DeleteTimerQueueTimer(NULL, this->timer_id, NULL);
        this->timer_id = NULL;
    }
#else
    if (this->timer_id != NULL) {
        timeKillEvent(this->timer_id);
        this->timer_id = NULL;
    }
#endif
}

int st_CheckExec::stop(CheckExec_Errno no)
{
    this->stop_timer();
    QueryPerformanceCounter(&this->end_time);
    this->eno = no;

    // 异常结束时调用 taskkill
    if (this->eno != CheckExec_Errno::ok) {
        char cmd_taskkill[512];
        
        sprintf_s(cmd_taskkill, "taskkill /f /t /im %s 1>nul 2>&1", this->exec_name.c_str());
        system(cmd_taskkill);
    }

    if (this->fp_exe) {
        _pclose(this->fp_exe);
        this->fp_exe = NULL;
    }
    return 0;
}

int st_CheckExec::running()
{
    // 1. 启动子进程
    if ((this->fp_exe = _popen(this->full_exec_cmd.c_str(), "rb")) == NULL) {
        this->eno = CheckExec_Errno::popen_faliled;
        return -1;
    }

    // 2. 启动监控定时器
    if (this->start_timer() < 0) {
        this->msg << "启动定时器错误" << endl;
        this->eno = CheckExec_Errno::start_timer_failed;
        return -1;
    }

    // 3. 循环读取输出
    signed char ch;
    int ch_num = 0;

    while ((ch = fgetc(this->fp_exe)) != EOF) {
        this->msg << ch;
        ++ch_num;

        // 检查是否超时（由回调函数设置状态）
        if (this->eno == CheckExec_Errno::timeout) {
            this->stop(CheckExec_Errno::timeout);
            return -1;
        }

        // 检查是否输出超限
        if (ch_num >= this->max_output_len) {
            this->stop(CheckExec_Errno::max_output);
            return -1;
        }
    }

    // 4. 检查是否被回调强杀
    if (this->eno == CheckExec_Errno::killed_by_callback)
        return -1;

    // 5. 正常结束
    this->stop(CheckExec_Errno::ok);
    return 0;
}

double st_CheckExec::get_running_time() const
{
    return double(this->end_time.QuadPart - this->begin_time.QuadPart) / double(time_tick.QuadPart);
}

string st_CheckExec::get_full_cmd_exec() const
{
    return this->full_exec_cmd;
}

CheckExec_Errno st_CheckExec::get_errno() const
{
    return this->eno;
}

int st_CheckExec::reset()
{
    this->time_count = 0;
    this->fp_exe = NULL;
    this->timer_id = NULL;
    this->eno = CheckExec_Errno::ok;
    this->msg.str("");
    return 0;
}