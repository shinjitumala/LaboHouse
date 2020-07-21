/// @file main.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include "labo/LaboHouse.h"
#include <labo/debug/Log.h>

using namespace std;
using namespace labo;
using namespace server;

void
signal_handler(int sig)
{
    errs << "Signal received: " << strsignal(sig) << endl;
    ::signal(sig, SIG_DFL);
    ::kill(::getpid(), sig);
};

int
main()
{
    /// Add handlers.
    signal(SIGINT, signal_handler);

    LaboHouse labohouse;
    labohouse.start();
}
