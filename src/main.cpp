/// @file main.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <iostream>
#include <labo/debug/Log.h>
#include <labo/server/Base.h>
#include <llvm/Support/CommandLine.h>
#include <signal.h>

namespace labo::cl {
using namespace std;
using namespace llvm::cl;

/// List used commandline options here.
const vector<const OptionCategory*> related_categories{};
};

struct Action
{
    Action(int socket_fd)
    {
        using namespace labo;
        logs << "Start hoge: " << socket_fd << endl;
        char buffer[256];
        ::bzero(buffer, 256);
        {
            auto size{ ::read(socket_fd, buffer, 255) };
            if (size < 0) {
                errs << "Input error" << endl;
            }
            logs << "Received: " << buffer << endl;
        }
        {
            stringstream ss;
            ss << "echo: " << buffer;
            string reply{ ss.str() };
            auto size{ ::write(socket_fd, reply.c_str(), reply.size()) };
            if (size < 0) {
                errs << "Output error" << endl;
            }
        }

        logs << "Finish hoge" << endl;
    }
};

labo::Server<Action>* server{ nullptr };

void
terminate_server(int sig)
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
    ::exit(0);
};

template<class T, typename labo::is_action<T>::value = true>
void
foo()
{}

    struct Hoge{
    };
int
main(int argc, char* argv[])
{
    // Command line stuff. It's magic.
    llvm::cl::HideUnrelatedOptions(labo::cl::related_categories);
    llvm::cl::ParseCommandLineOptions(argc, argv);

    signal(SIGQUIT, terminate_server);
    signal(SIGTERM, terminate_server);
    signal(SIGSTOP, terminate_server);
    signal(SIGINT, terminate_server);


    server = new labo::Server<Action>{ 12345 };
    server->start();
    return 0;
}