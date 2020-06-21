/// @file main.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <filesystem>
#include <fstream>
#include <iostream>
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>
#include <labo/server/Base.h>
#include <labo/server/http/Body.h>
#include <labo/server/http/Request.h>
#include <labo/server/http/Response.h>
#include <labo/util/fdstreambuf.h>
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

labo::LaboHouse labohouse{};

struct Action
{
    Action(int socket_fd)
    {
        using namespace labo;
        socket::fdstreambuf stream{ socket_fd };
        istream in{ &stream };
        ostream out{ &stream };

        http::Request req;
        in >> req;
        switch (req.method) {
            case http::Request::Method::GET:
                if (req.path == "/") {
                    /// reply with home page

                    out << http::Response{ http::Response::Status::OK,
                                           { "../res/home.html" },
                                           { { "Set-Cookie", "foobar" } } };

                    logs << "Replied with home page." << endl;
                    return;
                }
                break;
            case http::Request::Method::POST:
                if (req.path == "/register") {
                    /// Register new name
                    auto name{ req.headers.at("name") };
                    auto id{ labohouse.add_name(name) };

                    out << http::Response{ http::Response::Status::OK,
                                           { { "Set-Cookie",
                                               to_string(id) } } };
                    return;
                }
                break;
        }

        out << http::Response{ http::Response::Status::NOT_FOUND,
                               { "../res/not_found.html" } };
        logs << "Replied with not found." << endl;
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
        delete server;
        logs << "Server terminated." << endl;
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

    server = new labo::Server<Action>{ 12345 };
    server->start();
    return 0;
}