#ifndef PACKED_REGISTER_ARRAY_H
#define PACKED_REGISTER_ARRAY_H

#include <vector>
#include <cstdint>
#include <cstddef>

class PackedRegisterArray {
private:
    std::vector<uint64_t> data;
    size_t size_;
    
    static const int REG_WIDTH = 6;
    static const int REGS_PER_WORD = 10;

public:
    PackedRegisterArray(size_t n_registers) : size_(n_registers) {
        size_t words = (n_registers + REGS_PER_WORD - 1) / REGS_PER_WORD;
        data.assign(words, 0);
    }

    uint8_t get(size_t index) const {
        size_t word_idx = index / REGS_PER_WORD;
        int bit_offset = (index % REGS_PER_WORD) * REG_WIDTH;
        return (data[word_idx] >> bit_offset) & 0x3F;
    }

    void set(size_t index, uint8_t value) {
        if (value > 63) value = 63;
        
        size_t word_idx = index / REGS_PER_WORD;
        int bit_offset = (index % REGS_PER_WORD) * REG_WIDTH;
        
        uint64_t mask = ~(uint64_t(0x3F) << bit_offset);
        data[word_idx] &= mask;
        data[word_idx] |= (uint64_t(value) << bit_offset);
    }

    size_t memory_usage() const {
        return data.size() * sizeof(uint64_t);
    }
};

#endif