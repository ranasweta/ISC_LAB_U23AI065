#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <iomanip>
#include <sstream>
using namespace std;

class SHA1 {
public:
   
    SHA1() {
        reset();
    }

    
    void reset() {
        H[0] = 0x67452301; 
        H[1] = 0xEFCDAB89; 
        H[2] = 0x98BADCFE;
        H[3] = 0x10325476; 
        H[4] = 0xC3D2E1F0; 
        buffer.clear();
        total_bit_count = 0;
    }

    
    void update(const string& data) {
        update(vector<uint8_t>(data.begin(), data.end()));
    }

    void update(const vector<uint8_t>& data) {
       
        buffer.insert(buffer.end(), data.begin(), data.end());
        total_bit_count += data.size() * 8;

        
        size_t offset = 0;
        while (buffer.size() - offset >= 64) {
            process_block(&buffer[offset]);
            offset += 64;
        }
        
       
        buffer.erase(buffer.begin(), buffer.begin() + offset);
    }

   
    string digest() {
        
        vector<uint8_t> final_buffer = buffer;
        uint64_t final_bit_count = total_bit_count;

        
        final_buffer.push_back(0x80);

        
        while (final_buffer.size() % 64 != 56) {
            final_buffer.push_back(0x00);
        }

        
        for (int i = 7; i >= 0; --i) {
            final_buffer.push_back((final_bit_count >> (i * 8)) & 0xFF);
        }

        
        uint32_t final_H[5];
        copy(H, H + 5, final_H);

        for (size_t offset = 0; offset < final_buffer.size(); offset += 64) {
            process_block(&final_buffer[offset], final_H);
        }

        
        stringstream ss;
        ss << hex << setfill('0');
        for (int i = 0; i < 5; ++i) {
            ss << setw(8) << final_H[i];
        }
        
        reset();
        
        return ss.str();
    }


private:
    uint32_t H[5];
    vector<uint8_t> buffer;
    uint64_t total_bit_count;

    
    inline uint32_t ROTL(uint32_t value, int shift) {
        return (value << shift) | (value >> (32 - shift));
    }

   
    void process_block(const uint8_t* block) {
        process_block(block, H); // Call the main version
    }


    void process_block(const uint8_t* block, uint32_t* current_H) {
        
      
        uint32_t W[80];
        for (int t = 0; t < 16; ++t) {
            W[t] = (block[t * 4]     << 24) |
                   (block[t * 4 + 1] << 16) |
                   (block[t * 4 + 2] << 8)  |
                   (block[t * 4 + 3]);
        }

        
        for (int t = 16; t < 80; ++t) {
            W[t] = ROTL(1, W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16]); // 
        }

        // 4. Main loop 
        uint32_t A = current_H[0];
        uint32_t B = current_H[1];
        uint32_t C = current_H[2];
        uint32_t D = current_H[3];
        uint32_t E = current_H[4];

        for (int t = 0; t < 80; ++t) {
            uint32_t f = 0;
            uint32_t K = 0;

            // 
            if (t <= 19) {
                f = (B & C) | (~B & D);
                K = 0x5A827999;
            } else if (t <= 39) {
                f = B ^ C ^ D;
                K = 0x6ED9EBA1;
            } else if (t <= 59) {
                f = (B & C) | (B & D) | (C & D);
                K = 0x8F1BBCDC;
            } else {
                f = B ^ C ^ D;
                K = 0xCA62C1D6;
            }

            uint32_t temp = ROTL(5, A) + f + E + K + W[t];
            E = D;
            D = C;
            C = ROTL(30, B);
            B = A;
            A = temp;
        }

        // 5. Update [cite: 38]
        current_H[0] += A;
        current_H[1] += B;
        current_H[2] += C;
        current_H[3] += D;
        current_H[4] += E;
    }
};

// Main function to drive the class
int main() {
    SHA1 hasher;
    
    string input_message;
    cout << "Enter a message to hash with SHA-1: ";
    getline(cin, input_message);

    
    hasher.update(input_message);
    string digest = hasher.digest(); 

    cout << "\nInput: \"" << input_message << "\"" << endl;
    cout << "SHA-1 Digest: " << digest << endl;

    
    cout << "\n--- Verification Test Cases ---" << endl;

    // Example 1 
    hasher.update("The quick brown fox jumps over the lazy dog");
    cout << "Test: \"The quick brown fox jumps over the lazy dog\"" << endl;
    cout << "Expected: 2fd4e1c67a2d28fced849ee1bb76e7391b93eb12" << endl;
    cout << "Actual:   " << hasher.digest() << endl;
    
    // Example 2 
    hasher.update("Information Security and Cryptography Lab");
    cout << "\nTest: \"Information Security and Cryptography Lab\"" << endl;
    cout << "Expected: 2fd42e97d9111551984ac20b7e5dfa4432666165" << endl;
    cout << "Actual:   " << hasher.digest() << endl;
    
    // Example 3 
    hasher.update("");
    cout << "\nTest: \"\" (empty string)" << endl;
    cout << "Expected: da39a3ee5e6b4b0d3255bfef95601890afd80709" << endl;
    cout << "Actual:   " << hasher.digest() << endl;

    return 0;
}