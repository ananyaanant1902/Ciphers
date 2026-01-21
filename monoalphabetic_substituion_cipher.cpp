#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    // encryption mapping
    vector<pair<char, char>> enc = {
        {'a', 'f'}, {'b', 'j'}, {'c', 'k'}, {'d', 'l'}, {'e', 'm'},
        {'f', 'n'}, {'g', 'o'}, {'h', 'p'}, {'i', 'q'}, {'j', 'r'},
        {'k', 's'}, {'l', 't'}, {'m', 'u'}, {'n', 'v'}, {'o', 'w'},
        {'p', 'x'}, {'q', 'y'}, {'r', 'z'}, {'s', 'a'}, {'t', 'b'},
        {'u', 'c'}, {'v', 'd'}, {'w', 'e'}, {'x', 'g'}, {'y', 'h'},
        {'z', 'i'}
    };
    
    string plaintext;
    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    // ENCRYPTION
    string ciphertext = "";
    for (char ch : plaintext) {
        if (ch >= 'a' && ch <= 'z') {
            for (auto &p : enc) {
                if (p.first == ch) {
                    ciphertext += p.second;
                    break;
                }
            }
        } else {
            ciphertext += ch;
        }
    }

    cout << "Ciphertext: " << ciphertext << endl;

    // DECRYPTION
    string deciphered = "";
    for (char ch : ciphertext) {
        if (ch >= 'a' && ch <= 'z') {
            for (auto &p : enc) {
                if (p.second == ch) {
                    deciphered += p.first;
                    break;
                }
            }
        } else {
            deciphered += ch;
        }
    }

    cout << "Deciphered text: " << deciphered << endl;

    return 0;
}
