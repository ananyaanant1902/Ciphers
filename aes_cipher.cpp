#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

// --- 1. Constants & Lookup Tables ---

// The S-Box (Substitution Box)
const unsigned char sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// The Inverse S-Box (Used for Decryption)
const unsigned char rsbox[256] = {
    0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d 
};

// Rcon (Round Constant)
const unsigned char Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 
};

// --- 2. Helper Functions ---

// General Galois Field Multiplication
// This function multiplies two bytes in the AES Galois Field
unsigned char gmul(unsigned char a, unsigned char b) {
    unsigned char p = 0;
    for (int i = 0; i < 8; i++) {
        if (b & 1) p ^= a;
        unsigned char hi_bit_set = (a & 0x80);
        a <<= 1;
        if (hi_bit_set) a ^= 0x1B; // XOR with irreducible polynomial
        b >>= 1;
    }
    return p;
}

// Key Expansion
void KeyExpansion(const unsigned char* key, unsigned char* expandedKeys) {
    for (int i = 0; i < 16; i++) expandedKeys[i] = key[i];

    int bytesGenerated = 16;
    int rconIteration = 1;
    unsigned char temp[4];

    while (bytesGenerated < 176) {
        for (int i = 0; i < 4; i++) temp[i] = expandedKeys[bytesGenerated - 4 + i];

        if (bytesGenerated % 16 == 0) {
            // Rotate
            unsigned char t = temp[0];
            temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;
            // Sub
            for (int i = 0; i < 4; i++) temp[i] = sbox[temp[i]];
            // Rcon
            temp[0] ^= Rcon[rconIteration++];
        }
        for (int i = 0; i < 4; i++) {
            expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - 16] ^ temp[i];
            bytesGenerated++;
        }
    }
}

// AddRoundKey
void AddRoundKey(unsigned char state[4][4], const unsigned char* roundKey) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state[j][i] ^= roundKey[i * 4 + j];
        }
    }
}

// --- 3. Encryption Steps ---

void SubBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = sbox[state[i][j]];
}

void ShiftRows(unsigned char state[4][4]) {
    unsigned char temp;
    // Row 1: Shift 1
    temp = state[1][0]; state[1][0] = state[1][1]; state[1][1] = state[1][2]; state[1][2] = state[1][3]; state[1][3] = temp;
    // Row 2: Shift 2
    temp = state[2][0]; state[2][0] = state[2][2]; state[2][2] = temp;
    temp = state[2][1]; state[2][1] = state[2][3]; state[2][3] = temp;
    // Row 3: Shift 3
    temp = state[3][3]; state[3][3] = state[3][2]; state[3][2] = state[3][1]; state[3][1] = state[3][0]; state[3][0] = temp;
}

void MixColumns(unsigned char state[4][4]) {
    unsigned char Tmp, Tm, t;
    for (int i = 0; i < 4; i++) {
        t = state[0][i];
        Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i];
        Tm = state[0][i] ^ state[1][i]; Tm = gmul(Tm, 2); state[0][i] ^= Tm ^ Tmp;
        Tm = state[1][i] ^ state[2][i]; Tm = gmul(Tm, 2); state[1][i] ^= Tm ^ Tmp;
        Tm = state[2][i] ^ state[3][i]; Tm = gmul(Tm, 2); state[2][i] ^= Tm ^ Tmp;
        Tm = state[3][i] ^ t;           Tm = gmul(Tm, 2); state[3][i] ^= Tm ^ Tmp;
    }
}

void AES_Encrypt(unsigned char* message, const unsigned char* key) {
    unsigned char state[4][4];
    unsigned char expandedKeys[176];
    KeyExpansion(key, expandedKeys);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = message[i + 4 * j];

    AddRoundKey(state, expandedKeys);

    for (int round = 1; round <= 9; round++) {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, expandedKeys + (16 * round));
    }

    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, expandedKeys + 160);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            message[i + 4 * j] = state[i][j];
}

// --- 4. Decryption Steps (Inverse) ---

void InvSubBytes(unsigned char state[4][4]) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = rsbox[state[i][j]];
}

void InvShiftRows(unsigned char state[4][4]) {
    unsigned char temp;
    // Row 1: Shift Right 1
    temp = state[1][3]; state[1][3] = state[1][2]; state[1][2] = state[1][1]; state[1][1] = state[1][0]; state[1][0] = temp;
    // Row 2: Shift Right 2
    temp = state[2][0]; state[2][0] = state[2][2]; state[2][2] = temp;
    temp = state[2][1]; state[2][1] = state[2][3]; state[2][3] = temp;
    // Row 3: Shift Right 3
    temp = state[3][0]; state[3][0] = state[3][1]; state[3][1] = state[3][2]; state[3][2] = state[3][3]; state[3][3] = temp;
}

void InvMixColumns(unsigned char state[4][4]) {
    unsigned char a, b, c, d;
    for (int i = 0; i < 4; i++) {
        a = state[0][i]; b = state[1][i]; c = state[2][i]; d = state[3][i];
        
        // Multiply by 0x0e, 0x0b, 0x0d, 0x09
        state[0][i] = gmul(a, 0x0e) ^ gmul(b, 0x0b) ^ gmul(c, 0x0d) ^ gmul(d, 0x09);
        state[1][i] = gmul(a, 0x09) ^ gmul(b, 0x0e) ^ gmul(c, 0x0b) ^ gmul(d, 0x0d);
        state[2][i] = gmul(a, 0x0d) ^ gmul(b, 0x09) ^ gmul(c, 0x0e) ^ gmul(d, 0x0b);
        state[3][i] = gmul(a, 0x0b) ^ gmul(b, 0x0d) ^ gmul(c, 0x09) ^ gmul(d, 0x0e);
    }
}

void AES_Decrypt(unsigned char* message, const unsigned char* key) {
    unsigned char state[4][4];
    unsigned char expandedKeys[176];
    KeyExpansion(key, expandedKeys);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            state[i][j] = message[i + 4 * j];

    // Initial Round (using the last key)
    AddRoundKey(state, expandedKeys + 160);

    // 9 Main Inverse Rounds
    for (int round = 9; round >= 1; round--) {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, expandedKeys + (16 * round));
        InvMixColumns(state);
    }

    // Final Inverse Round
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, expandedKeys);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            message[i + 4 * j] = state[i][j];
}

// --- 5. Main Function ---

int main() {
    cout << "========================================" << endl;
    cout << "      AES-128 Encrpytion/Decryption     " << endl;
    cout << "========================================" << endl;

    // 1. Get Text Input
    string plainText;
    cout << "Enter Text (max 16 chars): ";
    getline(cin, plainText);

    // Pad with zeros if less than 16 chars
    unsigned char paddedMessage[16] = {0}; 
    for(int i = 0; i < plainText.length() && i < 16; i++) {
        paddedMessage[i] = (unsigned char)plainText[i];
    }

    // 2. Define a Key (16 bytes)
    unsigned char key[16] = {
        'T', 'h', 'a', 't', 's', ' ', 'm', 'y', 
        ' ', 'K', 'u', 'n', 'g', ' ', 'F', 'u' 
    };

    cout << "\n[Input]" << endl;
    cout << "Plain Text: " << plainText << endl;
    cout << "Key:        Thats my Kung Fu" << endl;

    // 3. Encrypt
    AES_Encrypt(paddedMessage, key);

    cout << "\n[Encryption]" << endl;
    cout << "Ciphertext (Hex): ";
    for (int i = 0; i < 16; i++) {
        cout << hex << setfill('0') << setw(2) << (int)paddedMessage[i] << " ";
    }
    cout << dec << endl; // Switch back to decimal format

    // 4. Decrypt
    // We pass the ENCRYPTED 'paddedMessage' back into the decrypt function
    AES_Decrypt(paddedMessage, key);

    cout << "\n[Decryption]" << endl;
    cout << "Decrypted Text: ";
    // Print characters until we hit the padding zeros
    for (int i = 0; i < 16; i++) {
        if(paddedMessage[i] != 0)
            cout << (char)paddedMessage[i];
    }
    cout << endl;

    cout << "========================================" << endl;
    return 0;
}
