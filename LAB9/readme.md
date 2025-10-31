# Lab 9: Implementation of RSA Cryptosystem

**Course:** AI331 - Information Security and Cryptography  
**Institution:** SVNIT, Surat  
**Lab Title:** RSA Implementation (Toy Example)
**By:** U23AI065

---

## Submission Overview

This submission contains a complete implementation of the RSA cryptosystem in C++ using the Boost Multiprecision library. All five TODO tasks from the lab have been successfully completed.

---

## Completed Tasks

### Task 1: Extended Euclidean Algorithm
**Function:** `egcd(cpp_int a, cpp_int b)`

Implemented the recursive Extended Euclidean Algorithm that computes:
- `g` = gcd(a, b)
- `x, y` such that `ax + by = g`

**Implementation approach:**
- Base case: When b = 0, return {a, 1, 0}
- Recursive case: Call egcd(b, a % b) and update coefficients
- Returns a struct containing g, x, and y

This algorithm is essential for computing the modular inverse needed in RSA key generation.

### Task 2: Modular Inverse
**Function:** `modinv(cpp_int a, cpp_int m)`

Computes the modular inverse `a^(-1) mod m` using the Extended Euclidean Algorithm.

**Implementation approach:**
- Calls egcd(a, m) to get coefficients
- Checks if gcd = 1 (inverse exists only if a and m are coprime)
- Returns `(x % m + m) % m` to ensure positive result
- Throws error if inverse doesn't exist

This is used to compute the private exponent d from the public exponent e.

### Task 3: Modular Exponentiation
**Function:** `modexp(cpp_int base, cpp_int exp, cpp_int mod)`

Efficiently computes `(base^exp) mod mod` using the square-and-multiply algorithm.

**Implementation approach:**
- Initialize result = 1
- While exp > 0:
  - If exp is odd: multiply result by base (mod mod)
  - Square the base (mod mod)
  - Divide exp by 2
- Returns the final result

This prevents overflow and computes large exponentiations efficiently, crucial for both encryption and decryption.

### Task 4: RSA Key Generation
**Implementation in main():**

Generated RSA keys using the following steps:
1. Selected two prime numbers: `p = 61`, `q = 53`
2. Computed `n = p × q = 3233`
3. Computed Euler's totient: `phi = (p-1) × (q-1) = 3120`
4. Selected public exponent: `e = 17` (coprime with φ(n))
5. Computed private exponent: `d = modinv(e, phi) = 2753`

**Public Key:** (e, n) = (17, 3233)  
**Private Key:** (d, n) = (2753, 3233)

### Task 5: Encryption and Decryption
**Implementation in main():**

Performed encryption and decryption on message m = 65:

- **Encryption:** `c = modexp(m, e, n) = modexp(65, 17, 3233) = 2790`
- **Decryption:** `m_dec = modexp(c, d, n) = modexp(2790, 2753, 3233) = 65`
- **Verification:** m == m_dec ✓ (Decryption successful!)

---

## Program Output

```
=== RSA Implementation (Toy Example) ===
p = 61
q = 53
n = 3233
phi(n) = 3120
Public exponent e = 17
Private exponent d = 2753

Message m = 65
Ciphertext c = 2790
Decrypted message m_dec = 65

[OK] Decryption successful!
```

---

## Compilation and Execution

### Prerequisites
- C++ compiler (g++ or clang++)
- Boost Multiprecision library

### Compilation Command
```bash
g++ -std=c++11 lab9_rsa.cpp -o rsa
```

### Execution
```bash
./rsa
```

---

## Answers to Lab Discussion Questions

### 1. What is the purpose of choosing e coprime with φ(n)?

The public exponent e must be coprime with φ(n) (i.e., gcd(e, φ(n)) = 1) to ensure that the modular inverse exists. If gcd(e, φ(n)) ≠ 1, we cannot compute the private key d such that `e × d ≡ 1 (mod φ(n))`, making decryption impossible.

### 2. Why is the modular inverse necessary in key generation?

The modular inverse is necessary to compute the private exponent d from the public exponent e. We need d to satisfy:
```
d ≡ e^(-1) (mod φ(n))
```
This ensures that `(m^e)^d ≡ m^(ed) ≡ m (mod n)` due to Euler's theorem, allowing correct decryption. Without the modular inverse, we cannot recover the original message from the ciphertext.

### 3. What happens if m ≥ n?

If the message m is greater than or equal to n, the encryption operation `c = m^e mod n` will reduce m modulo n before encryption. This causes information loss because multiple different messages (m, m+n, m+2n, ...) would all encrypt to the same ciphertext. During decryption, we can only recover `m mod n`, not the original value of m. Therefore, messages must always satisfy `0 ≤ m < n`.

### 4. Explain why this "textbook RSA" is not secure in practice.

Textbook RSA is insecure for several reasons:

- **Deterministic Encryption:** The same plaintext always produces the same ciphertext, allowing attackers to build dictionaries of encrypted messages
- **No Padding:** Vulnerable to various mathematical attacks including:
  - Small message attacks (if m^e < n, c = m^e directly reveals m)
  - Chosen ciphertext attacks
  - Related message attacks
- **Malleability:** RSA is multiplicatively homomorphic: `E(m1) × E(m2) = E(m1 × m2)`, allowing attackers to manipulate ciphertexts
- **Small Exponent Attacks:** Using small values of e (like e=3) with unpadded messages can be exploited

**Solution:** Modern RSA implementations use padding schemes like OAEP (Optimal Asymmetric Encryption Padding) which adds randomness and structure to messages before encryption, preventing these attacks.

---

## Key Learning Outcomes

Through this lab, I have successfully:
- Implemented the Extended Euclidean Algorithm for computing GCD and coefficients
- Computed modular inverses using the Extended Euclidean Algorithm
- Implemented efficient modular exponentiation using square-and-multiply
- Generated RSA key pairs (public and private keys)
- Performed RSA encryption and decryption
- Verified the correctness of the implementation
- Understood the mathematical foundation of RSA
- Recognized the limitations of textbook RSA and the need for padding schemes

---

## Testing and Verification

The implementation has been tested with the given parameters:
- Primes: p = 61, q = 53
- Message: m = 65
- Successfully encrypted and decrypted with matching results

The output matches the expected example output from the lab manual, confirming correct implementation of all components.

---

**End of Submission**
