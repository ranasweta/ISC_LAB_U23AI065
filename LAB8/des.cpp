#include "des.hpp"
#include <cstdint>

namespace des {

uint32_t feistel(uint32_t R, uint64_t subkey) {
    uint64_t expanded = 0;
    for (int i = 0; i < 48; i++) {
        int bit_pos = Ebit[i] - 1; 
        uint32_t bit = (R >> (32 - 1 - bit_pos)) & 1;
        expanded = (expanded << 1) | bit;
    }
    
    uint64_t xored = expanded ^ subkey;
    
    uint32_t sbox_output = 0;
    for (int i = 0; i < 8; i++) {
        uint8_t six_bits = (xored >> (42 - i * 6)) & 0x3F;
        
        int row = ((six_bits >> 5) & 1) << 1 | (six_bits & 1);
        
        int col = (six_bits >> 1) & 0x0F;
        
        uint8_t sbox_val = SBOX[i][row * 16 + col];
        
        sbox_output = (sbox_output << 4) | sbox_val;
    }
    
    uint32_t output = 0;
    for (int i = 0; i < 32; i++) {
        int bit_pos = Pbox[i] - 1; 
        uint32_t bit = (sbox_output >> (32 - 1 - bit_pos)) & 1;
        output = (output << 1) | bit;
    }
    
    return output;
}

// Generic permutation for a 64-bit container (MSB-first bit numbering)
static inline uint64_t permute(uint64_t in, const int* table, int n) {
    uint64_t out = 0;
    for (int i = 0; i < n; ++i) {
        int from = table[i] - 1; // 1-based -> 0-based
        uint64_t bit = (in >> (64 - 1 - from)) & 1ULL; // take bit at position 'from'
        out = (out << 1) | bit;
    }
    return out;
}

static inline uint64_t left_rotate28(uint64_t v, int r) {
    v &= 0x0FFFFFFF; // 28 bits
    return ((v << r) | (v >> (28 - r))) & 0x0FFFFFFF;
}

// Key schedule: input 64-bit key (with parity bits); outputs 16 subkeys (low 48 bits used)
void key_schedule(uint64_t key64, uint64_t subkeys[16]) {
    // PC-1: 64 -> 56 (drop parity)
    uint64_t key56 = permute(key64, PC1, 56);
    uint64_t C = (key56 >> 28) & 0x0FFFFFFF;
    uint64_t D = key56 & 0x0FFFFFFF;

    for (int r = 0; r < 16; ++r) {
        C = left_rotate28(C, ROTATIONS[r]);
        D = left_rotate28(D, ROTATIONS[r]);
        uint64_t CD = (C << 28) | D; // 56 bits
        
        // PC-2: 56 -> 48
        uint64_t k48 = 0;
        // Note: The provided permute function assumes a 64-bit container, 
        // so we manually implement the permutation for 56->48 bit selection.
        for(int i=0; i<48; ++i) {
            int from = PC2[i]-1;
            uint64_t bit = (CD >> (56-1-from)) & 1ULL;
            k48 = (k48<<1) | bit;
        }
        subkeys[r] = k48; // stored in low 48 bits
    }
}

} 