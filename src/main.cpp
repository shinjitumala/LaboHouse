/// @file main.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <iostream>
#include <labo/debug/Log.h>
#include <labo/server/Html.h>
#include <labo/server/WS.h>
#include <set>

using namespace std;
using namespace labo;
using namespace server;

optional<Html*> html;
optional<WebSocket*> web_socket;

void
signal_handler(int sig)
{
    using namespace labo;
    errs << "Signal received: " << strsignal(sig) << endl;
    if (html.has_value()) {
        delete html.value();
    }
    if (web_socket.has_value()) {
        delete web_socket.value();
    }

    ::signal(sig, SIG_DFL);
    ::kill(::getpid(), sig);
};

int
main()
{
    /// Add handlers.
    signal(SIGINT, signal_handler);

    html = new Html{};
    web_socket = new WebSocket{};
    thread t1{ bind(&WebSocket::start, web_socket.value(), 42069) };
    html.value()->start(12345);
}
