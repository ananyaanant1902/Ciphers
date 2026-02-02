#include <iostream>
#include <cmath>

using namespace std;

// 1. Function to calculate Greatest Common Divisor (GCD)
// We need this to check if 'e' and 'phi' are coprime.
long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 2. Modular Exponentiation Algorithm
// This calculates (base^exp) % mod safely.
// Using standard pow() causes overflow with large numbers.
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        // If exp is odd, multiply base with result
        if (exp % 2 == 1)
            res = (res * base) % mod;
        
        // exp must be even now
        exp = exp / 2;
        base = (base * base) % mod;
    }
    return res;
}

// 3. Function to find Modular Inverse (calculate 'd')
// We need to find 'd' such that (d * e) % phi == 1
long long modInverse(long long e, long long phi) {
    for (long long d = 1; d < phi; d++) {
        if ((e * d) % phi == 1) {
            return d;
        }
    }
    return -1; // Should not happen if inputs are correct
}

int main() {
    long long p, q;
    
    cout << "--- RSA Algorithm Demo ---" << endl;
    cout << "NOTE: Enter small prime numbers (e.g., 11, 13, 17, 19) to avoid overflow." << endl;
    
    // Step 1: Input two prime numbers
    cout << "Enter First Prime (p): ";
    cin >> p;
    cout << "Enter Second Prime (q): ";
    cin >> q;

    // Step 2: Calculate n and phi
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    // Step 3: Choose 'e' (Public Key)
    // 'e' must be small, odd, and coprime to phi.
    // Commonly used 'e' values are 3, 17, 65537. We will pick a small one automatically.
    long long e = 2;
    while (e < phi) {
        if (gcd(e, phi) == 1) {
            break; 
        }
        e++;
    }

    // Step 4: Calculate 'd' (Private Key)
    long long d = modInverse(e, phi);

    cout << "\n-----------------------------------" << endl;
    cout << "Generated Keys:" << endl;
    cout << "Modulus (n): " << n << endl;
    cout << "Phi(n): " << phi << endl;
    cout << "Public Key (e): " << e << endl;
    cout << "Private Key (d): " << d << endl;
    cout << "-----------------------------------" << endl;

    // Step 5: Encryption
    long long msg;
    cout << "\nEnter message data (integer < " << n << "): ";
    cin >> msg;

    if (msg >= n) {
        cout << "Error: Message must be smaller than modulus n!" << endl;
        return 1;
    }

    // Encrypt: C = (msg ^ e) % n
    long long ciphertext = power(msg, e, n);
    cout << "Encrypted Ciphertext: " << ciphertext << endl;

    // Step 6: Decryption
    // Decrypt: M = (ciphertext ^ d) % n
    long long decryptedMsg = power(ciphertext, d, n);
    cout << "Decrypted Message: " << decryptedMsg << endl;

    return 0;
}
