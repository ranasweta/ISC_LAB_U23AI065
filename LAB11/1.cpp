#include <bits/stdc++.h>
using namespace std;

uint32_t leftRotate(uint32_t value, unsigned int bits) {
    return (value << bits) | (value >> (32 - bits));
}

vector<uint8_t> toBytes(const string &input) {
    return vector<uint8_t>(input.begin(), input.end());
}

string bytesToBinary(const vector<uint8_t> &bytes) {
    string binary;
    for (uint8_t b : bytes) {
        for (int i = 7; i >= 0; --i)
            binary += ((b >> i) & 1) ? '1' : '0';
    }
    return binary;
}

string sha1(const string &input, bool showPadded = true) {
    vector<uint8_t> message = toBytes(input);

    uint64_t originalBitLen = (uint64_t)message.size() * 8;

    message.push_back(0x80);
    while ((message.size() * 8) % 512 != 448)
        message.push_back(0x00);

    for (int i = 7; i >= 0; --i)
        message.push_back((originalBitLen >> (i * 8)) & 0xFF);

    if (showPadded) {
        cout << "\n--- Padded Binary Message ---\n";
        string binary = bytesToBinary(message);
        for (size_t i = 0; i < binary.size(); i++) {
            cout << binary[i];
            if ((i + 1) % 64 == 0) cout << endl;
        }
        cout << "\nMessage Length (bits): " << binary.size() << endl;
    }

    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    for (size_t chunk = 0; chunk < message.size(); chunk += 64) {
        uint32_t w[80];

        for (int i = 0; i < 16; ++i) {
            w[i] = (message[chunk + i * 4] << 24) |
                   (message[chunk + i * 4 + 1] << 16) |
                   (message[chunk + i * 4 + 2] << 8) |
                   (message[chunk + i * 4 + 3]);
        }

        for (int t = 16; t < 80; ++t)
            w[t] = leftRotate(w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16], 1);

        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;

        for (int t = 0; t < 80; ++t) {
            uint32_t f, k;
            if (t <= 19) {
                f = (b & c) | ((~b) & d);
                k = 0x5A827999;
            } else if (t <= 39) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            } else if (t <= 59) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            } else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = leftRotate(a, 5) + f + e + k + w[t];
            e = d;
            d = c;
            c = leftRotate(b, 30);
            b = a;
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    stringstream ss;
    ss << hex << setfill('0')
       << setw(8) << h0
       << setw(8) << h1
       << setw(8) << h2
       << setw(8) << h3
       << setw(8) << h4;

    return ss.str();
}

int main() {
    string input;
    cout << "Enter message: ";
    getline(cin, input);

    string digest = sha1(input, true);
    cout << "\n--- SHA-1 Digest ---\n" << digest << endl;

    return 0;
}