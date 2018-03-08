#ifndef ENTERPRISE_SCRIPTS_H
#define ENTERPRISE_SCRIPTS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

/**
 *
 * Based on CScripts class
 *
 **/

class eScripts {
public:
    eScripts(
            std::string id,
            std::string script,
            std::string type
    ) : id(id),
        script(script),
        type(type) {};

#pragma db id
    std::string id;
    std::string script;
    std::string type;

private:
    friend class odb::access;

    eScripts() {}

};

#endif //ENTERPRISE_SCRIPTS_H
