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
#include <fpr/log/Location.h>
#include <iomanip>
#include <labo/debug/Log.h>
#include <sstream>

namespace labo {
void
failure(experimental::source_location loc)
{
    using fpr::log::operator<<;
    logs << "Error at: " << loc << std::endl;
    ::abort();
};

void
warning(experimental::source_location loc)
{
    using fpr::log::operator<<;
    logs << "Warning at: " << loc << std::endl;
}

namespace log {
TimeCallback tcb;
TimeCallback tcb_err;
LineCallbackBuf<TimeCallback> stream_hook{ cout, tcb };
LineCallbackBuf<TimeCallback> stream_hook_errs{ cerr, tcb_err };

int
TimeCallback::call(streambuf& buf) const
{
    auto now{ chrono::system_clock::now() };
    auto time_t{ chrono::system_clock::to_time_t(now) };
    stringstream ss;
    ss << "[" << put_time(localtime(&time_t), "%Y-%m-%d_%X") << "] ";
    auto output{ ss.str() };
    return buf.sputn(output.c_str(), output.size());
};
};
ostream logs{ &log::stream_hook };
ostream errs{ &log::stream_hook_errs };
};