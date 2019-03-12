#include "yangkvMain.h"
#pragma once

namespace leveldb {

class YangkvMain;

class Singleton {
public:
    static YangkvMain* get() {
        pthread_once(&once, _new);
        return _instance;
    }


private:

    Singleton();

    ~Singleton();
    /// @brief Construct the singleton instance
    static void _new() {
        _instance = new YangkvMain();
    }

    /// @brief  Destruct the singleton instance
    /// @note Only work with gcc
    static void _delete() {
    }
    static pthread_once_t once;    ///< Initialization once control
    static YangkvMain* _instance;    ///< The singleton instance

};

}