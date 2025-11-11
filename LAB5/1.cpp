 #include <bits/stdc++.h>
using namespace std;

vector<vector<char>> Key_square(5, vector<char>(5));
string keyword;
map<char, pair<int, int>> vis;

void create_key_square()
{
    int x = 0, y = 0;
    for (auto i : keyword)
    {
        if (i >= 'a' && i <= 'z')
        {
            if (vis.find(i) == vis.end())
            {
                vis[i] = {x, y};
                if (i == 'j')
                {
                    vis['i'] = {x, y};
                    Key_square[x][y++] = 'i';
                }
                else if (i == 'i')
                {
                    vis['j'] = {x, y};
                    Key_square[x][y++] = 'i';
                }
                else
                    Key_square[x][y++] = i;

                if (y == 5)
                {
                    y = 0;
                    x++;
                }
            }
        }
    }

    for (char i = 'a'; i <= 'z'; i++)
    {
        if (vis.find(i) == vis.end())
        {
            vis[i] = {x, y};
            if (i == 'i')
                vis['j'] = {x, y};

            Key_square[x][y++] = i;
            if (y == 5)
            {
                y = 0;
                x++;
            }
        }
    }
}

void print_key_square()
{
    for (auto row : Key_square)
    {
        for (auto col : row)
            if (col == 'i')
                cout << "i/j ";
            else
                cout << col << " ";
        cout << endl;
    }
}

vector<string> digraphs;
void create_digraphs(const string &plaintext)
{
    digraphs.clear();
    for (int i = 0; i < (int)plaintext.size(); i++)
    {
        if (i == (int)plaintext.size() - 1)
        {
            digraphs.push_back(string(1, plaintext[i]) + 'x');
            break;
        }
        if (plaintext[i] == plaintext[i + 1])
        {
            digraphs.push_back(string(1, plaintext[i]) + 'x');
        }
        else
        {
            digraphs.push_back(string(1, plaintext[i]) + string(1, plaintext[i + 1]));
            i++;
        }
    }
}

vector<string> ciphertext;
vector<pair<int, char>> non_alpha_positions;

void encrypt(const string &raw_input)
{
    ciphertext.clear();
    non_alpha_positions.clear();

    // Preprocess input: keep only alphabets, convert to lowercase
    string plaintext;
    for (int i = 0; i < raw_input.size(); i++)
    {
        char c = raw_input[i];
        if (isalpha(c))
        {
            char lo = tolower(c);
            if (lo == 'j')
                lo = 'i';
            plaintext.push_back(lo);
        }
        else
        {
            non_alpha_positions.push_back({(int)plaintext.size(), c});
        }
    }

    if (plaintext.empty())
    {
        cerr << "Error: No valid alphabetic characters found in input." << endl;
        return;
    }


    create_digraphs(plaintext);

    for (auto digraph : digraphs)
    {
        char first = digraph[0];
        char second = digraph[1];
        auto pos1 = vis[first];
        auto pos2 = vis[second];
        if (pos1.first == pos2.first)
        {
            ciphertext.push_back(string(1, Key_square[pos1.first][(pos1.second + 1) % 5]) +
                                 string(1, Key_square[pos2.first][(pos2.second + 1) % 5]));
        }
        else if (pos1.second == pos2.second)
        {
            ciphertext.push_back(string(1, Key_square[(pos1.first + 1) % 5][pos1.second]) +
                                 string(1, Key_square[(pos2.first + 1) % 5][pos2.second]));
        }
        else
        {
            ciphertext.push_back(string(1, Key_square[pos1.first][pos2.second]) +
                                 string(1, Key_square[pos2.first][pos1.second]));
        }
    }

    cout << "\nCiphertext: ";
    for (auto &c : ciphertext)
        cout << (char)toupper(c[0]) << (char)toupper(c[1]);
    cout << endl;
}

string decrypt_and_restore(const string &cipher_input)
{
    string result;
    vector<string> cipher_digraphs;

    // Split ciphertext into digraphs
    for (int i = 0; i < (int)cipher_input.size(); i += 2)
    {
        if (i + 1 < (int)cipher_input.size())
            cipher_digraphs.push_back(string(1, tolower(cipher_input[i])) +
                                      string(1, tolower(cipher_input[i + 1])));
    }

    for (auto digraph : cipher_digraphs)
    {
        char first = digraph[0];
        char second = digraph[1];
        auto pos1 = vis[first];
        auto pos2 = vis[second];
        if (pos1.first == pos2.first)
        {
            result += string(1, Key_square[pos1.first][(pos1.second - 1 + 5) % 5]);
            result += string(1, Key_square[pos2.first][(pos2.second - 1 + 5) % 5]);
        }
        else if (pos1.second == pos2.second)
        {
            result += string(1, Key_square[(pos1.first - 1 + 5) % 5][pos1.second]);
            result += string(1, Key_square[(pos2.first - 1 + 5) % 5][pos2.second]);
        }
        else
        {
            result += string(1, Key_square[pos1.first][pos2.second]);
            result += string(1, Key_square[pos2.first][pos1.second]);
        }
    }

    // Convert to uppercase
    for (auto &c : result)
        c = toupper(c);

    return result;
}

int main()
{
    int option;
    cout << "Welcome to the Playfair Cipher Program!" << endl;
    cout << "Choose an option:\n";
    cout << "1. Encrypt\n";
    cout << "2. Decrypt\n";
    cout << "Enter your choice: ";
    cin >> option;
    cin.ignore();

    if (option < 1 || option > 2)
    {
        cerr << "Error: Invalid option selected." << endl;
        return 1;
    }

    cout << "Enter the keyword: ";
    getline(cin, keyword);
    for (auto &c : keyword)
        if (isalpha(c))
            c = tolower(c);

    create_key_square();
    cout << "\nKey Square:" << endl;
    print_key_square();

    if (option == 1)
    {
        cout << "\nEnter plaintext: ";
        string raw_input;
        getline(cin, raw_input);
        encrypt(raw_input);
    }
    else
    {
        cout << "\nEnter ciphertext (letters only, no spaces/punctuation): ";
        string cipher_input;
        getline(cin, cipher_input);

        string decrypted_text = decrypt_and_restore(cipher_input);
        cout << "Decrypted text: " << decrypted_text << endl;
    }

    return 0;
}
 