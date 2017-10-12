#ifndef ENTERPRISE_DATABASE_H
#define ENTERPRISE_DATABASE_H

#include <odb/database.hxx>
#include <odb/pgsql/database.hxx>

inline std::auto_ptr <odb::database>
create_enterprise_database() {
    using namespace std;
    using namespace odb::core;

    auto_ptr <database> enterprise_database(new odb::pgsql::database(
             "test_user"     // database login name
             "test_password" // database password
             "test_database" // database name
    ));

    return enterprise_database;
}

#endif