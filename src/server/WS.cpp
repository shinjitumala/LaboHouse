/// @file WS.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/server/WS.h>

namespace labo::server {
using namespace websocketpp;

WebSocket::WebSocket()
{
    using namespace log;
    using namespace lib::placeholders;

    /// Logger settings
    clear_error_channels(elevel::all);
    set_error_channels(elevel::warn);
    set_error_channels(elevel::rerror);
    clear_access_channels(alevel::all);
    set_access_channels(alevel::connect);
    set_access_channels(alevel::disconnect);
    set_access_channels(alevel::message_payload);

    set_open_handler(bind(&WebSocket::on_open, this, _1));

    set_message_handler(bind(&WebSocket::on_message, this, _1, _2));
};

void
WebSocket::start(const int port)
{
    init_asio();
    set_reuse_addr(true);

    listen(port);
    start_accept();
    run();
}

void
WebSocket::on_open(connection_hdl h)
{}

void
WebSocket::on_message(connection_hdl h, message_ptr m)
{
    std::cout << "MSG" << m->get_payload() << std::endl;
    nlohmann::json j;
    j["hage"] = "M";
    send(h, j.dump(), frame::opcode::text);
    close(h, close::status::normal, "A");
}

}