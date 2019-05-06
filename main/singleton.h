#include "yangkvMain.h"
#pragma once

namespace yangkv {

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
    static void _new() {
        _instance = new YangkvMain();
    }

    static void _delete() {
    }
    static pthread_once_t once;    ///< Initialization once control
    static YangkvMain* _instance;    ///< The singleton instance

};

}