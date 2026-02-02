#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
//
// Function to prepare the key matrix
void generateKeyTable(string key, char matrix[5][5]) {
    string keyString = "";
    bool map[26] = {0};

    // Add 'J' to map as handled by 'I' usually
    map['J' - 'A'] = true;
    // Process the key
    for (char c : key) {
        if (c == 'J') c = 'I'; // Treat J as I
        if (!map[c - 'A']) {
            keyString += c;
            map[c - 'A'] = true;
        }
    }

    // Fill remaining alphabet
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue;
        if (!map[c - 'A']) {
            keyString += c;
            map[c - 'A'] = true;
        }
    }

    // Fill the matrix
    int k = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matrix[i][j] = keyString[k++];
        }
    }
}

// Helper to find position of a character
void search(char matrix[5][5], char a, int &r, int &c) {
    if (a == 'J') a = 'I';
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (matrix[i][j] == a) {
                r = i;
                c = j;
                return;
            }
        }
    }
}

// Function to prepare text (add X for duplicates, make even length)
string prepareText(string str) {
    string temp = "";
    for (char c : str) {
        if (c == 'J') c = 'I'; // Standardize J to I
        if (isalpha(c)) temp += toupper(c);
    }
    
    string prepared = "";
    for (int i = 0; i < temp.length(); i++) {
        prepared += temp[i];
        // If next char is same or we are at end, insert X
        if (i + 1 < temp.length() && temp[i] == temp[i+1]) {
            prepared += 'X';
        }
    }
    
    // Make length even
    if (prepared.length() % 2 != 0) {
        prepared += 'X';
    }
    return prepared;
}

// Encrypt Function
string encrypt(string str, char matrix[5][5]) {
    string text = prepareText(str);
    string result = "";
    
    for (int i = 0; i < text.length(); i += 2) {
        int r1, c1, r2, c2;
        search(matrix, text[i], r1, c1);
        search(matrix, text[i+1], r2, c2);

        if (r1 == r2) { // Same Row
            result += matrix[r1][(c1 + 1) % 5];
            result += matrix[r2][(c2 + 1) % 5];
        } else if (c1 == c2) { // Same Col
            result += matrix[(r1 + 1) % 5][c1];
            result += matrix[(r2 + 1) % 5][c2];
        } else { // Rectangle
            result += matrix[r1][c2];
            result += matrix[r2][c1];
        }
    }
    return result;
}

// Decrypt Function
string decrypt(string str, char matrix[5][5]) {
    string result = "";
    
    for (int i = 0; i < str.length(); i += 2) {
        int r1, c1, r2, c2;
        search(matrix, str[i], r1, c1);
        search(matrix, str[i+1], r2, c2);

        if (r1 == r2) { // Same Row
            result += matrix[r1][(c1 - 1 + 5) % 5]; // +5 handles negative mod
            result += matrix[r2][(c2 - 1 + 5) % 5];
        } else if (c1 == c2) { // Same Col
            result += matrix[(r1 - 1 + 5) % 5][c1];
            result += matrix[(r2 - 1 + 5) % 5][c2];
        } else { // Rectangle
            result += matrix[r1][c2];
            result += matrix[r2][c1];
        }
    }
    return result;
}

int main() {
    char matrix[5][5];
    string key, text;

    cout << "Enter Key: ";
    cin >> key;
    // Normalize key to uppercase
    for(auto & c: key) c = toupper(c);

    generateKeyTable(key, matrix);

    cout << "Key Matrix:" << endl;
    for(int i=0; i<5; i++) {
        for(int j=0; j<5; j++) cout << matrix[i][j] << " ";
        cout << endl;
    }

    cout << "\nEnter Plaintext (no spaces): ";
    cin >> text;

    string encrypted = encrypt(text, matrix);
    cout << "Encrypted: " << encrypted << endl;

    string decrypted = decrypt(encrypted, matrix);
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}
