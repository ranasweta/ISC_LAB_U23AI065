#include <bits/stdc++.h>
using namespace std;

vector<unsigned char> performXOR(const vector<unsigned char>& data1, const vector<unsigned char>& data2) {
    if (data1.size() != data2.size()) {
        throw runtime_error("Error: The two data blocks must be the same size to XOR them.");
    }

    vector<unsigned char> result(data1.size());

    for (size_t i = 0; i < data1.size(); ++i) {
        result[i] = data1[i] ^ data2[i];
    }

    return result;
}

void printBytesAsNumbers(const vector<unsigned char>& data) {
    for (unsigned char byte : data) {
        cout << static_cast<int>(byte) << " ";  
    }
    cout << endl;
}

int main() {
    string message1 = "ATTACK AT DAWN";
    string message2 = "ATTACK AT NOON";

    string reused_key = "SECRETKEY12345";

    vector<unsigned char> M1(message1.begin(), message1.end());
    vector<unsigned char> M2(message2.begin(), message2.end());
    vector<unsigned char> K(reused_key.begin(), reused_key.end());
    
    cout << "Message 1 (M1): " << message1 << endl;
    cout << "Message 2 (M2): " << message2 << endl;
    cout << "Reused Key (K): " << reused_key << endl;

    vector<unsigned char> C1 = performXOR(M1, K);
    
    vector<unsigned char> C2 = performXOR(M2, K);

    cout << "Ciphertext 1 (C1 = M1 ^ K) has been generated." << endl;
    cout << "Ciphertext 2 (C2 = M2 ^ K) has been generated." << endl << endl;

    vector<unsigned char> C1_xor_C2 = performXOR(C1, C2);
    
    vector<unsigned char> M1_xor_M2 = performXOR(M1, M2);

    string attacker_result_str(C1_xor_C2.begin(), C1_xor_C2.end());
    string proof_result_str(M1_xor_M2.begin(), M1_xor_M2.end());
    
    cout << "Attacker computes (C1 ^ C2). Result: ";
    printBytesAsNumbers(C1_xor_C2); 
    
    cout << "We compute        (M1 ^ M2). Result: ";
    printBytesAsNumbers(M1_xor_M2);
    
    cout << "\nConclusion: The results are identical!" << endl;
    cout << "This shows that C1 ^ C2 = M1 ^ M2. This is a huge information leak." << endl;
    cout << "The strange characters and empty spaces in the result show where the" << endl;
    cout << "original messages were different and where they were the same." << endl;

    return 0; 
}