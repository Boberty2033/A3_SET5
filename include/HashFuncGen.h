#ifndef HASH_FUNC_GEN_H
#define HASH_FUNC_GEN_H

#include <string>
#include <cstdint>

class HashFuncGen {
public:
    static uint32_t hash(const std::string& key, uint32_t seed = 0) {
        const char* data = key.data();
        int len = key.length();
        const int nblocks = len / 4;

        uint32_t h1 = seed;
        uint32_t c1 = 0xcc9e2d51;
        uint32_t c2 = 0x1b873593;

        const uint32_t* blocks = (const uint32_t*)(data);

        for (int i = 0; i < nblocks; i++) {
            uint32_t k1 = blocks[i];
            k1 *= c1;
            k1 = (k1 << 15) | (k1 >> 17);
            k1 *= c2;

            h1 ^= k1;
            h1 = (h1 << 13) | (h1 >> 19);
            h1 = h1 * 5 + 0xe6546b64;
        }

        const uint8_t* tail = (const uint8_t*)(data + nblocks * 4);
        uint32_t k1 = 0;

        switch (len & 3) {
        case 3: k1 ^= tail[2] << 16;
        case 2: k1 ^= tail[1] << 8;
        case 1: k1 ^= tail[0];
                k1 *= c1; k1 = (k1 << 15) | (k1 >> 17); k1 *= c2; h1 ^= k1;
        };

        h1 ^= len;
        h1 ^= h1 >> 16;
        h1 *= 0x85ebca6b;
        h1 ^= h1 >> 13;
        h1 *= 0xc2b2ae35;
        h1 ^= h1 >> 16;

        return h1;
    }
};

#endif