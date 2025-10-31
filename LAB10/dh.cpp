#include <iostream>
#include <cstdint>
#include <random>
#include <iomanip>

uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod) {
    __uint128_t res = 1;
    __uint128_t b = base % mod;
    
    while (exp > 0) {
        if (exp % 2 == 1) {
            res = (res * b) % mod;
        }
        b = (b * b) % mod;
        exp /= 2;
    }
    
    return (uint64_t)res;
}

int main() {
    uint64_t p = 4294967311;
    uint64_t g = 5;

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(2, p - 2);

    uint64_t a = dist(gen);
    uint64_t b = dist(gen);

    uint64_t A = modexp(g, a, p);
    uint64_t B = modexp(g, b, p);

    uint64_t K_A = modexp(B, a, p);
    uint64_t K_B = modexp(A, b, p);

    std::cout << std::left << std::setw(28) << "Public prime p:" << p << std::endl;
    std::cout << std::left << std::setw(28) << "Public generator g:" << g << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::cout << std::left << std::setw(28) << "Alice's private key a:" << a << std::endl;
    std::cout << std::left << std::setw(28) << "Alice's public value A:" << A << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::cout << std::left << std::setw(28) << "Bob's private key b:" << b << std::endl;
    std::cout << std::left << std::setw(28) << "Bob's public value B:" << B << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::cout << std::left << std::setw(28) << "Shared key (Alice) K_A:" << K_A << std::endl;
    std::cout << std::left << std::setw(28) << "Shared key (Bob) K_B:" << K_B << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::cout << std::left << std::setw(28) << "Keys match:" 
              << (K_A == K_B ? "YES" : "NO") << std::endl;

    return 0;
}