#include <bits/stdc++.h>
using namespace std;

map<char,int> char_to_num;
map<int,char> num_to_char;

string shiftEncrypt(const string &input){
    string result;
    for(char ch : input){
        int val = (char_to_num[ch] + 6) % 26;
        result.push_back(num_to_char[val]);
    }
    return result;
}

string shiftDecrypt(const string &input){
    string result;
    for(char ch : input){
        int val = char_to_num[ch] - 6;
        if(val < 0) val += 26;
        result.push_back(num_to_char[val]);
    }
    return result;
}

int main(){
    // initialize mappings
    for(int i=0;i<26;i++){
        char_to_num['a'+i] = i;
        num_to_char[i] = 'a'+i;
    }

    string text;
    cout << "Enter text: ";
    cin >> text;

    string enc = shiftEncrypt(text);
    cout << "Encrypted: " << enc << "\n";

    string dec = shiftDecrypt(enc);
    cout << "Decrypted: " << dec << "\n";
    return 0;
}
