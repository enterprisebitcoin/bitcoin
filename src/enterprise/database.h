#ifndef ENTERPRISE_DATABASE_H
#define ENTERPRISE_DATABASE_H

#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>

#include "util.h"

inline std::auto_ptr <odb::database>
create_enterprise_database() {

    std::auto_ptr <odb::database> enterprise_database(
            new odb::pgsql::database(
                    gArgs.GetArg("-enterprise_user", std::getenv("ENTERPRISE_PGUSER")),
                    gArgs.GetArg("-enterprise_password", std::getenv("ENTERPRISE_PGPASSWORD")),
                    gArgs.GetArg("-enterprise_database", std::getenv("ENTERPRISE_PGDATABASE")),
                    gArgs.GetArg("-enterprise_host", std::getenv("ENTERPRISE_PGHOST")),
                    gArgs.GetArg("-enterprise_port", std::getenv("ENTERPRISE_PGPORT"))
            ));

    return enterprise_database;
}

#endif