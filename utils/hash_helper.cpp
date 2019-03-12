#include "hash_helper.h"

namespace leveldb {

uint64_t strHash(const std::string& str, const int seed) {
    uint64_t val = 1;
    for (auto ch: str) {
        val = (val + ch) * seed;
    }
    return val;
}

}
