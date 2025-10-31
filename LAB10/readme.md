# Lab 10: Diffie-Hellman Key Exchange

This project is a C++ implementation of the Diffie-Hellman key exchange protocol, completed for the Information Security & Cryptography lab.

## Objective

The goal is to simulate how two parties, Alice and Bob, can securely establish a shared secret key over an insecure channel. The program demonstrates this by:
1.  Implementing modular exponentiation (repeated squaring).
2.  Simulating Alice and Bob generating private keys and exchanging public values.
3.  Calculating the shared secret key independently on both sides ($K_A$ and $K_B$).
4.  Verifying that the computed keys match.

## Files

* `dh.cpp`: The C++ source code containing the full implementation.
* `output.txt`: A sample run of the program's output.


## How to Compile and Run

The program is written in C++17 and requires no external libraries.

### Compilation

Use `g++` to compile the source code:

```bash
g++ -std=c++17 dh.cpp -o dh