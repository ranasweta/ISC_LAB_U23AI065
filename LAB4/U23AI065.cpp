#include <bits/stdc++.h>
using namespace std;
// Generate random substitution key
string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
string generateKey()
{
    string key = alphabet; // Make a local copy
    random_device rd;
    mt19937 g(rd());
    shuffle(key.begin(), key.end(), g);
    return key;
}


string encrypt(string text, string key)
{
    map<char, char> dict1;
    
    for (int i = 0; i < alphabet.length(); i++) {
        dict1[alphabet[i]] = key[i];
    }

    string cipher = "";
    for (char c : text) {
        if (dict1.find(c) != dict1.end())
            cipher += dict1[c];
        else
            cipher += c;
    }

    return cipher;
}

// Decrypt function (TO BE COMPLETED)
string decrypt(string cipher, string key)
{
    map<char, char> dict2;
    // Build reverse mapping: key[i] -> alphabet[i]
    for (int i = 0; i < alphabet.length(); i++) {
        dict2[key[i]] = alphabet[i];
    }
    string decrypted = "";
    for (char c : cipher) {
        if (dict2.find(c) != dict2.end())
            decrypted += dict2[c];
        else
            decrypted += c;
    }
    return decrypted;
}

int main()
{
    string key = generateKey();

    
    // Read plaintext from file
    ifstream fin("plaintext.txt");
    string plaintext((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();
    
    string cipher = encrypt(plaintext, key);
    
    // Write ciphertext to file
    ofstream fout("cipher.txt");
    // Print key mapping
    fout << "Key mapping:\n";
    for (int i = 0; i < alphabet.length(); i++) {
        fout << alphabet[i] << " -> " << key[i] << endl;
    }
    fout << cipher;
    fout.close();

    string decrypted = decrypt(cipher, key);

    // Write decrypted text to file
    ofstream fout2("decrypted.txt");
    fout2 << decrypted;
    fout2.close();

    // Optionally print to console
    cout << "\nCiphertext written to cipher.txt\n";
    cout << "Decrypted text written to decrypted.txt\n";
}