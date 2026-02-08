#include <iostream>
#include <set>
#include <fstream>
#include "RandomStreamGen.h"
#include "HyperLogLog.h"

struct ExperimentResult {
    int time_step;
    long long true_cardinality;
    double estimated_cardinality;
};

int main() {
    const int B_BITS = 14;
    const int NUM_RUNS = 20;
    const int STREAM_SIZE = 100000;
    const int STEP = 5000;

    std::ofstream csv_file("hll_results.csv");
    csv_file << "step,run_id,true_count,est_count\n";

    for (int run = 0; run < NUM_RUNS; ++run) {
        RandomStreamGen gen(42 + run);
        HyperLogLog hll(B_BITS);
        std::set<std::string> exact_set;

        for (int t = 0; t <= STREAM_SIZE; t += STEP) {
            if (t > 0) {
                auto batch = gen.generate_batch(STEP);
                for (const auto& s : batch) {
                    exact_set.insert(s);
                    hll.add(s);
                }
            }
            
            if (t > 0) {
                csv_file << t << "," << run << "," << exact_set.size() << "," << hll.count() << "\n";
            }
        }
    }

    csv_file.close();
    return 0;
}