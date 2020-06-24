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
#include <type_traits>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>

namespace labo {
using namespace std;

/// Checks if T is an appropriate class to use for Server
/// @tparam T
template<class T>
class is_action
{
    template<class U>
    static constexpr auto test(int i) -> decltype(U{ i }, true_type());
    template<class>
    static constexpr auto test(...) -> false_type;

  public:
    static constexpr bool value{ is_same_v<decltype(test<T>(0)), true_type> &&
                                 !is_fundamental_v<T> };
};

/// @tparam Action Action to take when a connection is made. The socket file
/// discriptor is passed to the constructor. ex) Action{socket_fc}.
template<class Action>
class Server
{
    static_assert(is_action<Action>::value, "Invalid 'Action' type.");

  public:
    /// Port number of the server.
    const int port;

  private:
    /// the listening socket
    int socket_listen;

    /// Set of created threads.
    unordered_set<thread*> threads;
    /// All client sockets.
    unordered_map<thread*, int> sockets_accepted;

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
  : port{ port } {};

template<class Action>
void
Server<Action>::start()
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

        logs << "[Server] Accepted connection from: " << client_address.sin_addr.s_addr
             << ":" << client_address.sin_port << endl;
        auto [itr, status]{ threads.insert(
          new thread{ Server<Action>::action, socket_accepted }) };
        sockets_accepted.insert({ *itr, socket_accepted });
    }
}

template<class Action>
Server<Action>::~Server()
{
    logs << "[Server] Waiting for all threads to finish..." << endl;
    for_each(threads.begin(), threads.end(), [&](auto thread_ptr) {
        thread_ptr->join();
        auto itr{ sockets_accepted.find(thread_ptr) };
        ::close(itr->second);
        sockets_accepted.erase(itr);
    });
    ::close(socket_listen);
}

};