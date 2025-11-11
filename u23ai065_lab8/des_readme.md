# Lab 8: DES Round Function Implementation

## Implementation Overview

This implementation completes the DES round function `f` (feistel) which performs the following operations:

1. **E-expansion**: Expands 32-bit input R to 48 bits using the E-table
2. **XOR**: XORs the expanded result with the 48-bit subkey
3. **S-box substitution**: Applies 8 S-boxes, each converting 6 bits to 4 bits
4. **P-permutation**: Permutes the resulting 32 bits to produce final output

## Algorithm Details

### E-Expansion
- Uses bit-by-bit extraction from the 32-bit R
- Each bit position is mapped according to the Ebit table
- Produces 48-bit output

### S-Box Application
- Divides 48-bit XOR result into 8 groups of 6 bits
- For each 6-bit group:
  - Outer bits (bits 5 and 0) form the row index (0-3)
  - Middle bits (bits 4-1) form the column index (0-15)
  - Looks up value in corresponding S-box
  - Each S-box outputs 4 bits

### P-Permutation
- Rearranges the 32-bit S-box output according to Pbox table
- Uses bit-by-bit extraction and reconstruction

## Test Results

### Test Case
```
Input R:       0xF0AAF0AA
Subkey K:      0x1B02EFFC7072
Expected:      0x234AA9BB
Your output:   0x234AA9BB

✓ TEST PASSED!
```

## Compilation Instructions

```bash
g++ -std=c++11 -o test_feistel test_feistel.cpp des.cpp des_tables.cpp
./test_feistel
```

## Files Submitted
- `des.hpp` - Header file with function declarations
- `des.cpp` - Implementation of the feistel function
- `README.md` - This file with test results

## Notes
- Bit numbering follows DES convention (MSB = bit 1, LSB = bit 32 for 32-bit values)
- All tables (E, S-boxes, P) use 1-based indexing and are converted to 0-based in code
- Implementation is straightforward and readable for educational purposes
