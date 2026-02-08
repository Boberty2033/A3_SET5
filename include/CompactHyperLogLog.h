#ifndef COMPACT_HYPER_LOG_LOG_H
#define COMPACT_HYPER_LOG_LOG_H

#include <cmath>
#include <string>
#include "HashFuncGen64.h"
#include "PackedRegisterArray.h"

class CompactHyperLogLog {
private:
    int b;
    int m;
    double alphaMM;
    PackedRegisterArray registers;

    uint8_t get_rank(uint64_t hash_val) {
        uint64_t w = hash_val << b; 
        
        if (w == 0) return 64 - b + 1;
        
        uint8_t rank = 1;
        while ((w & 0x8000000000000000ULL) == 0) {
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
    CompactHyperLogLog(int b_bits) : b(b_bits), m(1 << b_bits), registers(1 << b_bits) {
        alphaMM = get_alpha(m) * m * m;
    }

    void add(const std::string& s) {
        uint64_t x = HashFuncGen64::hash(s);
        uint32_t idx = x >> (64 - b);
        uint8_t rank = get_rank(x); 
        
        uint64_t w = x << b; 
        uint8_t current_rank = 1;
        if (w != 0) {
             while ((w & 0x8000000000000000ULL) == 0) {
                current_rank++;
                w <<= 1;
            }
        } else {
            current_rank = 64 - b + 1;
        }

        if (current_rank > registers.get(idx)) {
            registers.set(idx, current_rank);
        }
    }

    double count() const {
        double sum = 0.0;
        int zeros = 0;
        for (int i = 0; i < m; ++i) {
            uint8_t val = registers.get(i);
            sum += std::pow(2.0, -val);
            if (val == 0) zeros++;
        }

        double estimate = alphaMM / sum;

        if (estimate <= 2.5 * m) {
            if (zeros > 0) {
                estimate = m * std::log((double)m / zeros);
            }
        } else if (estimate > std::pow(2.0, 32)) {
             estimate = -std::pow(2.0, 32) * std::log(1.0 - estimate/std::pow(2.0, 32));
        }
        return estimate;
    }

    size_t memory_usage_bytes() const {
        return registers.memory_usage();
    }
};

#endif