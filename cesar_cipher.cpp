#include <iostream>
#include <string>
void decrypt(std::string &st, int key){
     for (int i = 0; i < st.size(); i++) {
    char ch = st[i];
    if (ch >= 'a' && ch <= 'z') {  
        ch = (ch - 'a' - key) % 26 + 'a';
    }
    st[i] = ch; 
}
}
int main(){
    std::string st="";
    std::cout<<"Enter a string to encrypt: ";
    std::cin>>st;
    int key;
    std::cin>>key;
    std::cout<<" string: "<<st;
    for (int i = 0; i < st.size(); i++) {
    char ch = st[i];
    if (ch >= 'a' && ch <= 'z') {  
        ch = (ch - 'a' + key) % 26 + 'a';
    }
    st[i] = ch; 
}
std::cout << "Encrypted string: " << st << "\n";
decrypt(st,key);
std::cout << "Decrypted string: " << st << "\n";
   // std::cout<<st;
    return 0;
}
