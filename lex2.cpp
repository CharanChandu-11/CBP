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
    cout << "Enter your C program (end input with a single line containing only 'END'):\n";
    string line, code;
    
    // Read multiple lines until user types "END"
    while (true) {
        getline(cin, line);
        if (line == "END") break;
        code += line + "\n";  // keep newline to separate lines
    }

    cout << "\nLexical Analysis Output:\n";

    for (int i = 0; i < code.size(); i++) {
        char ch = code[i];

        if (isspace(ch)) continue;

        // Identifiers (starts with letter, can have letters/digits)
        if (isalpha(ch)) {
            string token;
            while (i < code.size() && (isalnum(code[i]) || code[i] == '_')) {
                token += code[i];
                i++;
            }
            i--;
            cout << "Identifier: " << token << endl;
        }
        // Numbers
        else if (isdigit(ch)) {
            string num;
            while (i < code.size() && isdigit(code[i])) {
                num += code[i];
                i++;
            }
            i--;
            cout << "Number: " << num << endl;
        }
        // Operators and punctuation
        else {
            cout << "Operator/Symbol: " << ch << endl;
        }
    }

    return 0;
}
