#include "des.hpp"
#include <iostream>
#include <iomanip>
#include <cstdint>
using namespace std;

int main() {
    uint32_t R = 0xF0AAF0AA;
    uint64_t K = 0x1B02EFFC7072;
    uint32_t expected_output = 0x234AA9BB;

    cout << "Testing DES f function..." << endl;
    cout << "Input R:         0x" << hex << uppercase << R << endl;
    cout << "Input Subkey:    0x" << K << endl;

    uint32_t actual_output = des::feistel(R, K);

    cout << "Actual output:   0x" << actual_output << endl;
    cout << "Expected output: 0x" << expected_output << endl;

    if (actual_output == expected_output) {
        cout << "\nTest PASSED!" << endl;
    } else {
        cout << "\nTest FAILED!" << endl;
    }

    return 0;
}
