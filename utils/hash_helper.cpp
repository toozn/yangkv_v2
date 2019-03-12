#include "hash_helper.h"

unsigned long long strHash(const string& str, const int seed) {
    unsigned long long val = 1;
    for (auto ch: str) {
        val = (val + ch) * seed;
    }
    return val;
}
