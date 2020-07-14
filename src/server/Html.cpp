/// @file Html.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the SWORD tool. Proprietary and confidential.
/// See the licenses directory for details.

#include <algorithm>
#include <labo/debug/Log.h>
#include <labo/server/Html.h>
#include <labo/server/http/Request.h>
#include <labo/server/http/Response.h>
#include <labo/util/fdstreambuf.h>
#include <netinet/in.h>
#include <sstream>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

namespace labo::server {

void
Html::start(const int port)
{
    // Try to open socket.
    sfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sfd < 0) {
        errs << "[Html] Failed to create socket." << endl;
        failure();
    }

    // Close your eyes, some nasty legacy C code
    // We want to pass this struct with address information.
    sockaddr_in server_address;
    ::bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    {
        // Override the socket for faster testing.
        int option{ 1 };
        setsockopt(
          sfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option));
        setsockopt(
          sfd, SOL_SOCKET, SO_REUSEPORT, (const char*)&option, sizeof(option));
    }

    // Bind our socket to an address
    if (::bind(sfd, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        errs << "[Html] Failed to bind to port: " << port << endl;
        failure();
    }

    // Start listening. Standby for new connections.
    ::listen(sfd, 8);

    logs << "[Html] Started at port: " << port << endl;

    sockaddr_in client_address;
    uint client_address_length;
    while (true) {
        auto fd{ ::accept(
          sfd, (sockaddr*)&client_address, &client_address_length) };
        if (fd < 0) {
            errs << "[Html] Failed to accept connection." << endl;
            continue;
        }

        logs << "[Html] Accepted connection from: "
             << client_address.sin_addr.s_addr << ":" << client_address.sin_port
             << endl;

        using namespace labo;
        using namespace http;
        socket::fdstreambuf stream{ fd };
        istream in{ &stream };
        ostream out{ &stream };

        Request req;
        in >> req;
        if (!req.valid) {
            logs << "[Html] Invalid Request." << endl;
            return;
        } else if (req.method() == Request::Method::GET && req.path() == "/") {
            /// reply with home page
            out << Response{
                Response::Status::OK,
                http::Html{ "res/home.html" },
                {
                  { "Content-Security-Policy",
                    "connect-src 'self' ws://funnypigrun.dns-cloud.net:12273" },
                }
            };
            logs << "[Html] Homepage access." << endl;
        } else {
            out << Response{ Response::Status::NOT_FOUND,
                             http::Html{ "res/not_found.html" } };
            errs << "[Html] Invalid URL: " << req.path() << endl;
        }

        logs << "[Html] Connection Finished." << endl;

        ::close(fd);
    }
}

Html::~Html()
{
    logs << "[Html] Server stopped." << endl;
    ::close(sfd);
}
};