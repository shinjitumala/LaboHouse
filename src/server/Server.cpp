/// @file Server.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <algorithm>
#include <fstream>
#include <future>
#include <labo/debug/Log.h>
#include <labo/server/Server.h>
#include <mutex>
#include <netinet/in.h>
#include <sstream>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

namespace labo::server {

ofstream logs{ "server.log" };

Server::Server(int port, decltype(action) action)
  : port{ port }
  , action{ action } {};

void
Server::start()
{
    // Try to open socket.
    socket_listen = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket_listen < 0) {
        errs << "[Server] Failed to create socket." << endl;
        failure();
    }

    // Close your eyes, some nasty legacy C code
    // We want to pass this struct with address information.
    sockaddr_in server_address;
    ::bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    {
        // Override the socket for faster testing.
        int option{ 1 };
        setsockopt(socket_listen,
                   SOL_SOCKET,
                   (SO_REUSEADDR | SO_REUSEPORT),
                   (const char*)&option,
                   sizeof(option));
    }

    // Bind our socket to an address
    if (::bind(socket_listen,
               (sockaddr*)&server_address,
               sizeof(server_address)) < 0) {
        errs << "[Server] Failed to bind to port: " << port << endl;
        failure();
    }

    // Start listening. Standby for new connections.
    ::listen(socket_listen, 8);

    logs << "[Server] Started at port: " << port << endl;

    sockaddr_in client_address;
    uint client_address_length;
    while (true) {
        auto socket_accepted{ ::accept(
          socket_listen, (sockaddr*)&client_address, &client_address_length) };
        if (socket_accepted < 0) {
            errs << "[Server] Failed to accept connection." << endl;
            continue;
        }

        logs << "[Server] Accepted connection from: "
             << client_address.sin_addr.s_addr << ":" << client_address.sin_port
             << endl;

        lock_guard lg{ t_mtx };
        {
            auto t{ new thread{ [&, socket_accepted] {
                action(socket_accepted);
                lock_guard lg{ t_mtx };
                ::close(socket_accepted);
                threads.at(this_thread::get_id()).second = true;
            } } };
            threads.insert({ t->get_id(), { t, false } });
        }
        for (auto itr{ threads.begin() }; itr != threads.end();) {
            auto [id, s]{ *itr };
            if (s.second) {
                s.first->join();
                delete s.first;
                logs << "[Server] Connetion finished." << endl;
                itr = threads.erase(itr);
            } else {
                itr++;
            }
        }
    }
}

void
Server::kill()
{}

Server::~Server()
{
    logs << "[Server] Waiting for all threads to finish..." << endl;
    ::close(socket_listen);
}
};