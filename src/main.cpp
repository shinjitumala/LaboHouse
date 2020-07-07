/// @file main.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <ratio>
#if __has_include(<filesystem>)
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#include <fstream>
#include <iostream>
#include <labo/LaboHouse.h>
#include <labo/debug/Log.h>
#include <labo/house/User.h>
#include <labo/server/Base.h>
#include <labo/server/http/Html.h>
#include <labo/server/http/Request.h>
#include <labo/server/http/Response.h>
#include <labo/server/http/ResponseHelper.h>
#include <labo/util/fdstreambuf.h>
#include <limits>
// #include <llvm/Support/CommandLine.h>
#include <regex>
#include <signal.h>

// namespace labo::cl {
// using namespace std;
// using namespace llvm::cl;

// /// List used commandline options here.
// const vector<const OptionCategory*> related_categories{};
// };

labo::LaboHouse labohouse{};

std::mutex mtx;

struct Action
{
    Action(int socket_fd)
    {
        std::lock_guard<std::mutex> lg{ mtx };
        using namespace labo;
        using namespace http;
        socket::fdstreambuf stream{ socket_fd };
        istream in{ &stream };
        ostream out{ &stream };

        Request req;
        in >> req;

        auto is_bad_request{ false };
        vector<string> missing_values;
        /// Convenience function for obtaining header values.
        auto get_header = [&](const string value_name) -> string {
            auto opt{ req.header_value(value_name) };
            if (!opt) {
                is_bad_request = true;
                missing_values.push_back(value_name);
                return "";
            }
            return opt.get();
        };
        /// Must be called after obtaining all required values with 'get_header'
        auto check_request = [&]() {
            if (is_bad_request) {
                ostringstream oss;
                oss << "Values missing: { ";
                for (auto v : missing_values) {
                    oss << v << ", ";
                }
                oss << "}";
                string error{ oss.str() };
                out << bad_request(error) << endl << endl;
                errs << "[Action] ERROR: Bad request, " << error << endl;
            }
            return is_bad_request;
        };

        auto path{ req.path() };
        switch (req.method()) {
            case Request::Method::GET:
                if (path == "/") {
                    /// reply with home page
                    out << Response{
                        Response::Status::OK,
                        Html{ "../res/home.html" },
                    };
                    logs << "[Action] Homepage access." << endl;
                    return;
                }
                break;
            case Request::Method::POST:
                if (path == "/register") {
                    // Register new name
                    auto id{ get_header("id") };
                    auto display_name{ get_header("display_name") };

                    // Mandatory check.
                    if (check_request()) {
                        return;
                    }

                    if (labohouse.get(id)) {
                        out << forbidden(
                          "Name already exists. Please choose another one");
                        errs << "[Action] ERROR: Name already exists, " << id
                             << endl;
                        return;
                    }

                    auto& user{ labohouse.Users::add(id, display_name) };
                    out << Response{ Response::Status::OK,
                                     { { "Set-Cookie", user.id },
                                       { "name", user.display_name } } };

                    labohouse.chat(user, " has joined the chat!");

                    logs << "[Action] New user added." << endl;
                    return;
                }
                if (path == "/name") {
                    auto cookie{ get_header("Cookie") };

                    // Mandatory check.
                    if (check_request()) {
                        return;
                    }

                    auto opt{ labohouse.Users::get_from_id(cookie) };
                    if (!opt) {
                        out << forbidden("Invalid cookie.");
                        errs << "[Action] ERROR: Invalid cookie, " << cookie
                             << endl;
                        return;
                    }

                    auto& user{ opt.get() };
                    logs << "Name for user " << user.id << " is "
                         << user.display_name << endl;
                    out << Response{ Response::Status::OK,
                                     { { "name", user.display_name } } };

                    logs << "[Action] Name of user " << user.id << " is "
                         << user.display_name << endl;
                    return;
                }

                if (path == "/sethimado") {
                    auto himado{ get_header("Himado") };
                    auto cookie{ get_header("Cookie") };

                    // Mandatory check.
                    if (check_request()) {
                        return;
                    }

                    auto opt{ labohouse.Users::get_from_id(cookie) };
                    if (!opt) {
                        out << forbidden("User does not exist: id = " + cookie);
                        return;
                    }
                    auto& user{ opt.get() };
                    auto status{ static_cast<User::Status>(stoul(himado)) };
                    user.status = status;
                    out << Response{ Response::Status::OK };

                    logs << "[User] " << user.display_name
                         << "'s himado was changed to '"
                         << User::to_string(status) << "'." << endl;
                    return;
                }

                if (path == "/gethimado") {
                    auto cookie{ get_header("Cookie") };

                    // Mandatory check.
                    if (check_request()) {
                        return;
                    }

                    auto opt{ labohouse.Users::get_from_id(cookie) };
                    if (!opt) {
                        out << forbidden("User does not exist: id = " + cookie);
                        return;
                    }
                    auto& user{ opt.get() };

                    auto himado{ User::to_string(user.status) };
                    out << Response{ Response::Status::OK,
                                     { { "Himado", himado } } };
                    logs << "[User] " << user.display_name
                         << " obtained his/her himado which is '" << himado
                         << "'." << endl;
                    return;
                }

                if (path == "/names") {
                    out << Response{ Response::Status::OK,
                                     labohouse.Users::to_json() };
                    return;
                }

                if (path == "/names_sorted") {
                    out << Response{ Response::Status::OK,
                                     labohouse.Users::to_json_sorted() };
                    return;
                }

                if (path == "/echo") {
                    auto text{ get_header("text") };

                    // Mandatory check.
                    if (check_request()) {
                        return;
                    }

                    logs << "[Echo] " << text << endl;
                    return;
                }

                if (path == "/chat_main") {
                    auto cookie{ get_header("Cookie") };
                    auto str{ get_header("message") };

                    // Mandatory check.
                    if (check_request()) {
                        return;
                    }

                    auto opt{ labohouse.Users::get_from_id(cookie) };
                    if (!opt) {
                        out << forbidden("User does not exist: id = " + cookie);
                        return;
                    }
                    auto& user{ opt.get() };
                    auto msg{ labohouse.chat(user, str) };

                    logs << msg << endl;

                    out << Response{ Response::Status::OK };

                    return;
                }

                if (path == "/chat_main_get") {
                    out << Response{ Response::Status::OK,
                                     labohouse.get_main_chat().to_json() };
                    return;
                }

                break;
        }

        out << not_found("Invalid URL.");
        errs << "[Action] ERROR: Invalid URL." << endl;
    }
};

labo::Server<Action>* server{ nullptr };

void
signal_handler(int sig)
{
    using namespace labo;
    errs << "Signal received: " << strsignal(sig) << endl;
    if (server) {
        errs << "Terminating..." << endl;
        delete server;
        logs << "Server terminated." << endl;
    } else {
        errs << "Server was not running." << endl;
    }
    logs << "Goodbye." << endl;

    ::signal(sig, SIG_DFL);
    ::kill(::getpid(), sig);
};

int
main(int argc, char* argv[])
{
    // Command line stuff. It's magic.
    // llvm::cl::HideUnrelatedOptions(labo::cl::related_categories);
    // llvm::cl::ParseCommandLineOptions(argc, argv);

    signal(SIGINT, signal_handler);

    server = new labo::Server<Action>{ 12345 };
    server->start();
    return 0;
}