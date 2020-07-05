/// @file Log.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <chrono>
#include <ctime>
#include <fpr/log/LineCallbackBuf.h>
#include <iomanip>
#include <labo/debug/Log.h>
#include <mutex>
#include <sstream>
#include <unordered_map>

namespace labo {
void
failure()
{
    logs << "Error" << std::endl;
    ::abort();
};

void
warning()
{
    logs << "Warning" << std::endl;
}

namespace log {
TimeCallback tcb;
TimeCallback tcb_err;
LineCallbackBuf<TimeCallback> stream_hook{ cout, tcb };
LineCallbackBuf<TimeCallback> stream_hook_errs{ cerr, tcb_err };

mutex mtx;
auto
get_threadnum()
{

    auto tid{ pthread_self() };
    static unordered_map<decltype(tid), uint> thread_nums;
    static uint count{ 0 };

    uint thread_num;
    if (auto itr{ thread_nums.find(tid) }; itr == thread_nums.end()) {
        thread_nums.insert({ tid, count });
        thread_num = count;
        count++;
    } else {
        thread_num = itr->second;
    }
    ostringstream oss;
    oss << setw(4) << setfill('0') << thread_num;
    return oss.str();
}

int
TimeCallback::call(streambuf& buf) const
{
    lock_guard<mutex> lg{ mtx };
    string output{ "[T" + get_threadnum() + "] " };
    return buf.sputn(output.c_str(), output.size());
};
};
ostream logs{ &log::stream_hook };
ostream errs{ &log::stream_hook_errs };
};