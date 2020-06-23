/// @file ResponseHelper.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
/// 
/// @copyright Copyright (c) 2020
/// 
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/server/http/Response.h>

namespace labo::http{
    Response not_found(string error = "");
    Response unauthorized(string error = "");
    Response forbidden(string error = "");
    Response bad_request(string error = "");
};