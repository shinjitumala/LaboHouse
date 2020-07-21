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

namespace labo {
class LaboHouse;
namespace server {
using namespace std;

/// Abstraction of a http server that returns a html page.
class Html
{
  private:
    /// the listening socket
    int sfd;

    /// Reference to LaboHouse
    LaboHouse& lh;

  public:
    /// Default constructor.
    Html(LaboHouse &lh);

    /// Start the server.
    /// WARNING: This function will not return until the server is killed. Call
    /// kill() to return from it.
    void start(const int port);

    /// Try to join all created threads before returning.
    ~Html();

  private:
    /// Disallow copy because we own resources.
    Html(const Html&) = delete;
};
};
};