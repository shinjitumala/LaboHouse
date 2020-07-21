/// @file WS.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#include "labo/debug/Log.h"
#include "nlohmann/json.hpp"
#include <labo/server/WS.h>
#include <mutex>

namespace labo::server {
using namespace websocketpp;
using namespace std;

WebSocket::WebSocket(decltype(msg_cb) msg_cb, decltype(close_cb) close_cb)
  : msg_cb{ msg_cb }
  , close_cb{ close_cb }
{
    using namespace websocketpp::log;

    /// Logger settings
    clear_access_channels(alevel::all);
    set_access_channels(alevel::connect);
    set_access_channels(alevel::disconnect);
    set_access_channels(alevel::message_payload);

    using namespace lib::placeholders;
    set_open_handler(bind(&WebSocket::on_open, this, _1));
    set_close_handler(bind(&WebSocket::on_close, this, _1));
    set_message_handler(bind(&WebSocket::on_message, this, _1, _2));
};

void
WebSocket::start(const int port)
{
    init_asio();
    set_reuse_addr(true);

    listen(port);
    logs << "[WebSocket] Started WebSocket at port: " << port << endl;
    start_accept();
    run();
}

void
WebSocket::on_open(connection_hdl h)
{}
void
WebSocket::on_message(connection_hdl h, message_ptr m)
{
    msg_cb(m->get_payload(), h);
}

void
WebSocket::on_close(connection_hdl h)
{
    close_cb(h);
}

void
WebSocket::send(string &&s, connection_hdl h)
{
    Base::send(h, s, frame::opcode::text);
}
}