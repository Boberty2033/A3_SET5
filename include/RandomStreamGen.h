#ifndef RANDOM_STREAM_GEN_H
#define RANDOM_STREAM_GEN_H

#include <string>
#include <vector>
#include <random>

class RandomStreamGen {
private:

    const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-";
    
    std::mt19937 rng;
    std::uniform_int_distribution<int> length_dist;
    std::uniform_int_distribution<int> char_dist;

public:
    RandomStreamGen(unsigned int seed) 
        : rng(seed), 
          length_dist(1, 30), 
          char_dist(0, chars.size() - 1) {}

    std::string next() {
        int len = length_dist(rng);
        std::string res;
        res.reserve(len);
        for (int i = 0; i < len; ++i) {
            res += chars[char_dist(rng)];
        }
        return res;
    }

    std::vector<std::string> generate_batch(size_t count) {
        std::vector<std::string> batch;
        batch.reserve(count);
        for (size_t i = 0; i < count; ++i) {
            batch.push_back(next());
        }
        return batch;
    }
};

#endif