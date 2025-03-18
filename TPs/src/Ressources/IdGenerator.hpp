
#ifndef ID_GENERATOR_HPP
#define ID_GENERATOR_HPP

#include <string>

#define ID_LENGTH 11

static std::string generateId() {
    static const char alphanum[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                "abcdefghijklmnopqrstuvwxyz";
    std::string id = "id_";
    for (int i = 0; i < ID_LENGTH; ++i) {
        id += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return id;
}


#endif