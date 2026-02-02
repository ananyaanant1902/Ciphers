#include <iostream>
#include <cmath>

using namespace std;

// Function to compute (base^exp) % mod
// We use this because standard pow() can overflow and isn't precise for modulo
long long power(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod; // Handle large base initially
    
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

int main() {
    long long P, G, a, b, x, y, ka, kb;
    
    cout << "--- Diffie-Hellman Key Exchange ---" << endl;
    
    // Step 1: Agree on Public Keys P and G
    // In reality, these are huge numbers defined by standards.
    // Example: P = 23, G = 9
    cout << "Enter a Prime number (P): ";
    cin >> P;
    cout << "Enter a Primitive Root (G): ";
    cin >> G;
    
    // Step 2: Choose Private Keys
    cout << "\n[A] Enter Private Key (a): ";
    cin >> a;
    
    cout << "[B] Enter Private Key (b): ";
    cin >> b;
    // Step 3: Calculate Public Values to Exchange
    // A calculates x = G^a mod P
    x = power(G, a, P);
    
    // B calculates y = G^b mod P
    y = power(G, b, P);
    
    cout << "A sends public key (x): " << x << endl;
    cout << "B sends public key (y): " << y << endl;
    
    cout << "\n--- Generating Shared Secret ---" << endl;
    
    // Step 4: Generate the Shared Secret Key
    // A computes secret using B's public key (y) and her private key (a)
    // ka = y^a mod P
    ka = power(y, a, P);
    
    // B computes secret using A's public key (x) and his private key (b)
    // kb = x^b mod P
    kb = power(x, b, P);
    
    cout << "A's Calculated Secret Key: " << ka << endl;
    cout << "B's Calculated Secret Key: "   << kb << endl;
    
    cout << "-----------------------------------" << endl;
    if (ka == kb) {
        cout << "Success! Both parties have agreed on the secret key: " << ka << endl;
    } else {
        cout << "Error: Keys do not match." << endl;
    }
    
    return 0;
}
