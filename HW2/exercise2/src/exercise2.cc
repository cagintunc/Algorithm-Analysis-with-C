#include "exercise2_utils.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Player {
    string team = "Nan";
};

class BST {
public:
    Node* Insert(Node* new_node, Node* root);
    void Query(Node* root, int r_min, int r_max);
};

int get_count(ifstream* stream);
bool is_home(string code);

int main(int argc, char** argv) {

    string path = argv[1];
    int s_min = atoi(argv[2]);
    int s_max = atoi(argv[3]);

    BST* binary_tree = new BST();
    Node* root = NULL;


    ifstream stream;
    stream.open(path, ios::in);
    int count = get_count(&stream);
    stream.open(path, ios::in);
    int n = 0;
    int current = 0;
    while (stream.good()) {
        string line;
        getline(stream, line);
        if (n > 0) {
            vector<string> tokens;
            string token;
            stringstream ss(line);
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.capacity() > 0) {
                Node* new_node = new Node();
                new_node->is_home = is_home(tokens[0]);
                new_node->shirt_no = stoi(tokens[1]);
                new_node->score = stoi(tokens[2]);
                new_node->left = NULL;
                new_node->right = NULL;
                if (root != NULL)
                    binary_tree->Insert(new_node, root);
                else
                    root = new_node;
                print_binary_tree(*root);
            }
        }
        n++;
    }
    binary_tree->Query(root, s_min, s_max);
    return 0;
}

bool is_home(string code) {
    bool result = false;
    if (code == "H")
        result = true;
    return result;
}

int get_count(ifstream* stream) {
    int result = 0;
    while (stream->good()) {
        string temp;
        getline(*stream, temp, ' ');
        result++;
    }
    stream->close();
    return result - 1;
}

Node* BST::Insert(Node* new_node, Node* root) {
    if (root == NULL) {
        root = new_node;
    }
    else if (new_node->score <= root->score)
        root->left = Insert(new_node, root->left);
    else
        root->right = Insert(new_node, root->right);
    return root;
}
void BST::Query(Node* root, int r_min, int r_max) {
    if (root == NULL)
        return;
    if (r_min < root->score)
        Query(root->left, r_min, r_max);
    if ((r_min <= root->score) && (r_max >= root->score)) {
        if (root->is_home)
            cout << "H" << " " << root->shirt_no << " " << root->score << endl;
        else
            cout << "A" << " " << root->shirt_no << " " << root->score << endl;
    }
    Query(root->right, r_min, r_max);
}
