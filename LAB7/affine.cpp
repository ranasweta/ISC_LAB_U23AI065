/*
 * AI331: Information Security & Cryptography
 * Lab 7: Affine Cipher
 *
 * Build:
 * g++ -std=c++17 affine.cpp -o affine
 *
 * Usage:
 * ./affine <mode> a b
 * <mode> can be 'enc', 'dec', or 'cryptanalyze'
 *
 * Input:
 * Reads a single line of text from STDIN.
 *
 * Policy:
 * Keeps only letters A..Z (uppercased). Non-letters are dropped.
 */
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <numeric>
#include <map>
#include <algorithm>

using namespace std;

// ===== Small helpers =====
// Custom modulo operator to handle negative results correctly
static inline int mod(int x, int m) {
    int r = x % m;
    return (r < 0) ? r + m : r;
}

// Valid 'a' iff gcd(a, 26) == 1
static inline bool is_valid_a(int a) {
    int x = abs(a), y = 26;
    while (y) {
        int t = x % y;
        x = y;
        y = t;
    }
    return x == 1;
}

// Convert to uppercase A..Z only, drop everything else
static string sanitize_letters_only_upper(const string& s) {
    string t;
    t.reserve(s.size());
    for (char c : s) {
        if (isalpha((unsigned char)c)) {
            t.push_back((char)toupper(c));
        }
    }
    return t;
}

// ===== TODO: modular inverse a^{-1} (mod 26) via extended Euclid =====
// Implements the Extended Euclidean Algorithm to find the modular multiplicative inverse.
// Throws a runtime_error if the inverse does not exist.
static int modinv(int a, int m) {
    a = mod(a, m);
    if (m < 2) {
        throw runtime_error("modinv: modulus must be >= 2");
    }

    int old_r = a, r = m;
    int old_s = 1, s = 0;

    while (r != 0) {
        int quotient = old_r / r;
        
        int temp_r = r;
        r = old_r - quotient * r;
        old_r = temp_r;

        int temp_s = s;
        s = old_s - quotient * s;
        old_s = temp_s;
    }

    if (old_r != 1) {
        throw runtime_error("modinv: inverse does not exist (a and m are not coprime).");
    }

    return mod(old_s, m);
}

// ===== TODO: encrypt =====
// For each letter x in 0..25, compute y = (a*x + b) mod 26 and map back.
static string encrypt_text(const string& plain, int a, int b) {
    string cipher_text;
    cipher_text.reserve(plain.length());

    for (char p_char : plain) {
        int x = p_char - 'A';
        int y = mod((a * x + b), 26);
        cipher_text.push_back((char)('A' + y));
    }
    return cipher_text;
}

// ===== TODO: decrypt =====
// Use a_inv = a^{-1} (mod 26): x = a_inv * (y - b) mod 26.
static string decrypt_text(const string& cipher, int a, int b) {
    string plain_text;
    plain_text.reserve(cipher.length());

    int a_inv = modinv(a, 26);

    for (char c_char : cipher) {
        int y = c_char - 'A';
        int x = mod(a_inv * (y - b), 26);
        plain_text.push_back((char)('A' + x));
    }
    return plain_text;
}

// ===== Task B: Cryptanalysis =====
// Attempts to recover the key (a,b) using frequency analysis.
static void cryptanalyze(const string& cipher) {
    if (cipher.length() < 2) {
        cerr << "Error: Ciphertext is too short for frequency analysis." << endl;
        return;
    }

    // 1. Calculate frequency of each character
    map<char, int> freq;
    for (char c : cipher) {
        freq[c]++;
    }

    // 2. Find the two most frequent characters
    vector<pair<int, char>> sorted_freq;
    for (auto const& [key, val] : freq) {
        sorted_freq.push_back({val, key});
    }
    sort(sorted_freq.rbegin(), sorted_freq.rend());

    char y_max = sorted_freq[0].second;
    char y_2nd = (sorted_freq.size() > 1) ? sorted_freq[1].second : y_max;

    cout << "Most frequent letters: '" << y_max << "', '" << y_2nd << "'" << endl;
    cout << "Attempting key recovery based on E, T mapping..." << endl;
    cout << "------------------------------------------------" << endl;


    // 3. Test two hypotheses based on English letter frequencies (E and T)
    // English plaintext: E -> 4, T -> 19
    const int x_E = 4;
    const int x_T = 19;
    
    // Calculate (x1 - x2)^-1 mod 26
    int delta_x_inv = modinv(x_E - x_T, 26);

    // Hypothesis 1: y_max = E, y_2nd = T
    cout << "Hypothesis 1: '" << y_max << "' -> 'E', '" << y_2nd << "' -> 'T'" << endl;
    try {
        int y1 = y_max - 'A';
        int y2 = y_2nd - 'A';
        int a = mod((y1 - y2) * delta_x_inv, 26);
        if (is_valid_a(a)) {
            int b = mod(y1 - a * x_E, 26);
            cout << "  Candidate Key: (a=" << a << ", b=" << b << ")" << endl;
            cout << "  Decrypted Text: " << decrypt_text(cipher, a, b) << endl;
        } else {
            cout << "  Invalid key 'a' computed: " << a << endl;
        }
    } catch (const exception& e) {
        cerr << "  Error during Hypothesis 1: " << e.what() << endl;
    }
    cout << "------------------------------------------------" << endl;


    // Hypothesis 2: y_max = T, y_2nd = E
    cout << "Hypothesis 2: '" << y_max << "' -> 'T', '" << y_2nd << "' -> 'E'" << endl;
    try {
        // Here, y1 corresponds to T and y2 corresponds to E
        int y1 = y_max - 'A';
        int y2 = y_2nd - 'A';
        // a = (y_T - y_E) * (x_T - x_E)^-1
        // (x_T - x_E)^-1 is the same as (-(x_E - x_T))^-1 = -delta_x_inv
        int a = mod((y1 - y2) * modinv(x_T - x_E, 26), 26);
        if (is_valid_a(a)) {
            int b = mod(y1 - a * x_T, 26);
            cout << "  Candidate Key: (a=" << a << ", b=" << b << ")" << endl;
            cout << "  Decrypted Text: " << decrypt_text(cipher, a, b) << endl;
        } else {
            cout << "  Invalid key 'a' computed: " << a << endl;
        }
    } catch (const exception& e) {
        cerr << "  Error during Hypothesis 2: " << e.what() << endl;
    }
}


int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <enc|dec|cryptanalyze> [a] [b]\n";
        return 1;
    }

    string mode = argv[1];

    string line;
    if (!getline(cin, line)) line = "";
    string in = sanitize_letters_only_upper(line);

    if (mode == "cryptanalyze") {
        cryptanalyze(in);
        return 0;
    }

    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <enc|dec> a b\n";
        return 1;
    }

    int a = stoi(argv[2]);
    int b = stoi(argv[3]);

    if (!is_valid_a(a)) {
        cerr << "Error: a must be coprime with 26. Valid choices: {1,3,5,7,9,11,15,17,19,21,23,25}.\n";
        return 1;
    }

    b = mod(b, 26);

    try {
        if (mode == "enc") {
            cout << encrypt_text(in, a, b) << "\n";
        } else if (mode == "dec") {
            cout << decrypt_text(in, a, b) << "\n";
        } else {
            cerr << "Mode must be 'enc', 'dec', or 'cryptanalyze'.\n";
            return 1;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}