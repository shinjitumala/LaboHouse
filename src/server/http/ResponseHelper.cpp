/// @file ResponseHelper.cpp
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#include <labo/server/http/Response.h>
#include <labo/server/http/ResponseHelper.h>

namespace labo::http {
Response
not_found(string error)
{
    return { Response::Status::NOT_FOUND, { { "error", error } } };
};

Response
unauthorized(string error)
{
    return { Response::Status::UNAURHORIZED, { { "error", error } } };
};

Response
forbidden(string error)
{
    return { Response::Status::FORBIDDEN, { { "error", error } } };
}

Response
bad_request(string error)
{
    return { Response::Status::BAD_REQUEST, { { "error", error } } };
}
};