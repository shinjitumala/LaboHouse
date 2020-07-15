/// @file Users.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the Users tool. Proprietary and confidential.
/// See the licenses directory for details.
#include "labo/server/WS.h"
#include "nlohmann/json.hpp"
#include "websocketpp/close.hpp"
#include <future>
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>
#include <labo/house/Users.h>
#include <mutex>
#include <sstream>

namespace labo {
void
LaboHouse::request(Json j, Connection c)
{
    logs << "[LaboHouse] Request: " << j.dump(2) << endl;
    auto type{ j["type"] };
    if (type.is_null()) {
        logs << "Request type missing." << endl;
        return;
    }
    auto args{ j["args"] };

    if (type == "cookie") {
        if (args.is_null()) {
            logs << "Args missing." << endl;
            return;
        }

        auto cookie{ args["Cookie"] };
        if (cookie.is_null() || !cookie.is_string()) {
            logs << "Invalid Cookie." << endl;
            return;
        }

        auto ousr{ users.by_cookie(cookie) };
        if (!ousr) {
            ws.close(c, websocketpp::close::status::normal, "Unknown Cookie.");
            logs << "Unknown Cookie." << endl;
            return;
        }

        log_in(ousr.get(), c);
        return;
    }

    auto itr{ online.find(c) };
    if (itr == online.end()) {
        // If user not found.
        ws.close(c, websocketpp::close::status::normal, "Unknown handle.");
        logs << "Unknown handle." << endl;
        return;
    }
    // User is known
    auto& usr{ *itr->second };

    if (type == "setHimado") {
        if (args.is_null()) {
            logs << "Args missing." << endl;
            return;
        }
        auto himado{ args["himado"] };
        if (himado.is_null() || !himado.is_number_unsigned()) {
            logs << "Invalid Himado." << endl;
            return;
        }

        usr.status = static_cast<User::Status>(static_cast<uint>(himado));
        // TODO: Broadcast.
        return;
    }
    if (type == "getNames") {
    }

    return;
}

const Chat::Msg&
LaboHouse::chat(User& user, string msg)
{
    return main_chat.chat(user, msg);
}

const Chat&
LaboHouse::get_main_chat() const
{
    return main_chat;
};

void
LaboHouse::send_names(User& u) const
{
    async([]() {
        // ws.send_to(
    });
}

LaboHouse::LaboHouse()
  : html{ *this }
  , ws{ [&](auto m, auto c) {
           istringstream iss{ m };
           Json j;
           try {
               iss >> j;
           } catch (nlohmann::json::parse_error e) {
               logs << "JSON parsing error: " << e.what() << endl;
               return;
           }
           request(j, c);
       },
        [&](auto c) { log_out(c); } }
{}

void
LaboHouse::log_in(User& u, Connection c)
{
    lock_guard lg{ mtx_online };
    online.insert({ c, &u });
}

void
LaboHouse::log_out(Connection c)
{
    lock_guard lg{ mtx_online };
    online.erase(c);
}

void
LaboHouse::start()
{
    thread t{ [&]() { ws.start(42069); } };
    html.start(12345);
}
};