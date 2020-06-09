/// @file Log.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once

#include <experimental/source_location>
#include <fpr/log/LineCallbackBuf.h>
#include <fpr/log/callback/Indent.h>

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

/// Callback that inserts a time tag before the line.
struct TimeCallback
{
    /// Inserts time tag before printing line.
    /// @param buf
    /// @return int
    int call(streambuf& buf) const;
};
};

extern ostream logs;
extern ostream errs;

};