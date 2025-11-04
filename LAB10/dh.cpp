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

struct Person {
    uint64_t private_key;
    uint64_t public_value;
    uint64_t shared_key;

    void gen_private_key(uint64_t p, std::mt19937_64& gen) {
        std::uniform_int_distribution<uint64_t> dist(2, p - 2);
        private_key = dist(gen);
    }

    void compute_public_value(uint64_t g, uint64_t p) {
        public_value = modexp(g, private_key, p);
    }

    void compute_shared_key(uint64_t other_public_value, uint64_t p) {
        shared_key = modexp(other_public_value, private_key, p);
    }
};

int main() {
    uint64_t p = 4294967311;
    uint64_t g = 5;

    std::random_device rd;
    std::mt19937_64 gen(rd());

    Person alice;
    Person bob;

    alice.gen_private_key(p, gen);
    bob.gen_private_key(p, gen);

    alice.compute_public_value(g, p);
    bob.compute_public_value(g, p);

    alice.compute_shared_key(bob.public_value, p);
    bob.compute_shared_key(alice.public_value, p);

    std::cout << std::left << std::setw(28) << "Public prime p:" << p << std::endl;
    std::cout << std::left << std::setw(28) << "Public generator g:" << g << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::cout << std::left << std::setw(28) << "Alice's private key a:" << alice.private_key << std::endl;
    std::cout << std::left << std::setw(28) << "Alice's public value A:" << alice.public_value << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::cout << std::left << std::setw(28) << "Bob's private key b:" << bob.private_key << std::endl;
    std::cout << std::left << std::setw(28) << "Bob's public value B:" << bob.public_value << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::cout << std::left << std::setw(28) << "Shared key (Alice) K_A:" << alice.shared_key << std::endl;
    std::cout << std::left << std::setw(28) << "Shared key (Bob) K_B:" << bob.shared_key << std::endl;
    std::cout << "---------------------------------" << std::endl;

    std::cout << std::left << std::setw(28) << "Keys match:" 
              << (alice.shared_key == bob.shared_key ? "YES" : "NO") << std::endl;

    return 0;
}