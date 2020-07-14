/// @file WS.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once

#include <nlohmann/json.hpp>
#include <websocketpp/close.hpp>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/frame.hpp>
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/roles/server_endpoint.hpp>
#include <websocketpp/server.hpp>

namespace labo::server {
/// Abstraction of a WebSocket server for real-time communication.
class WebSocket : websocketpp::server<websocketpp::config::asio>
{
  public:
    /// Default constructor.
    WebSocket();
    /// Start the server. NOTE: Does not return.
    /// @param port
    void start(const int port);

  private:
    /// Connection handler
    /// @param h
    void on_open(websocketpp::connection_hdl h);

    /// Message handler
    /// @param h
    /// @param m
    void on_message(websocketpp::connection_hdl h, message_ptr m);
};
};