/// @file Base.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <algorithm>
#include <labo/debug/Log.h>
#include <netinet/in.h>
#include <sstream>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <unordered_set>

namespace labo {
using namespace std;

/// @tparam Action Action to take when a connection is made. The socket file
/// discriptor is passed to the constructor. ex) Action{socket_fc}.
template<class Action>
class Server
{
  public:
    /// Port number of the server.
    const int port;

  private:
    /// Server will try to exit when this is set to true.
    bool exit;

    /// Set of created threads.
    unordered_set<thread*> threads;

  public:
    /// @param port Port number of the server.
    Server(int port);

    /// Start the server.
    /// WARNING: This function will not return until the server is killed. Call
    /// kill() to return from it.
    void start();

    /// Server's constructor will return as soon as possible after callling
    /// this.
    void kill();

    /// Try to join all created threads before returning.
    ~Server();

  private:
    /// Called when connection is accepted.
    /// @param socket_fd
    static void action(int socket_fd);

    /// Disallow copy because we own resources.
    Server(const Server&) = delete;
};

//================== Definitions below ===========================

template<class Action>
void
Server<Action>::action(int socket_fd)
{
    Action{ socket_fd };
    ::close(socket_fd);
}

template<class Action>
Server<Action>::Server(int port)
  : port{ port }
  , exit{ false } {};

template<class Action>
void
Server<Action>::start()
{
    // Try to open socket.
    auto socket_fd{ ::socket(AF_INET, SOCK_STREAM, 0) };
    if (socket_fd < 0) {
        errs << "Failed to create socket." << endl;
        failure();
    }

    // Close your eyes, some nasty legacy C code
    // We want to pass this struct with address information.
    sockaddr_in server_address;
    ::bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = ::htons(port);

    // Bind our socket to an address
    if (::bind(socket_fd, (sockaddr*)&server_address, sizeof(server_address)) <
        0) {
        errs << "Failed to bind to port: " << port << endl;
        failure();
    }

    // Start listening. Standby for new connections.
    ::listen(socket_fd, 8);

    logs << "Server started at port: " << port << endl;

    sockaddr_in client_address;
    uint client_address_length;
    while (!exit) {
        auto personal_socket_fd{ ::accept(
          socket_fd, (sockaddr*)&client_address, &client_address_length) };
        if (personal_socket_fd < 0) {
            errs << "Failed to accept connection." << endl;
            continue;
        }

        logs << "Accepted connection from: " << client_address.sin_addr.s_addr
             << ":" << client_address.sin_port << endl;
        threads.insert(
          new thread{ Server<Action>::action, personal_socket_fd });
    }
    ::close(socket_fd);
}

template<class Action>
void
Server<Action>::kill()
{
    logs << "Server kill request received." << endl;
    exit = true;
}

template<class Action>
Server<Action>::~Server()
{
    logs << "Waiting for all threads to finish..." << endl;
    for_each(threads.begin(), threads.end(), [](auto thread_ptr) {
        thread_ptr->join();
    });
    logs << "Server terminated." << endl;
}

};