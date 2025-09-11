#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

using namespace std;


int mod(int a, int m) {
    return (a % m + m) % m;
}

// Function to find the modular multiplicative inverse of a under modulo m
// Uses a simple search, suitable for small m like 26.
int modInverse(int a, int m) {
    a = mod(a, m);
    for (int x = 1; x < m; x++) {
        if (mod(a * x, m) == 1) {
            return x;
        }
    }
    // Return -1 if inverse doesn't exist
    return -1;
}

vector<vector<int>> matrixInverse(vector<vector<int>> K, int m) {
 
    int det = K[0][0] * K[1][1] - K[0][1] * K[1][0];
    det = mod(det, m);

    
    int det_inv = modInverse(det, m);
    if (det_inv == -1) {
        throw runtime_error("Matrix is not invertible (determinant has no modular inverse).");
    }

   
    vector<vector<int>> K_inv(2, vector<int>(2));
    K_inv[0][0] = mod(det_inv * K[1][1], m);
    K_inv[0][1] = mod(det_inv * -K[0][1], m);
    K_inv[1][0] = mod(det_inv * -K[1][0], m);
    K_inv[1][1] = mod(det_inv * K[0][0], m);

    return K_inv;
}


vector<int> encryptBlock(vector<int> P, vector<vector<int>> K, int m) {
    vector<int> C(2);
    C[0] = mod(P[0] * K[0][0] + P[1] * K[1][0], m);
    C[1] = mod(P[0] * K[0][1] + P[1] * K[1][1], m);
    return C;
}


vector<int> decryptBlock(vector<int> C, vector<vector<int>> K_inv, int m) {
    vector<int> P(2);
    P[0] = mod(C[0] * K_inv[0][0] + C[1] * K_inv[1][0], m);
    P[1] = mod(C[0] * K_inv[0][1] + C[1] * K_inv[1][1], m);
    return P;
}

int main() {
    string plaintext = "HELP";
    vector<vector<int>> K = {{3, 3}, {2, 5}};
    int mod_val = 26;

    string ciphertext = "";
    cout << "Encrypting..." << endl;
    for (size_t i = 0; i < plaintext.length(); i += 2) {
        
        vector<int> P(2);
        P[0] = plaintext[i] - 'A';
        P[1] = plaintext[i+1] - 'A';
        
        cout << "Plaintext block: [" << P[0] << ", " << P[1] << "]" << endl;

        vector<int> C = encryptBlock(P, K, mod_val);
        cout << "Ciphertext block: [" << C[0] << ", " << C[1] << "]" << endl;
   
        ciphertext += (char)(C[0] + 'A');
        ciphertext += (char)(C[1] + 'A');
    }
    cout << "Final Ciphertext: " << ciphertext << endl << endl;


    cout << "Decrypting..." << endl;
    string decrypted_text = "";
    try {
      
        vector<vector<int>> K_inv = matrixInverse(K, mod_val);
        cout << "Inverse Key Matrix K_inv:" << endl;
        cout << "[ " << K_inv[0][0] << " " << K_inv[0][1] << " ]" << endl;
        cout << "[ " << K_inv[1][0] << " " << K_inv[1][1] << " ]" << endl;

        for (size_t i = 0; i < ciphertext.length(); i += 2) {
           
            vector<int> C(2);
            C[0] = ciphertext[i] - 'A';
            C[1] = ciphertext[i+1] - 'A';

            cout << "Ciphertext block: [" << C[0] << ", " << C[1] << "]" << endl;

            vector<int> P_decrypted = decryptBlock(C, K_inv, mod_val);
            cout << "Decrypted block: [" << P_decrypted[0] << ", " << P_decrypted[1] << "]" << endl;
          
            decrypted_text += (char)(P_decrypted[0] + 'A');
            decrypted_text += (char)(P_decrypted[1] + 'A');
        }
        cout << "Final Decrypted Plaintext: " << decrypted_text << endl;

    } catch (const runtime_error& e) {
        cerr << "Error during decryption: " << e.what() << endl;
    }

    return 0;
}