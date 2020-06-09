/// @file main.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <fstream>
#include <iostream>
#include <labo/debug/Log.h>
#include <labo/server/Base.h>
#include <labo/server/http.h>
#include <labo/server/socket_stream.h>
#include <limits>
// #include <llvm/Support/CommandLine.h>
#include <regex>
#include <signal.h>

// namespace labo::cl {
// using namespace std;
// using namespace llvm::cl;

// /// List used commandline options here.
// const vector<const OptionCategory*> related_categories{};
// };

struct Action
{
    Action(int socket_fd)
    {
        using namespace labo;
        logs << "Start hoge: " << socket_fd << endl;

        socket::fdstreambuf stream{ socket_fd };
        istream in{ &stream };
        ostream out{ &stream };

        // for (string line; getline(in, line);) {
        //     out << line << endl;
        // }

        http::Request req;
        in >> req;
        if(req.path == "/"){
            /// reply with home page
        }

        const string s{ "hello wolrd" };
        out << "HTTP/1.1 200 OK" << endl;
        out << "Set-Cookie: COOKIE" << endl;
        out << "Content-Length: " << s.size() << endl;
        out << endl;
        out << s << endl << endl;

        logs << "Finish hoge" << endl;
    }
};

labo::Server<Action>* server{ nullptr };

void
signal_handler(int sig)
{
    using namespace labo;
    errs << "Signal received: " << strsignal(sig) << endl;
    if (server) {
        errs << "Terminating..." << endl;
        server->kill();
        delete server;
    } else {
        errs << "Server was not running." << endl;
    }
    logs << "Goodbye." << endl;

    ::signal(sig, SIG_DFL);
    ::kill(::getpid(), sig);
};

int
main(int argc, char* argv[])
{
    // Command line stuff. It's magic.
    // llvm::cl::HideUnrelatedOptions(labo::cl::related_categories);
    // llvm::cl::ParseCommandLineOptions(argc, argv);

    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGSTOP, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGABRT, signal_handler);

    server = new labo::Server<Action>{ 12345 };
    server->start();
    return 0;
}