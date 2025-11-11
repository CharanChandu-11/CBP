#include <bits/stdc++.h> 
using namespace std; 
 
struct Item { 
    string lhs; 
    string rhs; 
    int dot; 
    string lookahead; 
}; 
 
struct State { 
    vector<Item> items; 
}; 
 
map<string, vector<string>> grammar; 
vector<State> states; 
map<pair<int, string>, int> GOTO; 
map<pair<int, string>, string> ACTION; 
vector<string> symbols; 
string startSymbol; 
 
// Check equality of two items 
bool sameItem(const Item &a, const Item &b) { 
    return a.lhs == b.lhs && a.rhs == b.rhs && a.dot == b.dot && a.lookahead == b.lookahead; 
} 
 
// Check equality of two states 
bool sameState(const State &a, const State &b) { 
    if (a.items.size() != b.items.size()) return false; 
    for (auto &x : a.items) 
        if (find_if(b.items.begin(), b.items.end(), [&](const Item &y){ return sameItem(x, y); }) == 
b.items.end()) 
            return false; 
    return true; 
} 
 
// Closure operation 
 
 
State closure(State I) { 
    bool added = true; 
    while (added) { 
        added = false; 
        vector<Item> newItems = I.items; 
        for (auto &it : I.items) { 
            if (it.dot < it.rhs.size()) { 
                string sym(1, it.rhs[it.dot]); 
                if (grammar.count(sym)) { 
                    for (auto &prod : grammar[sym]) { 
                        Item newItem = {sym, prod, 0, it.lookahead}; 
                        bool exists = any_of(I.items.begin(), I.items.end(), [&](Item a){ return 
sameItem(a, newItem); }); 
                        if (!exists) { 
                            newItems.push_back(newItem); 
                            added = true; 
                        } 
                    } 
                } 
            } 
        } 
        I.items = newItems; 
    } 
    return I; 
} 
 
// Goto operation 
State GOTOfunc(State I, string X) { 
    State J; 
    for (auto &it : I.items) { 
        if (it.dot < it.rhs.size() && it.rhs[it.dot] == X[0]) { 
            Item temp = it; 
            temp.dot++; 
            J.items.push_back(temp); 
        } 
    } 
    return closure(J); 
} 
 
 
 
int findState(State &I) { 
    for (int i = 0; i < states.size(); i++) 
        if (sameState(states[i], I)) return i; 
    return -1; 
} 
 
void printStates() { 
    cout << "\nLR(0) Item Sets:\n"; 
    for (int i = 0; i < states.size(); i++) { 
        cout << "\nState " << i << ":\n"; 
        for (auto &it : states[i].items) 
            cout << "  " << it.lhs << " -> " 
                 << it.rhs.substr(0, it.dot) << "." << it.rhs.substr(it.dot) 
                 << " , " << it.lookahead << "\n"; 
    } 
} 
 
void buildLALR() { 
    queue<int> Q; 
    Q.push(0); 
 
    while (!Q.empty()) { 
        int i = Q.front(); Q.pop(); 
        for (auto X : symbols) { 
            State J = GOTOfunc(states[i], X); 
            if (J.items.empty()) continue; 
            int j = findState(J); 
            if (j == -1) { 
                states.push_back(J); 
                j = states.size() - 1; 
                Q.push(j); 
            } 
            GOTO[{i, X}] = j; 
        } 
    } 
 
    // ACTION/GOTO Table Construction 
    for (int i = 0; i < states.size(); i++) { 
        for (auto &it : states[i].items) { 
 
 
            if (it.dot < it.rhs.size()) { 
                string a(1, it.rhs[it.dot]); 
                if (!grammar.count(a)) 
                    ACTION[{i, a}] = "s" + to_string(GOTO[{i, a}]); 
            } else { 
                if (it.lhs == startSymbol + "'") 
                    ACTION[{i, "$"}] = "acc"; 
                else 
                    ACTION[{i, it.lookahead}] = "r(" + it.lhs + "->" + it.rhs + ")"; 
            } 
        } 
    } 
} 
 
void printTable() { 
    cout << "\nLALR(1) Parse Table:\n"; 
    set<string> terms, nonterms; 
    for (auto &sym : symbols) { 
        if (grammar.count(sym)) nonterms.insert(sym); 
        else terms.insert(sym); 
    } 
    terms.insert("$"); 
 
    cout << setw(10) << "State"; 
    for (auto &t : terms) cout << setw(10) << t; 
    for (auto &nt : nonterms) cout << setw(10) << nt; 
    cout << "\n--------------------------------------------------------------------------------\n"; 
 
    for (int i = 0; i < states.size(); i++) { 
        cout << setw(10) << i; 
        for (auto &t : terms) { 
            if (ACTION.count({i, t})) cout << setw(10) << ACTION[{i, t}]; 
            else cout << setw(10) << "-"; 
        } 
        for (auto &nt : nonterms) { 
            if (GOTO.count({i, nt})) cout << setw(10) << GOTO[{i, nt}]; 
            else cout << setw(10) << "-"; 
        } 
        cout << "\n"; 
 
 
    } 
} 
 
int main() { 
    int n; 
    cout << "Enter number of productions: "; 
    cin >> n; 
    cout << "Enter productions (e.g. E->E+T|T):\n"; 
 
    for (int i = 0; i < n; i++) { 
        string prod; 
        cin >> prod; 
        string lhs = prod.substr(0, 1); 
        string rhs = prod.substr(3); 
        stringstream ss(rhs); 
        string part; 
        while (getline(ss, part, '|')) 
            grammar[lhs].push_back(part); 
        if (i == 0) startSymbol = lhs; 
    } 
 
    // Collect all grammar symbols 
    for (auto &entry : grammar) 
        for (auto &rhs : entry.second) 
            for (char c : rhs) 
                symbols.push_back(string(1, c)); 
    sort(symbols.begin(), symbols.end()); 
    symbols.erase(unique(symbols.begin(), symbols.end()), symbols.end()); 
 
    // Augment grammar 
    grammar[startSymbol + "'"].push_back(startSymbol); 
    startSymbol += "'"; 
 
    // Initial state 
    State I0; 
    I0.items.push_back({startSymbol, grammar[startSymbol][0], 0, "$"}); 
    I0 = closure(I0); 
    states.push_back(I0); 
 
 
 
    buildLALR(); 
    printStates(); 
    printTable(); 
    return 0; 
}