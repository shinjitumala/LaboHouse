/// @file Base.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <functional>
#include <mutex>
#include <thread>
#include <unordered_set>

namespace labo::server {
using namespace std;

/// Abstraction of a http server.
class Server
{

  public:
    /// Port number of the server.
    int port;

    function<void(int)> action;

  private:
    /// the listening socket
    int socket_listen;

    /// Mutex for thread manipulation.
    mutex t_mtx;
    /// Set of created threads.
    unordered_map<thread::id, pair<thread*, bool>> threads;

  public:
    /// @param port Port number of the server.
    Server(int port, decltype(action) action);

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
    /// Disallow copy because we own resources.
    Server(const Server&) = delete;
};
};