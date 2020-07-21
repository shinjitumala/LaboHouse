/// @file WS.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once

#include <functional>
#include <memory>
#include <nlohmann/json.hpp>
#include <set>
#include <labo/server/WS_Headers.h>

namespace labo::server {
using namespace std;
/// Abstraction of a WebSocket server for real-time communication.
class WebSocket : public websocketpp::server<websocketpp::config::asio>
{
    using Base = websocketpp::server<websocketpp::config::asio>;

    /// Message receive callback.
    function<void(string, websocketpp::connection_hdl)> msg_cb;
    /// Close connection callback
    function<void(websocketpp::connection_hdl)> close_cb;

  public:
    /// Default constructor.
    WebSocket(decltype(msg_cb) msg_cb, decltype(close_cb) close_cb);
    /// Start the server. NOTE: Does not return.
    /// @param port
    void start(const int port);

    /// Send message to client.
    /// @param s
    /// @param h
    void send(string&& s, websocketpp::connection_hdl h);

  private:
    /// Connection Open handler.
    /// @param h
    void on_open(websocketpp::connection_hdl h);

    /// Message handler.
    /// @param h
    /// @param m
    void on_message(websocketpp::connection_hdl h, message_ptr m);

    /// Connection Close handler.
    void on_close(websocketpp::connection_hdl h);

    /// No copy.
    WebSocket(const WebSocket&) = delete;
};
};