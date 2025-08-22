#include <bits/stdc++.h>
using namespace std;
unordered_map<char, int> alphabet_map;
unordered_map<int,char> number_map;

string encrypt(string& plain_text){
    string encrypted_text;
    int num;
    for(auto i: plain_text){
        num = (alphabet_map[i]+3)%26;
        encrypted_text += number_map[num];
    }
    return encrypted_text;
}
string decrypt(string& encrypted_text){
    string plain_text;
    for(auto i: encrypted_text){
        int num = alphabet_map[i];
        num -= 3;
        if(num < 0) num = 26+num;
        plain_text += number_map[num];
    }
    return plain_text;
}
int main(){
    for (char letter = 'a'; letter <= 'z'; ++letter) {
        alphabet_map[letter] = letter - 'a';
        number_map[letter - 'a'] = letter;
    }

    // Read input from file
    ifstream infile("input.txt");
    string s;
    infile >> s;
    infile.close();

    string encrypted_text = encrypt(s);
    ofstream encrypt_file("encrypt.txt");
    encrypt_file << encrypted_text << endl;
    encrypt_file.close();

    string decrypted_text = decrypt(encrypted_text);
    ofstream decrypt_file("decrypt_plain_text.txt");
    decrypt_file << decrypted_text << endl;
    decrypt_file.close();

    return 0;
}