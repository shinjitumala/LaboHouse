/// @file Users.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the Users tool. Proprietary and confidential.
/// See the licenses directory for details.
#include "labo/house/Quotes.h"
#include "labo/server/WS.h"
#include "nlohmann/json.hpp"
#include <future>
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>
#include <labo/house/Users.h>
#include <mutex>
#include <sstream>

namespace labo {
using namespace websocketpp::close;
mutex mtx;
void
LaboHouse::request(Json j, Connection c)
{
    lock_guard lg{ mtx };
    logs << "[LaboHouse] Request: " << j.dump(2) << endl;
    auto type{ j["type"] };
    if (type.is_null() || !type.is_string()) {
        logs << "Request type invalid: " << type << endl;
        return;
    }

    if (type == "cookie") {
        auto cookie{ j["cookie"] };
        if (cookie.is_null() || !cookie.is_string()) {
            ws.close(c, status::normal, "Unknown Cookie.");
            logs << "Invalid Cookie." << endl;
            return;
        }

        auto ousr{ users.by_cookie(cookie) };
        if (!ousr) {
            ws.close(c, status::normal, "Unknown Cookie.");
            logs << "Unknown Cookie." << endl;
            return;
        }

        log_in(ousr.get(), c);
        return;
    }

    auto itr{ online.find(c) };
    if (itr == online.end()) {
        // If user not found.
        ws.close(c, status::normal, "Unknown handle.");
        logs << "Unknown handle." << endl;
        return;
    }
    // User is known
    auto& usr{ *itr->second };
    logs << "[User: " << usr.id << "] " << flush;

    if (type == "himado") {
        auto himado{ j["himado"] };
        if (himado.is_null() || !himado.is_number_unsigned()) {
            logs << "Invalid Himado: " << himado << endl;
            return;
        }

        change_status(usr,
                      static_cast<User::Status>(static_cast<uint>(himado)));
        return;
    }
    if (type == "chat") {
        auto msg{ j["msg"] };
        if (msg.is_null() || !msg.is_string()) {
            logs << "Invalid msg: " << msg << endl;
            return;
        }

        main_chat.chat(usr, msg);
        return;
    }

    if (type == "add_watchlist" || type == "remove_watchlist") {
        auto id{ j["id"] };
        if (id.is_null() || !id.is_string()) {
            logs << "Invalid id: " << id << endl;
            return;
        }
        auto ouser{ users.by_id(id) };
        if (!ouser) {
            logs << "User not found: " << id << endl;
            return;
        }
        if (&ouser.get() == &usr) {
            logs << "Cannot add self to watchlist." << endl;
            notify(usr, "Cannot add self to watchlist.");
            return;
        }

        if (type == "add_watchlist") {
            usr.watchlist_add(ouser.get());
            logs << "[User:" << usr.id
                 << "] Added user to watchlist: " << ouser.get().id << endl;
            return;
        } else if (type == "remove_watchlist") {
            usr.watchlist_remove(ouser.get());
            logs << "[User:" << usr.id
                 << "] Removed user from watchlist: " << ouser.get().id << endl;
            return;
        }
    }

    errs << "[LaboHouse][User:" << usr.id << "] Unknown type: " << type << endl;
    return;
}

LaboHouse::LaboHouse()
  : html{ *this }
  , ws{ [&](auto m, auto c) {
           istringstream iss{ m };
           Json j;
           try {
               iss >> j;
           } catch (Json::parse_error e) {
               logs << "JSON parsing error: " << e.what() << endl;
               return;
           }
           request(j, c);
       },
        [&](auto c) { log_out(c); } }
  , quote{ Quotes::get() }
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

    // Quote
    {
        Json j;
        j["type"] = "quote";
        j["author"] = quote.first;
        j["quote"] = quote.second;
        send(u, j);
    }

    change_status(u, User::Status::sFree);
    main_chat.chat(u, "has logged in.");

    // Send notification
    for (auto& ou : users) {
        if (ou.in_watchlist(u)) {
            notify(ou, u.name + " is online!.");
        }
        if (ou.status == User::Status::sEasy) {
            // Notify if on easy mode.
            notify(ou, u.name + "is online!.");
        }
    }
}

void
LaboHouse::log_out(Connection c)
{
    unique_lock lg{ mtx_online };
    auto itr{ online.find(c) };
    if (itr == online.end()) {
        logs << "[LaboHouse] : Closed connection with UNKNOWN." << endl;
        return;
    }
    auto& u{ *itr->second };
    ronline.erase(itr->second);
    online.erase(itr);
    lg.unlock();
    u.status = User::Status::sOffline;
    main_chat.chat(u, "has logged out.");
    logs << "[LaboHouse] User logged out: " << u.id << endl;
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
        errs << "User offline: " << u.id << endl;
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
    for (auto [c, u] : online) {
        send(*u, j);
    }
}

void
LaboHouse::notify(User& u, string m)
{
    Json j;
    j["type"] = "notification";
    j["msg"] = m;
    send(u, j);
}
};