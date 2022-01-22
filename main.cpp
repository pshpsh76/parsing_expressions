#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <set>

using namespace std;

template<typename Type>
ostream& operator<<(ostream& out, vector<Type>& data) {
    for (int i = 0; i < data.size() - 1; ++i) {
        if (data[i + 1] == ")" || data[i] == "(") {
            out << data[i];
        } else {
            out << data[i] << ' ';
        }
    }
    out << data[data.size() - 1];
    out << '\n';
    return out;
}

bool is_operation(string& s) {
    set<char> operations = {'+', '-', '*', '/', '(', ')', ' '};
    return operations.count(s[0]);
}

int find_last_min(vector<int>& data, int l, int r) {
    int last_min = 0;
    bool flag = false;
    for (int i = l; i < r + 1; ++i) {
        if (data[i] != 0) {
            if (!flag) {
                last_min = i;
                flag = true;
            } else if (data[last_min] >= data[i]){
                last_min = i;
            }
        }
    }
    return last_min;
}

bool is_num(string& val) {
    for (char c : val) {
        if (!('0' <= c && c <= '9')) {
            return false;
        }
    }
    return true;
}

class Node {
private:
    Node *right = nullptr, *left = nullptr;
    string val;
    string operation;
public:
    void build_tree_from_infix(vector<string>& data, vector<int>& balance, int l, int r) {
        if (l == r) {
            val = data[l];
            return;
        }
        if (r - l == 1) {
            if (data[l] == "(") {
                val = data[r];
            }
            if (data[r] == ")") {
                val = data[l];
            }
            return;
        }
        int last_min = find_last_min(balance, l, r);
        operation = data[last_min];
        right = new Node, left = new Node;
        right->build_tree_from_infix(data, balance, last_min + 1, r);
        left->build_tree_from_infix(data, balance, l, last_min - 1);
    }
    int build_tree_from_prefix(vector<string>& data, int i) {
        if (is_operation(data[i])) {
            operation = data[i];
            left = new Node;
            right = new Node;
            int j = left->build_tree_from_prefix(data, i + 1);
            return right->build_tree_from_prefix(data, j);
        } else {
            val = data[i];
            return i + 1;
        }
    }
    int build_tree_from_postfix(vector<string>& data, int i) {
        if (is_operation(data[i])) {
            operation = data[i];
            left = new Node;
            right = new Node;
            int j = right->build_tree_from_postfix(data, i - 1);
            return left->build_tree_from_postfix(data, j);
        } else {
            val = data[i];
            return i - 1;
        }
    }
    void to_postfix(vector<string>& result) {
        if (left == nullptr && right == nullptr) {
            result.push_back(val);
            return;
        }
        left->to_postfix(result);
        right->to_postfix(result);
        result.push_back(operation);
    }
    void to_prefix(vector<string>& result) {
        if (left == nullptr && right == nullptr) {
            result.push_back(val);
            return;
        }
        result.push_back(operation);
        left->to_prefix(result);
        right->to_prefix(result);
    }
    void to_infix(vector<string>& result) {
        if (left == nullptr && right == nullptr) {
            result.push_back(val);
            return;
        }
        result.push_back("(");
        left->to_infix(result);
        result.push_back(operation);
        right->to_infix(result);
        result.push_back(")");
    }
};

void remove_spaces(string& inp) {
    for (int i = 0; i < inp.size(); ++i) {
        if (inp[i] == ' ') {
            inp.erase(inp.begin() + i);
            --i;
        }
    }
}

void remove_empty(vector<string>& data) {
    int i = 0;
    while (true) {
        if (i == data.size()) {
            break;
        }
        if (data[i] == "" || data[i] == " ") {
            data.erase(data.begin() + i);
            --i;
        }
        ++i;
    }
}

vector<string> split(string& inp) {
    set<char> operations = {'+', '-', '*', '/', '(', ')', ' '};
    bool cut = false;
    int last = 0;
    vector<string> res;
    for (int i = 0; i < inp.size(); ++i) {
        if (operations.count(inp[i])) {
            res.push_back(inp.substr(last, i - last));
            res.push_back(inp.substr(i, 1));
            last = i + 1;
        }
        if (i == inp.size() - 1) {
            res.push_back(inp.substr(last, i - last + 1));
        }
    }
    return res;
}

vector<int> make_bal(vector<string>& data) {
    vector<int> res(data.size());
    int bal = 0;
    for (int i = 0; i < data.size(); ++i) {
        if (data[i] == "(") {
            bal += 10;
        }
        else if (data[i] == ")") {
            bal -= 10;
        }
        else if (data[i] == "+" || data[i] == "-") {
            res[i] = bal + 1;
        }
        else if (data[i] == "*" || data[i] == "/") {
            res[i] = bal + 2;
        }
    }
    return res;
}

Node* make_tree_from_infix(string& inp) {
    remove_spaces(inp);
    vector<string> data = split(inp);
    remove_empty(data);
    vector<int> balance = make_bal(data);
    Node* pnode = new Node;
    pnode->build_tree_from_infix(data, balance, 0, data.size() - 1);
    return pnode;
}

Node* make_tree_from_prefix(string& inp) {
    vector<string> data = split(inp);
    remove_empty(data);
    Node* pnode = new Node;
    pnode->build_tree_from_prefix(data, 0);
    return pnode;
}

Node* make_tree_from_postfix(string& inp) {
    vector<string> data = split(inp);
    remove_empty(data);
    Node* pnode = new Node;
    pnode->build_tree_from_postfix(data, (int)data.size() - 1);
    return pnode;
}

class Expression{
private:
    Node* root;
public:
    Expression(string& input, string& type) {
        if (type == "infix") {
            root = make_tree_from_infix(input);
        }
        else if (type == "prefix") {
            root = make_tree_from_prefix(input);
        } else {
            root = make_tree_from_postfix(input);
        }
    }
    vector<string> to_infix() {
        vector<string> res;
        root->to_infix(res);
        return res;
    }
    vector<string> to_prefix() {
        vector<string> res;
        root->to_prefix(res);
        return res;
    }
    vector<string> to_postfix() {
        vector<string> res;
        root->to_postfix(res);
        return res;
    }
};

int main() {
    string input, type;
    getline(cin, type);
    getline(cin, input);
    Expression exp(input, type);
    vector<string> result;
    if (type == "prefix") {
        result = exp.to_infix();
        cout << result;
        result = exp.to_postfix();
        cout << result;
    }
    if (type == "postfix") {
        result = exp.to_infix();
        cout << result;
        result = exp.to_prefix();
        cout << result;
    }
    if (type == "infix") {
        result = exp.to_prefix();
        cout << result;
        result = exp.to_postfix();
        cout << result;
    }
}
