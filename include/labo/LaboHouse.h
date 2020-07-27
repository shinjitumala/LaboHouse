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

    shared_mutex mtx_online;
    map<Connection, User*, owner_less<Connection>> online;
    unordered_map<User*, Connection> ronline;

    pair<string, string> quote;

  public:
    Chats chats;

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
    /// @param os void print(ostream& os) const;

    /// Send a message to a user.
    /// @param u
    /// @param j
    void send(User& u, Json j);

    /// Notify user with message.
    /// @param u
    /// @param m
    void notify(User& u, string m);

    /// Send message to users.
    /// @param m
    template<
      class I,
      enable_if_t<is_same_v<typename iterator_traits<I>::value_type, User*>,
                  bool> = true>
    void send_all(I begin, I end, Json j);
    template<
      class I,
      enable_if_t<is_same_v<typename iterator_traits<I>::value_type, User>,
                  bool> = true>
    void send_all(I begin, I end, Json j);

    /// Send message to all online users.
    /// @param j
    void send_online(Json j);

    /// Send notifications to online watchers for user 'u'.
    /// @param u
    /// @param msg
    void notify_watchers(User& u, string msg);

    /// Attempt to immediately terminate connections.
    void terminate();

  private:
    /// Send names to user.
    /// @param u
    void send_names(User& u) const;

    void log_in(User& u, Connection h);
    void log_out(Connection h);

    void change_status(User& u, User::Status s);
    void change_status(User& u, string subhimado);
    void broadcast_status(User& u);

    void send_watchlist(User &u);
    void send_timeranges(User &u);
    void send_timer(User &u);
};

template<
  class I,
  enable_if_t<is_same_v<typename iterator_traits<I>::value_type, User*>, bool>>
void
LaboHouse::send_all(I begin, I end, Json j)
{
    for_each(begin, end, [this, &j](auto u) { send(*u, j); });
}

template<
  class I,
  enable_if_t<is_same_v<typename iterator_traits<I>::value_type, User>, bool>>
void
LaboHouse::send_all(I begin, I end, Json j)
{
    for_each(begin, end, [this, &j](auto& u) { send(u, j); });
}
};