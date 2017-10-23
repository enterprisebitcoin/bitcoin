#ifndef ENTERPRISE_DATABASE_H
#define ENTERPRISE_DATABASE_H

#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>

#include "util.h"

inline std::auto_ptr <odb::database>
create_enterprise_database() {
    using namespace std;
    using namespace odb::core;

    auto_ptr <odb::database> enterprise_database(
            new odb::pgsql::database(
                    gArgs.GetArg("-enterprise_user", "test_user"),
                    gArgs.GetArg("-enterprise_password", "test_password"),
                    gArgs.GetArg("-enterprise_database", "test_database"),
                    gArgs.GetArg("-enterprise_host", "localhost"),
                    gArgs.GetArg("-enterprise_port", "5432")
            ));

    return enterprise_database;
}

#endif