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
#include <chrono>
#include <future>
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>
#include <labo/house/Users.h>
#include <mutex>
#include <sstream>

namespace labo {
using namespace websocketpp::close;
mutex mtx;
const string lh_tag{ "[LaboHouse]" };
void
LaboHouse::request(Json j, Connection c)
{
    try {
        auto tag{ lh_tag };
        lock_guard lg{ mtx };
        auto type{ j["type"] };
        if (type == "cookie") {
            auto cookie{ j["cookie"] };
            if (cookie.is_null() || !cookie.is_string()) {
                ws.close(c, status::normal, "Unknown Cookie.");
                logs << tag << " Invalid Cookie." << endl;
                return;
            }

            auto ousr{ users.by_cookie(cookie) };
            if (!ousr) {
                ws.close(c, status::normal, "Unknown Cookie.");
                logs << tag << " Unknown Cookie." << endl;
                return;
            }

            log_in(ousr.get(), c);
            return;
        }

        auto itr{ online.find(c) };
        if (itr == online.end()) {
            // If user not found.
            ws.close(c, status::normal, "Unknown handle.");
            logs << tag << " Unknown handle." << endl;
            return;
        }
        // User is known
        auto& usr{ *itr->second };
        tag += usr.tag();
        if (type == "himado") {
            change_status(
              usr, static_cast<User::Status>(static_cast<uint>(j["himado"])));
            return;
        }
        if (type == "subhimado") {
            change_status(usr, static_cast<string>(j["subhimado"]));
            return;
        }
        if (type == "chat") {
            auto msg{ j["msg"] };
            if (msg.is_null() || !msg.is_string()) {
                logs << tag << "Invalid msg: " << msg << endl;
                return;
            }

            if (auto c{ chats.get(j["chat"]) }) {
                c->chat(usr, msg);
            } else {
                logs << tag << "Unknown chat: " << j["chat"] << endl;
            }
            return;
        }

        if (type == "add_watchlist" || type == "remove_watchlist") {
            auto id{ j["id"] };
            auto ouser{ users.by_id(id) };
            if (!ouser) {
                logs << tag << "User not found: " << id << endl;
                return;
            }
            if (&ouser.get() == &usr) {
                logs << tag << "Cannot add self to watchlist." << endl;
                notify(usr, "Cannot add self to watchlist.");
                return;
            }

            if (type == "add_watchlist") {
                usr.watchlist_add(ouser.get());
                logs << tag << " Added user to watchlist: " << ouser.get().id
                     << endl;
                return;
            } else if (type == "remove_watchlist") {
                usr.watchlist_remove(ouser.get());
                logs << tag
                     << " Removed user from watchlist: " << ouser.get().id
                     << endl;
                return;
            }
        }

        if (type == "add_timerange") {
            if (string status{ usr.timerange_add(
                  j["start"],
                  j["end"],
                  static_cast<User::Status>(
                    static_cast<unsigned char>(j["himado"]))) };
                status.size()) {
                notify(usr, status);
                logs << tag << " " << status;
            }
            return;
        }
        if (type == "remove_timerange") {
            if (string status{ usr.timerange_remove(j["start"], j["end"]) };
                status.size()) {
                notify(usr, status);
                logs << tag << " " << status;
            }
            return;
        }
        if (type == "add_timer") {
            usr.timer = { minutes{ static_cast<uint>(j["duration"]) },
                          static_cast<User::Status>(
                            static_cast<unsigned char>(j["himado"])) };
            logs << tag << " Added timer: " << *usr.timer << "." << endl;
            return;
        }
        if (type == "remove_timer") {
            logs << tag << " Removed timer." << endl;
            usr.timer = {};
            return;
        }

        if (type == "rename") {
            usr.name = j["name"];
            logs << tag << "User was renamed to " << j["name"] << endl;
            broadcast_status(usr);
            return;
        }

        if (type == "substatus") {
            usr.substatus = j["substatus"];
            logs << tag << "User's substatus was changed to: " << j["substatus"]
                 << endl;
            broadcast_status(usr);
            return;
        }

        errs << tag << " Unknown type: " << type << endl;
    } catch (Json::exception e) {
        logs << lh_tag << "Json error: " << e.what() << endl;
        logs << lh_tag << "Message: " << j.dump() << endl;
    };
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
  , chats{ *this }
{
    chats.add("All");
    chats.add("Free");
    chats.add("Easy");
    chats.add("Busy");
}

void
LaboHouse::log_in(User& u, Connection c)
{
    {
        unique_lock lg{ mtx_online };
        online.insert({ c, &u });
        ronline.insert({ &u, c });
        logs << "[LaboHouse] User logged in :" << u.id << endl;
    }

    // Users
    {
        Json j;
        j["type"] = "names";
        j["names"] = users.to_json();
        send(u, j);
    }

    // Chat
    for (auto& [name, c] : chats) {
        auto j{ c.to_json() };
        j["type"] = "chat";
        send(u, j);
    }

    // Quote
    {
        Json j;
        auto quote{ Quotes::get() };
        j["type"] = "quote";
        j["author"] = quote.first;
        j["quote"] = quote.second;
        send(u, j);
    }

    change_status(u, User::Status::sFree);
    chats.get("All")->chat(u, "has logged in.");

    // Send notification
    notify_watchers(u, u.name + " logged in!.");
}

void
LaboHouse::notify_watchers(User& u, string msg)
{
    for (auto& ou : users) {
        if (!ou.in_watchlist(u)) {
            continue;
        }
        if (ou.status < User::Status::sBusy) {
            // Notify if not busy.
            notify(ou, msg);
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
    change_status(u, User::Status::sOffline);
    chats.get("All")->chat(u, "has logged out.");
    logs << "[LaboHouse] User logged out: " << u.id << endl;
}

void
LaboHouse::start()
{
    thread t{ [&]() { ws.start(42069); } };
    // 1 minute loop.
    thread t2{ [&]() {
        while (true) {
            this_thread::sleep_for(3s);
            logs << "[LaboHouse] Checking for timeranges..." << endl;
            shared_lock sl{ mtx_online };
            auto now{ User::Time::now() };
            for (auto [c, u] : online) {
                auto s{ u->timerange_query(now) };
                if (!s) {
                    continue;
                }
                change_status(*u, *s);
            }
            for (auto [c, u] : online) {
                if (!u->timer) {
                    continue;
                }
                if (!u->timer->expired(now)) {
                    continue;
                }
                logs << lh_tag << u->tag() << "Timer expired: " << *u->timer
                     << endl;
                change_status(*u, u->timer->s);
                u->timer = {};
            }
            logs << "[LaboHouse] Done!" << endl;
        };
    } };
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
    // Send notification to watcher.
    if (s == User::Status::sFree) {
        notify_watchers(u, u.name + " is '" + User::to_string(s) + "'.");
    }
    broadcast_status(u);
}

void
LaboHouse::broadcast_status(User& u)
{
    auto j{ u.to_json() };
    j["type"] = "himado";
    j["self"] = false;
    send_online(j);
    j["self"] = true;
    send(u, j);
}

void
LaboHouse::change_status(User& u, string subhimado)
{
    u.substatus = subhimado;
    logs << lh_tag << u.tag() << " Status changed (sub): " << u.substatus
         << endl;
    broadcast_status(u);
}

void
LaboHouse::send_online(Json j)
{
    shared_lock sl{ mtx_online };
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