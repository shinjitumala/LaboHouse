/// @file LaboHouse.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include "labo/server/Html.h"
#include "labo/server/WS.h"
#include "nlohmann/json.hpp"
#include <labo/house/Chat.h>
#include <labo/house/Users.h>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace labo {
using namespace std;

class LaboHouse
{
  public:
    using Json = nlohmann::json;
    using Connection = websocketpp::connection_hdl;

  private:
    /// The html server used.
    server::Html html;
    /// Websocket API.
    server::WebSocket ws;

    mutex mtx_online;
    map<Connection, User*, owner_less<Connection>> online;

    Chat main_chat;

  public:
    /// All known users.
    Users users;

    /// Default constructor.
    LaboHouse();

    /// Start the server. Does not return.
    void start();

    /// Handles a request in json format.
    /// @param j
    /// @param c
    void request(Json j, Connection c);

    /// Dump contents for debugging.
    /// @param os
    void print(ostream& os) const;

    /// Chat to the main chat.
    /// @param user
    /// @param msg
    const Chat::Msg& chat(User& user, string msg);

    /// Obtain main chat for printing.
    /// @return const Chat&
    const Chat& get_main_chat() const;

  private:
    /// Send names to user.
    /// @param u
    void send_names(User& u) const;

    void log_in(User& u, Connection h);
    void log_out(Connection h);
};
};