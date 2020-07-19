/// @file Users.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the Users tool. Proprietary and confidential.
/// See the licenses directory for details.
#include "labo/server/WS.h"
#include "nlohmann/json.hpp"
#include <future>
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>
#include <labo/house/Users.h>
#include <labo/util/keys.h>
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
            ws.pause_reading(c);
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

    if (type == "set_himado") {
        if (args.is_null()) {
            logs << "Args missing." << endl;
            return;
        }
        auto himado{ args["himado"] };
        if (himado.is_null() || !himado.is_number_unsigned()) {
            logs << "Invalid Himado." << endl;
            return;
        }

        change_status(usr,
                      static_cast<User::Status>(static_cast<uint>(himado)));
        return;
    }
    if (type == "chat") {
        if (args.is_null()) {
            logs << "Args missing." << endl;
            return;
        }
        auto msg{ args["msg"] };
        if (msg.is_null()) {
            logs << "Invalid msg." << endl;
            return;
        }

        main_chat.chat(usr, msg);
        return;
    }

    return;
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
  , main_chat{ *this, 0, "main" }
{}

void
LaboHouse::log_in(User& u, Connection c)
{
    {
        unique_lock lg{ mtx_online };
        online.insert({ c, &u });
        ronline.insert({ &u, c });
        logs << "[LaboHouse] User logged in :" << u.id << endl;
    }
    // Send all data on login.
    // Chat
    {
        Json j;
        j["type"] = "chat";
        j["chat"] = main_chat.to_json();
        send(u, j);
    }

    // Users
    {
        Json j;
        j["type"] = "names";
        j["names"] = users.to_json();
        send(u, j);
    }
}

void
LaboHouse::log_out(Connection c)
{
    unique_lock lg{ mtx_online };
    auto itr{ online.find(c) };
    if (itr == online.end()) {
        logs << "[LaboHouse] : Closed connection with UNKNOWN>" << endl;
        return;
    }
    itr->second->status = User::Status::sOffline;
    logs << "[LaboHouse] User logged out: " << itr->second->id << endl;
    ronline.erase(itr->second);
    online.erase(itr);
}

void
LaboHouse::start()
{
    thread t{ [&]() { ws.start(42069); } };
    html.start(12345);
}

void
LaboHouse::send(User& u, Json j)
{
    auto itr{ ronline.find(&u) };
    if (itr == ronline.end()) {
        errs << "Missing user: " << u.id << endl;
        return;
    }
    try {
        ws.send(to_string(j), itr->second);
    } catch (Json::exception e) {
        errs << "Failed to send: " << e.what() << endl;
        return;
    }
}

void
LaboHouse::change_status(User& u, User::Status s)
{
    u.status = s;
    auto j{ u.to_json() };
    j["type"] = "himado";
    send_online(j);
}

void
LaboHouse::send_online(Json j)
{
    shared_lock lg{ mtx_online };
    for (auto [c, u] : online) {
        send(*u, j);
    }
}
};