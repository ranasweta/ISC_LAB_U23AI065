#include "des.hpp"
#include <iostream>
#include <iomanip>
#include <cstdint>

int main() {
    uint32_t R = 0xF0AAF0AA;
    uint64_t subkey = 0x1B02EFFC7072ULL;
    uint32_t expected = 0x234AA9BB;
    
    std::cout << "Testing DES Feistel Function\n";
    std::cout << "==============================\n\n";
    
    std::cout << "Input R:       0x" << std::hex << std::uppercase 
              << std::setw(8) << std::setfill('0') << R << "\n";
    std::cout << "Subkey K:      0x" << std::setw(12) << subkey << "\n";
    std::cout << "Expected:      0x" << std::setw(8) << expected << "\n";
    
    uint32_t result = des::feistel(R, subkey);
    
    std::cout << "Your output:   0x" << std::setw(8) << result << "\n\n";
    
    if (result == expected) {
        std::cout << "TEST PASSED!\n";
        return 0;
    } else {
        std::cout << "TEST FAILED!\n";
        return 1;
    }
}