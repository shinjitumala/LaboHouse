/// @file Server.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <thread>
#include <unordered_set>

namespace labo {
using namespace std;

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

    /// Tried to join all created threads before returning.
    ~Server();
};
};