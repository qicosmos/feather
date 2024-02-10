//
// Created by root on 4/24/18.
//

#ifndef FEATHER_FEATHER_H
#define FEATHER_FEATHER_H

#include "cinatra/include/cinatra.hpp"
#include "iguana/json_reader.hpp"
#include "iguana/json_writer.hpp"
#include "ormpp/connection_pool.hpp"
#include "ormpp/dbng.hpp"
#ifdef ORMPP_ENABLE_MYSQL
#include "ormpp/mysql.hpp"
#endif
#ifdef ORMPP_ENABLE_SQLITE3
#include "ormpp/sqlite.hpp"
#endif
#ifdef ORMPP_ENABLE_PG
#include "ormpp/postgresql.hpp"
#endif
#include "util.hpp"
#endif  // FEATHER_FEATHER_H
