/// @file Chat.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <chrono>
#include <list>
#include <nlohmann/json.hpp>
#include <string>

namespace labo {
using namespace std;

class LaboHouse;
class User;

/// Basic chat functionality.
class Chat
{
  public:
    /// A single message.
    struct Msg
    {
        /// Who said it.
        User* user;
        /// What time it was said.
        chrono::system_clock::time_point time;
        /// The content of the message.
        string content;

        /// Print the message.
        /// @param os
        void print(ostream& os) const;

        /// Convert to json.
        nlohmann::json to_json() const;
    };

  private:
    LaboHouse& lh;

    long id;
    string name;

    /// Maximum number of messages before we start discarding them.
    static const uint max{ 64 };
    list<Msg> messages;

  public:
    /// Create a new chat.
    /// @param lh 
    /// @param id 
    /// @param name 
    Chat(decltype(lh) lh, decltype(id) id, decltype(name) name);

    /// Post a chat.
    /// @param user Who said it.
    /// @param msg The content.
    /// @return const Msg& The created chat message.
    const Msg& chat(User& user, string msg);

    /// Print the entire chat.
    /// @param os
    void print(ostream& os) const;

    /// Convert to json.
    nlohmann::json to_json() const;
};
};