#ifndef ENTERPRISE_DATABASE_H
#define ENTERPRISE_DATABASE_H

#include <enterprise/dotenv.h>
#include <logging.h>
#include <util/system.h>

#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>

using namespace dotenv;

inline std::auto_ptr<odb::database>
create_enterprise_database()
{
    auto& dotenv = env;
    dotenv.config();
    std::auto_ptr<odb::database> enterprise_database(
        new odb::pgsql::database(
            dotenv["PGUSER"],
            dotenv["PGPASSWORD"],
            dotenv["PGDB"],
            dotenv["PGHOST"],
            dotenv["PGPORT"]));
    return enterprise_database;
}

#endif