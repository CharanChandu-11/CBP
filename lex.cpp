#include <iostream>  
#include <string>    
#include <vector>    
#include <map>       
#include <set>        
#include <stack>      
#include <cctype>     
#include <iomanip>   
#include <algorithm>  
using namespace std;

int main() {
    string code;
    cout << "Enter code: ";
    getline(cin, code);

    for (int i = 0; i < code.size(); i++) {
        char ch = code[i];

        if (isspace(ch))
            continue;

        if (isalpha(ch)) {
            string token;
            while (isalnum(code[i])) {
                token += code[i];
                i++;
            }
            i--;
            cout << "Identifier: " << token << endl;
        }
        else if (isdigit(ch)) {
            string num;
            while (isdigit(code[i])) {
                num += code[i];
                i++;
            }
            i--;
            cout << "Number: " << num << endl;
        }
        else {
            cout << "Operator: " << ch << endl;
        }
    }
    return 0;
}
