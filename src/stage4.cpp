#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <iomanip>
#include "RandomStreamGen.h"
#include "HyperLogLog.h"
#include "CompactHyperLogLog.h"

int main() {
    const int B_BITS = 14; 
    const int STREAM_SIZE = 100000;
    const int STEP = 10000;

    CompactHyperLogLog compact_hll(B_BITS);
    HyperLogLog std_hll(B_BITS);

    size_t std_mem = std_hll.memory_usage_bytes();
    size_t compact_mem = compact_hll.memory_usage_bytes();
    
    std::cout << "\n[Memory Consumption]\n";
    std::cout << "Standard: " << std_mem << " bytes\n";
    std::cout << "Compact:  " << compact_mem << " bytes\n";
    std::cout << "Savings: " << (1.0 - (double)compact_mem / std_mem) * 100 << "%\n";

    std::cout << "\n[Accuracy Test]\n";
    std::cout << std::setw(10) << "Step" << std::setw(15) << "True Count" 
              << std::setw(15) << "Compact Est" << std::setw(10) << "Error %\n";
    
    RandomStreamGen gen(12345);
    std::set<std::string> unique_elements;

    for (int t = 0; t <= STREAM_SIZE; t++) {
        std::string s = gen.next();
        unique_elements.insert(s);
        compact_hll.add(s);

        if (t > 0 && t % STEP == 0) {
            double est = compact_hll.count();
            double real = unique_elements.size();
            double err = std::abs(est - real) / real * 100.0;
            
            std::cout << std::setw(10) << t << std::setw(15) << (int)real 
                      << std::setw(15) << (int)est << std::setw(9) << std::fixed << std::setprecision(2) << err << "%\n";
        }
    }

    return 0;
}