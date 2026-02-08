#ifndef HASH_FUNC_GEN_64_H
#define HASH_FUNC_GEN_64_H

#include <string>
#include <cstdint>

class HashFuncGen64 {
public:
    static uint64_t hash(const std::string& key, uint64_t seed = 0) {
        const uint64_t m = 0xc6a4a7935bd1e995;
        const int r = 47;
        uint64_t h = seed ^ (key.length() * m);

        const uint64_t* data = (const uint64_t*)key.data();
        const uint64_t* end = data + (key.length() / 8);

        while (data != end) {
            uint64_t k = *data++;
            k *= m;
            k ^= k >> r;
            k *= m;
            h ^= k;
            h *= m;
        }

        const unsigned char* data2 = (const unsigned char*)data;
        switch (key.length() & 7) {
        case 7: h ^= uint64_t(data2[6]) << 48;
        case 6: h ^= uint64_t(data2[5]) << 40;
        case 5: h ^= uint64_t(data2[4]) << 32;
        case 4: h ^= uint64_t(data2[3]) << 24;
        case 3: h ^= uint64_t(data2[2]) << 16;
        case 2: h ^= uint64_t(data2[1]) << 8;
        case 1: h ^= uint64_t(data2[0]);
                h *= m;
        };

        h ^= h >> r;
        h *= m;
        h ^= h >> r;

        return h;
    }
};

#endif