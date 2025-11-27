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
map<string, vector<vector<string>>> grammar;
map<string, set<string>> FIRST, FOLLOW;
string startSymbol;
bool isNonTerminal(const string &s) {
    return !s.empty() && isupper(s[0]);
}
vector<string> tokenizeRHS(const string &rhs) {
    vector<string> tokens;
    for (int i = 0; i < (int)rhs.size();) {
        char c = rhs[i];
        if (isspace(c)) { i++; continue; }
        if (isupper(c)) {
            string nt; nt.push_back(c);
            if (i + 1 < (int)rhs.size() && rhs[i+1] == '\'') { nt.push_back('\''); i += 2; }
            else i++;
            tokens.push_back(nt);
            continue;
        }
        // "id" terminal
        if (c == 'i' && i + 1 < (int)rhs.size() && rhs[i+1] == 'd') {
            tokens.push_back("id");
            i += 2;
            continue;
        }
        // epsilon marker
        if (c == '#') {
            tokens.push_back("#"); i++; continue;
        }
        // single-character terminal/operator: + * ( ) etc.
        string t(1, c);
        tokens.push_back(t);
        i++;
    }
    return tokens;
}
set<string> computeFIRST(const string &sym) {
    // Terminal or epsilon
    if (!isNonTerminal(sym) || sym == "#") {
        return set<string>{ sym };
    }
    if (!FIRST[sym].empty()) return FIRST[sym];
    set<string> res;

    for (auto &prod : grammar[sym]) {
        bool allEps = true;
        for (auto &s : prod) {
            set<string> f = computeFIRST(s);
            // add everything except epsilon
            for (auto &x : f) if (x != "#") res.insert(x);
            if (f.find("#") == f.end()) { allEps = false; break; }
        }
        if (allEps) res.insert("#");
    }
    FIRST[sym] = res;
    return res;
}
void computeFOLLOWs() {
    // initialize
    if (!startSymbol.empty()) FOLLOW[startSymbol].insert("$");

    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &rule : grammar) {
            const string &A = rule.first;
            for (auto &rhs : rule.second) {
                for (int i = 0; i < (int)rhs.size(); ++i) {
                    string B = rhs[i];
                    if (!isNonTerminal(B)) continue;
                    // Case: B followed by beta
                    if (i + 1 < (int)rhs.size()) {
                        // FIRST(beta)
                        set<string> firstBeta;
                        bool betaAllEps = true;
                        for (int k = i+1; k < (int)rhs.size(); ++k) {
                            set<string> f = computeFIRST(rhs[k]);
                            for (auto &x : f) if (x != "#") firstBeta.insert(x);
                            if (f.find("#") == f.end()) { betaAllEps = false; break; }
                        }
                        // add FIRST(beta) - {#} to FOLLOW(B)
                        int before = FOLLOW[B].size();
                        for (auto &x : firstBeta) FOLLOW[B].insert(x);
                        if (FOLLOW[B].size() != before) changed = true;

                        // add FOLLOW(A) to FOLLOW(B)
                        if (betaAllEps) {
                            int before2 = FOLLOW[B].size();
                            for (auto &x : FOLLOW[A]) FOLLOW[B].insert(x);
                            if (FOLLOW[B].size() != before2) changed = true;
                        }
                    } else {
                        // B is at end: add FOLLOW(A)
                        int before = FOLLOW[B].size();
                        for (auto &x : FOLLOW[A]) FOLLOW[B].insert(x);
                        if (FOLLOW[B].size() != before) changed = true;
                    }
                }
            }
        }
    }
}
int main() {
    int n;
    cout << "Enter number of productions: ";
    if (!(cin >> n)) return 0;
    cout << "Enter productions (like E=TE'):\n";
    string line;
    for (int i = 0; i < n; ++i) {
        cin >> line;                   
        // find separator '=' or "->"
        size_t pos = string::npos;
        pos = line.find('=');
        if (pos == string::npos) pos = line.find("->");
        if (pos == string::npos) { cerr << "Bad production format\n"; return 0; }

        string lhs = line.substr(0, pos);
        if (lhs.empty()) { cerr << "Empty LHS\n"; return 0; }
        if (i == 0) startSymbol = lhs;
        string rhs = line.substr(pos + (line[pos]=='=' ? 1 : 2));
        // split by '|'
        vector<string> parts;
        string cur="";
        for (char c : rhs) {
            if (c == '|') { parts.push_back(cur); cur=""; }
            else cur.push_back(c);
        }
        if (!cur.empty()) parts.push_back(cur);
        for (auto &p : parts) {
            vector<string> tokens = tokenizeRHS(p);
            grammar[lhs].push_back(tokens);
        }
    }

    // collect nonterminals: keys of grammar (already)
    // compute FIRST for all nonterminals
    for (auto &pr : grammar) computeFIRST(pr.first);

    // compute FOLLOW sets iteratively
    computeFOLLOWs();

    // print FIRST
    cout << "\nFIRST sets:\n";
    for (auto &pr : grammar) {
        cout << "FIRST(" << pr.first << ") = { ";
        for (auto &x : FIRST[pr.first]) cout << x << " ";
        cout << "}\n";
    }

    // print FOLLOW
    cout << "\nFOLLOW sets:\n";
    for (auto &pr : grammar) {
        cout << "FOLLOW(" << pr.first << ") = { ";
        for (auto &x : FOLLOW[pr.first]) cout << x << " ";
        cout << "}\n";
    }

    return 0;
}
// Enter number of productions: 5
// Enter productions (like E=TE'):
// E=TX
// X=+TX|#
// T=FY
// Y=*FY|#
// F=(E)|id


