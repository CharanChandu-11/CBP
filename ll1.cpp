#include <iostream>  
#include <string>    
#include <vector>    
#include <map>       
#include <set>        
#include <stack>      
#include <cctype>     
#include <iomanip>   
#include <algorithm>  using namespace std;

map<string, vector<vector<string>>> grammar;
map<string, set<string>> FIRST, FOLLOW;
map<string, map<string, vector<string>>> parseTable;
string startSymbol;

bool isNonTerminal(const string &s) { return !s.empty() && isupper(s[0]); }

vector<string> tokenizeRHS(const string &rhs) {
    vector<string> tokens;
    for (int i = 0; i < (int)rhs.size();) {
        char c = rhs[i];
        if (isspace(c)) { i++; continue; }
        if (isupper(c)) { 
            string nt; 
            nt.push_back(c); 
            if(i+1<(int)rhs.size() && rhs[i+1]=='\''){nt.push_back('\'');i+=2;} 
            else i++; 
            tokens.push_back(nt); 
            continue; 
        }
        if (c=='i' && i+1<(int)rhs.size() && rhs[i+1]=='d'){tokens.push_back("id"); i+=2; continue;}
        if (c=='#'){tokens.push_back("#"); i++; continue;}
        tokens.push_back(string(1,c)); i++;
    }
    return tokens;
}

set<string> computeFIRST(const string &sym){
    if (!isNonTerminal(sym) || sym=="#") return {sym};
    if (!FIRST[sym].empty()) return FIRST[sym];
    set<string> res;
    for(auto &prod: grammar[sym]){
        bool allEps=true;
        for(auto &s: prod){
            set<string> f=computeFIRST(s);
            for(auto &x:f) if(x!="#") res.insert(x);
            if(f.find("#")==f.end()){ allEps=false; break; }
        }
        if(allEps) res.insert("#");
    }
    FIRST[sym]=res; return res;
}

void computeFOLLOWs(){
    if(!startSymbol.empty()) FOLLOW[startSymbol].insert("$");
    bool changed=true;
    while(changed){
        changed=false;
        for(auto &rule: grammar){
            string A=rule.first;
            for(auto &rhs: rule.second){
                for(int i=0;i<(int)rhs.size();i++){
                    string B=rhs[i];
                    if(!isNonTerminal(B)) continue;
                    if(i+1<(int)rhs.size()){
                        set<string> firstBeta; bool betaAllEps=true;
                        for(int k=i+1;k<(int)rhs.size();k++){
                            set<string> f=computeFIRST(rhs[k]);
                            for(auto &x:f) if(x!="#") firstBeta.insert(x);
                            if(f.find("#")==f.end()){betaAllEps=false; break;}
                        }
                        for(auto &x:firstBeta) FOLLOW[B].insert(x);
                        if(betaAllEps){
                            for(auto &x:FOLLOW[A]) FOLLOW[B].insert(x);
                        }
                    } else {
                        for(auto &x:FOLLOW[A]) FOLLOW[B].insert(x);
                    }
                }
            }
        }
    }
}

// Build LL(1) parsing table
void buildParseTable(){
    for(auto &A: grammar){
        string nt=A.first;
        for(auto &prod: A.second){
            set<string> firstAlpha;
            bool allEps=true;
            for(auto &s: prod){
                set<string> f=computeFIRST(s);
                for(auto &x:f) if(x!="#") firstAlpha.insert(x);
                if(f.find("#")==f.end()){allEps=false; break;}
            }
            for(auto &terminal: firstAlpha) parseTable[nt][terminal]=prod;
            if(allEps){
                for(auto &terminal: FOLLOW[nt]) parseTable[nt][terminal]=prod;
            }
        }
    }
}

// Print FIRST and FOLLOW sets
void printFirstFollow(){
    cout << "\nFIRST sets:\n";
    for(auto &pr: FIRST){
        cout << "FIRST(" << pr.first << ") = { ";
        for(auto &x: pr.second) cout << x << " ";
        cout << "}\n";
    }

    cout << "\nFOLLOW sets:\n";
    for(auto &pr: FOLLOW){
        cout << "FOLLOW(" << pr.first << ") = { ";
        for(auto &x: pr.second) cout << x << " ";
        cout << "}\n";
    }
}

// Print LL(1) parse table
void printParseTable(){
    set<string> terminals;
    for(auto &A: parseTable)
        for(auto &entry: A.second) terminals.insert(entry.first);

    cout<<"\nLL(1) Parsing Table:\n";
    cout<<setw(8)<<"NT";
    for(auto &t: terminals) cout<<setw(12)<<t;
    cout<<"\n";

    for(auto &A: parseTable){
        cout<<setw(8)<<A.first;
        for(auto &t: terminals){
            if(A.second.count(t)){
                vector<string> prod=A.second[t];
                string rhs="";
                for(auto &s: prod) rhs+=s;
                cout<<setw(12)<<A.first+"->"+rhs;
            } else cout<<setw(12)<<""; 
        }
        cout<<"\n";
    }
}

int main(){
    int n; 
    cout<<"Enter number of productions: "; cin>>n;
    string line;
    for(int i=0;i<n;i++){
        cin>>line;
        size_t pos=line.find('='); if(pos==string::npos) pos=line.find("->");
        string lhs=line.substr(0,pos);
        if(i==0) startSymbol=lhs;
        string rhs=line.substr(pos+(line[pos]=='='?1:2));
        vector<string> parts; string cur="";
        for(char c:rhs){ if(c=='|'){parts.push_back(cur); cur="";} else cur.push_back(c);}
        if(!cur.empty()) parts.push_back(cur);
        for(auto &p: parts){
            vector<string> tokens=tokenizeRHS(p);
            grammar[lhs].push_back(tokens);
        }
    }

    for(auto &pr:grammar) computeFIRST(pr.first);
    computeFOLLOWs();

    printFirstFollow();  // Print FIRST and FOLLOW sets

    buildParseTable();
    printParseTable();

    return 0;
}
