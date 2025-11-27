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

int prec(char op){ return (op=='+'||op=='-')?1:(op=='*'||op=='/')?2:0; }

string toPostfix(string infix){
    string post=""; stack<char> st;
    for(char c: infix){
        if(isalpha(c)) post+=c;
        else if(c=='(') st.push(c);
        else if(c==')'){ while(!st.empty()&&st.top()!='('){ post+=st.top(); st.pop(); } st.pop(); }
        else{ while(!st.empty()&&prec(st.top())>=prec(c)){ post+=st.top(); st.pop(); } st.push(c); }
    }
    while(!st.empty()){ post+=st.top(); st.pop(); }
    return post;
}

void genCode(string lhs, string post){
stack<string> st;
int t = 1;
for(char c : post){
if(isalpha(c)) st.push(string(1,c));
else{
string b = st.top(); st.pop();
string a = st.top(); st.pop();


        string temp = "t" + to_string(t++);
        
        cout << "LOAD " << a << "\n";
        cout << "LOAD " << b << "\n";
        
        if(c == '+') cout << "ADD\n";
        else if(c == '-') cout << "SUB\n";
        else if(c == '*') cout << "MUL\n";
        else cout << "DIV\n";
        
        cout << "STORE " << temp << "\n";  // store intermediate result
        st.push(temp);
    }
}
cout << "LOAD " << st.top() << "\n";   // load last temp
cout << "STORE " << lhs << "\n";

}


int main(){
    string expr;
    cout<<"Enter expression (like a=b+c*d): ";
    cin>>expr;

    string lhs=expr.substr(0,expr.find('='));
    string rhs=expr.substr(expr.find('=')+1);

    string post=toPostfix(rhs);
    cout<<"\nGenerated Machine Code:\n";
    genCode(lhs,post);
}