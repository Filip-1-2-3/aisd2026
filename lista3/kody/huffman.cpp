#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <algorithm>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node *left, *middle, *right;

    Node(char c, int f) : ch(c), freq(f), left(nullptr), middle(nullptr), right(nullptr) {}
    Node(int f, Node* l, Node* r) : ch('\0'), freq(f), left(l), middle(nullptr), right(r) {}
    Node(int f, Node* l, Node* m, Node* r) : ch('\0'), freq(f), left(l), middle(m), right(r) {}
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

class HuffmanBinary {
    Node* root;
    map<char, string> codes;

    void generate_codes(Node* node, string str) {
        if (!node) return;
        if (!node->left && !node->right) {
            codes[node->ch] = str;
        }
        generate_codes(node->left, str + "0");
        generate_codes(node->right, str + "1");
    }

    void delete_tree(Node* node) {
        if (!node) return;
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }

public:
    HuffmanBinary() : root(nullptr) {}
    ~HuffmanBinary() { delete_tree(root); }

    void build(const string& text) {
        map<char, int> freq_map;
        for (char c : text) freq_map[c]++;

        priority_queue<Node*, vector<Node*>, Compare> pq;
        for (auto pair : freq_map) {
            pq.push(new Node(pair.first, pair.second));
        }

        while (pq.size() > 1) {
            Node* left = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();
            int sum = left->freq + right->freq;
            pq.push(new Node(sum, left, right));
        }

        root = pq.top();
        codes.clear();
        generate_codes(root, "");
    }

    void print_codes() {
        cout << "--- Kody Huffman (Binarne) ---" << endl;
        for (auto pair : codes) {
            cout << "'" << pair.first << "': " << pair.second << endl;
        }
    }

    string encode(const string& text) {
        string encoded = "";
        for (char c : text) encoded += codes[c];
        return encoded;
    }
    
    int get_total_bits() {
        return 0;
    }
};

class HuffmanTernary {
    Node* root;
    map<char, string> codes;

    void generate_codes(Node* node, string str) {
        if (!node) return;
        if (!node->left && !node->middle && !node->right) {
            if (node->ch != '\0') {
                codes[node->ch] = str;
            }
            return;
        }
        generate_codes(node->left, str + "0");
        generate_codes(node->middle, str + "1");
        generate_codes(node->right, str + "2");
    }

    void delete_tree(Node* node) {
        if (!node) return;
        delete_tree(node->left);
        delete_tree(node->middle);
        delete_tree(node->right);
        delete node;
    }

public:
    HuffmanTernary() : root(nullptr) {}
    ~HuffmanTernary() { delete_tree(root); }

    void build(const string& text) {
        map<char, int> freq_map;
        for (char c : text) freq_map[c]++;

        priority_queue<Node*, vector<Node*>, Compare> pq;
        for (auto pair : freq_map) {
            pq.push(new Node(pair.first, pair.second));
        }

        if ((pq.size() - 1) % 2 != 0) {
            pq.push(new Node('\0', 0));
        }

        while (pq.size() > 1) {
            Node* left = pq.top(); pq.pop();
            Node* middle = pq.top(); pq.pop();
            Node* right = pq.top(); pq.pop();
            int sum = left->freq + middle->freq + right->freq;
            pq.push(new Node(sum, left, middle, right));
        }

        root = pq.top();
        codes.clear();
        generate_codes(root, "");
    }

    void print_codes() {
        cout << "--- Kody Huffman (Ternarne: 0, 1, 2) ---" << endl;
        for (auto pair : codes) {
            cout << "'" << pair.first << "': " << pair.second << endl;
        }
    }
    
    string encode(const string& text) {
        string encoded = "";
        for (char c : text) encoded += codes[c];
        return encoded;
    }
};

int main() {
    string text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nam eu leo ut nulla tristique venenatis quis ac lacus. Pellentesque eu libero bibendum, pulvinar mi bibendum, eleifend lorem. Quisque non velit iaculis, volutpat enim non, convallis sem. Nullam orci magna, maximus malesuada egestas quis, condimentum id est. Suspendisse et sapien pretium, ultricies nunc ut, porta lacus. Suspendisse vel imperdiet sapien, at scelerisque orci. Curabitur eget risus ut nisi viverra luctus id et ligula. ";
    
    cout << "Tekst wejsciowy: " << text << endl;
    cout << "Dlugosc tekstu: " << text.length() << " znakow" << endl;
    cout << "==========================================" << endl;

    HuffmanBinary h_bin;
    h_bin.build(text);
    h_bin.print_codes();
    
    string encoded_bin = h_bin.encode(text);
    cout << "Zakodowany (bin): " << encoded_bin << endl;
    cout << "Dlugosc bitowa: " << encoded_bin.length() << endl;
    cout << "==========================================" << endl;

    HuffmanTernary h_ter;
    h_ter.build(text);
    h_ter.print_codes();

    string encoded_ter = h_ter.encode(text);
    cout << "Zakodowany (ter): " << encoded_ter << endl;
    cout << "Dlugosc (trity): " << encoded_ter.length() << endl;
    
    double ratio = (double)encoded_ter.length() / encoded_bin.length();
    cout << "\nStosunek dlugosci (Ternary/Binary): " << ratio << endl;

    return 0;
}