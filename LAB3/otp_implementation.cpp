#include <bits/stdc++.h>

using namespace std;

vector<unsigned char> readFile(const string &filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Cannot open file " << filename << endl;
        exit(1);
    }
    return vector<unsigned char>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
}


void writeFile(const string &filename, const vector<unsigned char> &data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error: Cannot create or open file " << filename << endl;
        exit(1);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}


vector<unsigned char> generateKey(size_t length) {
    vector<unsigned char> key(length);

    random_device rd;

    for (size_t i = 0; i < length; ++i) {
        key[i] = static_cast<unsigned char>(rd() % 256);
    }
    return key;
}


vector<unsigned char> xorData(const vector<unsigned char> &a, const vector<unsigned char> &b) {
    if (a.size() != b.size()) {
        throw runtime_error("Error: Data and key must be of the same length for XOR operation.");
    }

    vector<unsigned char> result(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result[i] = a[i] ^ b[i];
    }
    return result;
}

int main() {
    cout << "Starting One-Time Pad Encryption..." << endl;

    auto plaintext = readFile("plaintext.txt");
    if (plaintext.empty()) {
        cerr << "Plaintext is empty." << endl;
        return 1;
    }
    cout << "Successfully read " << plaintext.size() << " bytes from plaintext.txt." << endl;

    auto key = generateKey(plaintext.size());
    cout << "Generated a random key of " << key.size() << " bytes." << endl;

    writeFile("key.bin", key);
    cout << "Key saved to key.bin." << endl;

    auto ciphertext = xorData(plaintext, key);
    cout << "Plaintext encrypted." << endl;

    writeFile("ciphertext.bin", ciphertext);
    cout << "Ciphertext saved." << endl;

    cout << "\nDecrypting..." << endl;

    auto readKey = readFile("key.bin");
    cout << "Successfully read the key " << endl;

    auto decrypted = xorData(ciphertext, readKey);
    cout << "Ciphertext decrypted." << endl;

    writeFile("decrypted.txt", decrypted);
    cout << "Decrypted text saved." << endl;
    cout << "\nProcess complete!" << endl;

    return 0;
}