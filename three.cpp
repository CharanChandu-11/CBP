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
    string expr;
    cout << "Enter expression (like a=b+c*d): ";
    cin >> expr;

    string lhs = expr.substr(0, expr.find('='));
    string rhs = expr.substr(expr.find('=') + 1);
    vector<string> code;
    int t = 1;

    // convert to vector of strings for easy replacement
    vector<string> tokens;
    for (char c : rhs) tokens.push_back(string(1, c));

    // Step 1: handle * and /
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "*" || tokens[i] == "/") {
            string temp = "t" + to_string(t++);
            code.push_back(temp + " = " + tokens[i - 1] + " " + tokens[i] + " " + tokens[i + 1]);
            tokens[i - 1] = temp;
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i--;
        }
    }

    // Step 2: handle + and -
    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i] == "+" || tokens[i] == "-") {
            string temp = "t" + to_string(t++);
            code.push_back(temp + " = " + tokens[i - 1] + " " + tokens[i] + " " + tokens[i + 1]);
            tokens[i - 1] = temp;
            tokens.erase(tokens.begin() + i, tokens.begin() + i + 2);
            i--;
        }
    }

    code.push_back(lhs + " = " + tokens[0]);

    cout << "\nThree Address Code:\n";
    for (auto &x : code) cout << x << endl;
}