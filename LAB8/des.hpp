#ifndef DES_HPP
#define DES_HPP

#include <cstdint>

namespace des {
    // S-boxes
    const uint8_t SBOX[8][64] = {
        // S1
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
         0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
         4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
         15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13},
        // S2-S8 boxes should be added here
        // ... Add the remaining S-boxes
    };

    // Expansion table
    const uint8_t E[] = {
        32,1,2,3,4,5,
        4,5,6,7,8,9,
        8,9,10,11,12,13,
        12,13,14,15,16,17,
        16,17,18,19,20,21,
        20,21,22,23,24,25,
        24,25,26,27,28,29,
        28,29,30,31,32,1
    };

    // P-box permutation
    const uint8_t P[] = {
        16,7,20,21,29,12,28,17,
        1,15,23,26,5,18,31,10,
        2,8,24,14,32,27,3,9,
        19,13,30,6,22,11,4,25
    };

    inline uint32_t feistel(uint32_t R, uint64_t K) {
        // 1. Expansion: 32-bit R to 48-bit
        uint64_t expanded = 0;
        for(int i = 0; i < 48; i++) {
            expanded |= ((R >> (32 - E[i])) & 0x1ULL) << (47 - i);
        }
        
        // 2. XOR with subkey
        expanded ^= K;
        
        // 3. S-box substitution
        uint32_t output = 0;
        for(int i = 0; i < 8; i++) {
            int row = ((expanded >> (42 - 6*i)) & 0x20) | ((expanded >> (43 - 6*i)) & 0x1);
            int col = (expanded >> (43 - 6*i)) & 0x1E;
            output |= SBOX[i][row*16 + (col>>1)] << (28 - 4*i);
        }
        
        // 4. P-box permutation
        uint32_t final = 0;
        for(int i = 0; i < 32; i++) {
            final |= ((output >> (32 - P[i])) & 0x1) << (31 - i);
        }
        
        return final;
    }
}

#endif