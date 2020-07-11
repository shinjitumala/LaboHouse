/// @file Log.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once

#include <fpr/log/LineCallbackBuf.h>
#include <fpr/log/callback/Indent.h>
#include <mutex>

namespace labo {
using namespace std;

/// Crashes the program.
[[noreturn]] void
failure();

/// Outputs a warning message.
void
warning();

namespace log {
using namespace fpr::log;

/// Obain the log lock guard.
lock_guard<mutex>
get_lg();
};

extern ostream logs;
extern ostream errs;

};