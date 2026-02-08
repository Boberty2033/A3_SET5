#ifndef HYPER_LOG_LOG_H
#define HYPER_LOG_LOG_H

#include <vector>
#include <cmath>
#include <cstdint>
#include "HashFuncGen.h"

class HyperLogLog {
private:
    int b;
    int m;
    double alphaMM;
    std::vector<uint8_t> registers;

    uint8_t get_rank(uint32_t hash_val) {
        uint32_t w = hash_val << b;
        if (w == 0) return 32 - b + 1;
        
        uint8_t rank = 1;
        while ((w & 0x80000000) == 0) {
            rank++;
            w <<= 1;
        }
        return rank;
    }

    double get_alpha(int m) {
        if (m == 16) return 0.673;
        if (m == 32) return 0.697;
        if (m == 64) return 0.709;
        return 0.7213 / (1.0 + 1.079 / m);
    }

public:
    HyperLogLog(int b_bits) : b(b_bits) {
        m = 1 << b;
        registers.assign(m, 0);
        alphaMM = get_alpha(m) * m * m;
    }

    void add(const std::string& s) {
        uint32_t x = HashFuncGen::hash(s);
        uint32_t idx = x >> (32 - b);
        uint8_t rank = get_rank(x << b | (1 << (b-1)));

        if (rank > registers[idx]) {
            registers[idx] = rank;
        }
    }

    double count() const {
        double sum = 0.0;
        int zeros = 0;
        for (int val : registers) {
            sum += std::pow(2.0, -val);
            if (val == 0) zeros++;
        }

        double estimate = alphaMM / sum;

        if (estimate <= 2.5 * m) {
            if (zeros > 0) {
                estimate = m * std::log((double)m / zeros);
            }
        }
        else if (estimate > (1.0 / 30.0) * 4294967296.0) {
            estimate = -4294967296.0 * std::log(1.0 - (estimate / 4294967296.0));
        }

        return estimate;
    }
    
    size_t memory_usage_bytes() const {
        return registers.size() * sizeof(uint8_t);
    }
};

#endif