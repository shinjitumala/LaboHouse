/// @file Chat.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <chrono>
#include <iomanip>
#include <labo/LaboHouse.h>
#include <labo/house/Chat.h>
#include <labo/house/User.h>
#include <labo/util/stream.h>
#include <nlohmann/json.hpp>
#include <ostream>
#include <sstream>

namespace labo {
using namespace chrono;
void
Chat::Msg::print(ostream& os) const
{
    auto time_t{ system_clock::to_time_t(time) };
    os << "[" << put_time(localtime(&time_t), "%Y-%m-%d_%X") << "] ";
    os << user->name << " ";
    os << content;
};

nlohmann::json
Chat::Msg::to_json() const
{
    Json j;
    {
        ostringstream oss;
        auto time_t{ system_clock::to_time_t(time) };
        oss << "[" << put_time(localtime(&time_t), "%Y-%m-%d_%X") << "] ";
        j["time"] = oss.str();
    }
    j["user"] = user->to_json();
    j["msg"] = content;
    return j;
}

Chat::Chat(decltype(lh) lh, decltype(id) id, decltype(name) name)
  : lh{ lh }
  , id{ id }
  , name{ name }
{}

const Chat::Msg&
Chat::chat(User& user, string msg)
{
    if (messages.size() >= max) {
        messages.pop_front();
    }
    messages.push_back({ &user, system_clock::now(), msg });

    auto j{ messages.back().to_json() };
    j["type"] = "new_chat";
    lh.send_online(j);

    // Look for mentions
    for (auto s{ User::Status::sFree }; s < User::Status::sLast;
         s = static_cast<User::Status>(s + 1)) {
        string msg{ j["msg"] };
        if (msg.find("@" + User::to_string(s)) == string::npos) {
            continue;
        };
        for (auto& u : lh.users) {
            if (u.status != s) {
                continue;
            }
            lh.notify(u, "You mave been mentioned: @" + User::to_string(s));
        }
    }

    return messages.back();
};

void
Chat::print(ostream& os) const
{
    for (auto m : messages) {
        os << m << endl;
    }
}

nlohmann::json
Chat::to_json() const
{
    nlohmann::json j{ nlohmann::json::array() };
    for (auto m : messages) {
        j.push_back(m.to_json());
    }
    return j;
};
};