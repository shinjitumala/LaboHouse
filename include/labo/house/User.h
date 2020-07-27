/// @file User.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <chrono>
#include <iomanip>
#include <labo/util/stream.h>
#include <mutex>
#include <nlohmann/json.hpp>
#include <sstream>
#include <unordered_set>

namespace labo {
using namespace std;
using ulong = unsigned long;
using Json = nlohmann::json;
using namespace chrono;

class LaboHouse;

/// Holds the data relevant to the user.
class User
{
  public:
    // A time in a day.
    struct Time
    {
        hours h;
        minutes m;
        bool operator==(const Time& rhs) const
        {
            return h == rhs.h && m == rhs.m;
        };
        bool operator<(const Time& rhs) const
        {
            return h < rhs.h || m < rhs.m;
        };
        bool operator<=(const Time& rhs) const
        {
            return *this == rhs || *this < rhs;
        }

        static Time now();

        void print(ostream& os) const
        {
            os << setfill('0') << setw(2) << h.count() << ":" << setw(2)
               << m.count();
        }
    };
    enum Status : unsigned char
    {
        sFree,    // Doing nothing
        sEasy,    // Working but available for some chit-chat
        sBusy,    // Do not disturb
        sOffline, // Offline. (For future use.)
        sLast,    // Final element
    };
    /// Time where the himado is automatically set.
    struct TimeRange
    {
        Time start;
        Time end;
        Status status;
        bool operator==(const TimeRange& rhs) const
        {
            return start == rhs.start && end == rhs.end;
        };
        operator bool() const
        {
            // If start is greater than end, invalid.
            return start < end;
        };

        void print(ostream& os) const
        {
            os << start << " ~ " << end << " (" << to_string(status) << ")";
        }

        Json to_json() const
        {
            Json j;
            j["start"] = labo::to_string(start);
            j["end"] = labo::to_string(end);
            j["himado"] = User::to_string(status);
            return j;
        }
    };
    struct Timer : Time
    {
        Status s;
        Timer(minutes m, Status s);
        bool valid;
        bool expired(Time t);

        void print(ostream& os) const
        {
            os << "{" << static_cast<const Time&>(*this) << ", " << to_string(s)
               << "}";
        };

        Json to_json()
        {
            Json j;
            j["time"] = labo::to_string(static_cast<Time>(*this));
            j["himado"] = User::to_string(s);
            return j;
        };
    };

    /// Convert status to string.
    /// @param s
    /// @return string
    static string to_string(Status s);

    /// Student id
    const string id;
    /// Identifier for the user. Also used as the cookie. (FIXME: Vulnerable)
    const string cookie;

    /// Display name
    string name;
    /// level for freeness (is that a word?)
    Status status;

    /// Receive notification when users here become online.
    unordered_set<const User*> watchlist;

    /// List of timeranges.
    vector<TimeRange> timeranges;

  public:
    /// Sub status.
    string substatus;
    optional<Timer> timer;

    /// A user must have an id and a name.
    /// @param id
    /// @param cookie
    User(const string id, const string cookie);

    struct hash
    {
        /// Get hash.
        /// @param u
        /// @return size_t
        size_t operator()(const User& u) const;
    };

    /// @param rhs
    /// @return true If this is the same instance as rhs.
    /// @return false
    bool operator==(const User& rhs) const;

    /// Obtain this as json.
    /// @return Json
    Json to_json() const;

    /// Allow move.
    User(User&&) = default;

    bool in_watchlist(const User& u) const;
    void watchlist_add(const User& u);
    void watchlist_remove(const User& u);
    Json get_watchlist() const;

    string timerange_add(string start, string end, User::Status s);
    string timerange_remove(string start, string end);
    Json get_timeranges() const;
    /// Returns a valid status if in timerange.
    /// @param t
    /// @return optional<Status>
    optional<Status> timerange_query(Time t);

    string tag() const { return "[User:" + id + "]"; };

  private:
    /// Disallow copy because users are supposed to be unique.
    User(const User&) = delete;
};
};