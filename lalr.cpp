#include <bits/stdc++.h>
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

        // Nonterminal like E or E'
        if (isupper(c)) {
            string nt; nt.push_back(c);
            if (i + 1 < (int)rhs.size() && rhs[i+1] == '\'') { nt.push_back('\''); i += 2; }
            else i++;
            tokens.push_back(nt);
            continue;
        }

        // id terminal
        if (c == 'i' && i + 1 < (int)rhs.size() && rhs[i+1] == 'd') {
            tokens.push_back("id"); i += 2; continue;
        }

        // epsilon
        if (c == '#') { tokens.push_back("#"); i++; continue; }

        // single-char terminal
        string t(1, c);
        tokens.push_back(t);
        i++;
    }
    return tokens;
}

//--------------------------------------
// FIRST and FOLLOW (same as your earlier code)
//--------------------------------------
set<string> computeFIRST(const string &sym) {
    if (!isNonTerminal(sym) || sym == "#")
        return { sym };

    if (!FIRST[sym].empty()) return FIRST[sym];

    set<string> res;
    for (auto &prod : grammar[sym]) {
        bool allEps = true;
        for (auto &s : prod) {
            set<string> f = computeFIRST(s);
            for (auto &x : f) if (x != "#") res.insert(x);
            if (f.find("#") == f.end()) { allEps = false; break; }
        }
        if (allEps) res.insert("#");
    }
    FIRST[sym] = res;
    return res;
}

void computeFOLLOWs() {
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
                        set<string> firstBeta;
                        bool betaAllEps = true;
                        for (int k = i+1; k < (int)rhs.size(); ++k) {
                            set<string> f = computeFIRST(rhs[k]);
                            for (auto &x : f) if (x != "#") firstBeta.insert(x);
                            if (f.find("#") == f.end()) { betaAllEps = false; break; }
                        }

                        int before = FOLLOW[B].size();
                        for (auto &x : firstBeta) FOLLOW[B].insert(x);
                        if (FOLLOW[B].size() != before) changed = true;

                        if (betaAllEps) {
                            int before2 = FOLLOW[B].size();
                            for (auto &x : FOLLOW[A]) FOLLOW[B].insert(x);
                            if (FOLLOW[B].size() != before2) changed = true;
                        }
                    } 
                    else {
                        int before = FOLLOW[B].size();
                        for (auto &x : FOLLOW[A]) FOLLOW[B].insert(x);
                        if (FOLLOW[B].size() != before) changed = true;
                    }
                }
            }
        }
    }
}

//--------------------------------------
// STRUCT for LR(0) Item
//--------------------------------------
struct Item {
    string lhs;
    vector<string> rhs;
    int dot;
    bool operator<(const Item &o) const {
        if (lhs != o.lhs) return lhs < o.lhs;
        if (rhs != o.rhs) return rhs < o.rhs;
        return dot < o.dot;
    }
    bool operator==(const Item &o) const {
        return lhs == o.lhs && rhs == o.rhs && dot == o.dot;
    }
};

//--------------------------------------
// Closure and GOTO functions (simplified)
//--------------------------------------
vector<Item> closure(vector<Item> I) {
    set<Item> result(I.begin(), I.end());
    bool changed = true;
    while (changed) {
        changed = false;
        vector<Item> items(result.begin(), result.end());
        for (auto &it : items) {
            if (it.dot < (int)it.rhs.size() && isNonTerminal(it.rhs[it.dot])) {
                string B = it.rhs[it.dot];
                for (auto &prod : grammar[B]) {
                    Item newItem = { B, prod, 0 };
                    if (!result.count(newItem)) {
                        result.insert(newItem);
                        changed = true;
                    }
                }
            }
        }
    }
    return vector<Item>(result.begin(), result.end());
}

vector<Item> GOTO(vector<Item> I, string X) {
    vector<Item> moved;
    for (auto &it : I) {
        if (it.dot < (int)it.rhs.size() && it.rhs[it.dot] == X)
            moved.push_back({ it.lhs, it.rhs, it.dot + 1 });
    }
    return closure(moved);
}

//--------------------------------------
// MAIN
//--------------------------------------
int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cout << "Enter productions (like E->E+T):\n";

    string line;
    for (int i = 0; i < n; ++i) {
        cin >> line;
        size_t pos = line.find("->");
        string lhs = line.substr(0, pos);
        string rhs = line.substr(pos + 2);

        if (i == 0) startSymbol = lhs;

        vector<string> parts;
        string cur = "";
        for (char c : rhs) {
            if (c == '|') { parts.push_back(cur); cur = ""; }
            else cur.push_back(c);
        }
        if (!cur.empty()) parts.push_back(cur);

        for (auto &p : parts)
            grammar[lhs].push_back(tokenizeRHS(p));
    }

    // FIRST/FOLLOW
    for (auto &p : grammar) computeFIRST(p.first);
    computeFOLLOWs();

    // Augment
    string aug = startSymbol + "'";
    grammar[aug].push_back({ startSymbol });
    startSymbol = aug;

    // LR(0) CLOSURE SETS
    vector<vector<Item>> C;
    C.push_back(closure({ {startSymbol, grammar[startSymbol][0], 0} }));

    bool changed = true;
    while (changed) {
        changed = false;
        for (int i = 0; i < (int)C.size(); ++i) {
            set<string> symbols;
            for (auto &it : C[i]) {
                if (it.dot < (int)it.rhs.size()) symbols.insert(it.rhs[it.dot]);
            }
            for (auto &X : symbols) {
                auto next = GOTO(C[i], X);
                if (next.empty()) continue;

                bool exists = false;
                for (auto &s : C) if (s == next) { exists = true; break; }
                if (!exists) { C.push_back(next); changed = true; }
            }
        }
    }

    int states = C.size();

    //--------------------------------------
    // BUILD ACTION & GOTO TABLES
    //--------------------------------------
    vector<map<string,string>> ACTION(states);
    vector<map<string,int>> GOTOtable(states);

    // Build transitions
    vector<map<string,int>> transitions(states);

    for (int i = 0; i < states; i++) {
        set<string> symbols;
        for (auto &it : C[i])
            if (it.dot < it.rhs.size()) symbols.insert(it.rhs[it.dot]);

        for (auto &X : symbols) {
            auto next = GOTO(C[i], X);
            if (next.empty()) continue;

            int j = -1;
            for (int k = 0; k < states; k++)
                if (C[k] == next) j = k;

            transitions[i][X] = j;
        }
    }

    // Fill ACTION and GOTO
    for (int i = 0; i < states; i++) {
        for (auto &it : C[i]) {
            if (it.dot < it.rhs.size()) {
                string a = it.rhs[it.dot];
                if (!isNonTerminal(a))
                    ACTION[i][a] = "s" + to_string(transitions[i][a]);
                else
                    GOTOtable[i][a] = transitions[i][a];
            } else {
                if (it.lhs == startSymbol)
                    ACTION[i]["$"] = "accept";
                else {
                    string prod = it.lhs + "->";
                    for (auto &x : it.rhs) prod += x;
                    for (auto &f : FOLLOW[it.lhs]) {
                        ACTION[i][f] = "r(" + prod + ")";
                    }
                }
            }
        }
    }


    cout << "\n================ LALR PARSING TABLE ================\n";

    // collect terminals + $
    set<string> terminals;
    for (auto &p : ACTION)
        for (auto &q : p) terminals.insert(q.first);
    terminals.insert("$");

    // collect non-terminals
    set<string> nonterminals;
    for (auto &p : GOTOtable)
        for (auto &q : p) nonterminals.insert(q.first);

    // Print header
    cout << "\nSTATE\t";
    for (auto &t : terminals) cout << t << "\t";
    for (auto &nt : nonterminals) cout << nt << "\t";
    cout << "\n------------------------------------------------------\n";

    // print rows
    for (int i = 0; i < states; i++) {
        cout << i << "\t";
        for (auto &t : terminals) {
            if (ACTION[i].count(t)) cout << ACTION[i][t];
            cout << "\t";
        }
        for (auto &nt : nonterminals) {
            if (GOTOtable[i].count(nt)) cout << GOTOtable[i][nt];
            cout << "\t";
        }
        cout << "\n";
    }

    return 0;
}

// Enter number of productions: 3
// Enter productions (like E->E+T):
// S->A
// A->aA
// A->b

// ================ LALR PARSING TABLE ================

// STATE	$	a	b	A	S	
// ------------------------------------------------------
// 0		s3	s4	1	2	
// 1	r(S->A)					
// 2	accept					
// 3		s3	s4	5		
// 4	r(A->b)					
// 5	r(A->aA)					
