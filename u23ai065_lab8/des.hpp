#pragma once
#include <cstdint>

namespace des {

extern const int Ebit[48];
extern const int Pbox[32];
extern const uint8_t SBOX[8][64];
extern const int PC1[56];
extern const int PC2[48];
extern const int ROTATIONS[16];

// Round function f: takes 32-bit R and 48-bit subkey K (low 48 bits used)
uint32_t feistel(uint32_t R, uint64_t subkey);

// Optional: expose key schedule if you want to call it from tests
void key_schedule(uint64_t key64, uint64_t subkeys[16]);

} // namespace des