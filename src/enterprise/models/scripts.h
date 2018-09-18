#ifndef ENTERPRISE_SCRIPTS_H
#define ENTERPRISE_SCRIPTS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

/**
 *
 * Based on CScripts class
 *
 **/

#pragma db object
class eScripts {
public:
    eScripts(
            std::string hash,
            std::string script,
            std::string type
    ) : hash(hash),
        script(script),
        type(type) {};

    eScripts(
            std::string hash,
            std::string script
    ) : hash(hash),
        script(script) {};

#pragma db id auto
    unsigned int id;

    std::string hash;
    std::string script;
    std::string type;

private:
    friend class odb::access;

    eScripts() {}

};

#endif //ENTERPRISE_SCRIPTS_H
