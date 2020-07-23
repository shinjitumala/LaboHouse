/// @file User.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>
#include <labo/house/User.h>
#include <nlohmann/json.hpp>
#include <regex>
#include <sstream>

namespace labo {
User::User(const string id, const string cookie)
  : id{ id }
  , cookie{ cookie }
  , status{ sFree }
{}

string
User::to_string(Status s)
{
    switch (s) {
        case sFree:
            return "Free";
        case sEasy:
            return "Easy";
        case sBusy:
            return "Busy";
        case sOffline:
            return "Offline";
        default:
            errs << "Invalid 'HIMADO': " << static_cast<uint>(s) << endl;
            failure();
    }
}

size_t
User::hash::operator()(const User& u) const
{
    return std::hash<string>()(u.cookie);
}

bool
User::operator==(const User& rhs) const
{
    return this == &rhs;
}

nlohmann::json
User::to_json() const
{
    nlohmann::json j;
    j["name"] = name;
    j["id"] = id;
    j["himado"] = to_string(status);
    j["substatus"] = substatus;
    return j;
}

bool
User::in_watchlist(const User& u) const
{
    return watchlist.count(&u);
};

void
User::watchlist_add(const User& u)
{
    watchlist.insert(&u);
};

void
User::watchlist_remove(const User& u)
{
    auto itr{ watchlist.find(&u) };
    if (itr == watchlist.end()) {
        logs << "[Uesr:" << id << "] Not in watchlist: " << u.id << endl;
        return;
    }
    watchlist.erase(itr);
};

auto parse_time = [](string time) -> optional<User::Time> {
    static const regex r{ "([0-9][0-9]):([0-9][0-9])" };
    smatch m;
    if (!regex_match(time, m, r) || m.size() != 3) {
        logs << "Failed to match regex: " << time << endl;
        return {};
    }
    return User::Time{ hours{ stoi(m[1]) }, minutes{ stoi(m[2]) } };
};
auto get_timerange = [](string start, string end) -> optional<User::TimeRange> {
    auto tstart{ parse_time(start) };
    auto tend{ parse_time(end) };
    if (!tstart || !tend) {
        return {};
    }

    return User::TimeRange{ *tstart, *tend };
};

string
User::timerange_add(string start, string end, User::Status s)
{
    auto tr{ get_timerange(start, end) };
    if (!tr || !*tr) {
        return "Invalid timerange: start must be earlier than the end.";
    }

    for (auto otr : timeranges) {
        if (!((tr->end <= otr.start && tr->start <= otr.start) ||
              (otr.end <= tr->start && otr.start <= tr->start))) {
            return "Conflicts with existing timerange: " + labo::to_string(otr);
        }
    }

    tr->status = s;
    timeranges.push_back(*tr);
    logs << "[User:" << id << "] New timerage: " << *tr << endl;
    return "";
};

string
User::timerange_remove(string start, string end)
{
    auto tr{ get_timerange(start, end) };
    if (!tr || !*tr) {
        return "Invalid timerange.";
    }
    auto itr{ find_if(timeranges.begin(), timeranges.end(), [&](auto otr) {
        return otr == *tr;
    }) };
    if (itr == timeranges.end()) {
        return "No such TimeRange.";
    }
    logs << "[User:" << id << "] Removed timerage: " << *itr << endl;
    timeranges.erase(itr);
    return "";
};

optional<User::Status>
User::timerange_query(Time t)
{
    for (auto tr : timeranges) {
        if (tr.start <= t && t <= tr.end) {
            logs << tag() << " In Timerange: " << tr << endl;
            return tr.status;
        }
    }
    return {};
};

User::Time
User::Time::now()
{
    auto tp{ time(nullptr) };
    auto time{ *localtime(&tp) };
    return { hours{ time.tm_hour }, minutes{ time.tm_min } };
}

User::Timer::Timer(minutes m, Status s)
  : Time{ [&] {
      auto now{ Time::now() };
      if (now.m.count() + m.count() > 60) {
          now.h++;
      }
      now.m += m;
      return now;
  }() }
  , s{ s }
{}

bool
User::Timer::expired(Time t)
{
    return *this < t;
}
}