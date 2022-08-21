#ifndef EXERCISE2_UTILS_HPP
#define EXERCISE2_UTILS_HPP

#include <iostream>

/*
You are supposed to use Node and print_binary_tree.
*/

struct Node {
    int score;
    bool is_home; // true if home, false if away
    int shirt_no;
    Node* left;
    Node* right;
};

void print_node(const Node* node) {
    if (node == nullptr) {
        std::cout << "[]" << std::endl;
    } else {
        std::cout << node->score << " (" << (node->is_home ? "H" : "A") << " " << node->shirt_no << ")" << std::endl;
    }
}

void print_binary_subtree(const std::string& prefix, const Node* node, bool isRight) {
    std::cout << prefix << (isRight ? "└─RIGHT───" : "├─LEFT────");
    print_node(node);
    if(node != nullptr) {
        std::string newPrefix = prefix + (isRight ? "          " : "│         ");
        std::cout << newPrefix << "│" << std::endl;
        print_binary_subtree(newPrefix, node->left, false);
        std::cout << newPrefix << "│" << std::endl;
        print_binary_subtree(newPrefix, node->right, true);
    }
}

void print_binary_tree(const Node &root) {
    print_node(&root);
    std::cout << "│" << std::endl;
    print_binary_subtree("", root.left, false);
    std::cout << "│" << std::endl;
    print_binary_subtree("", root.right, true);
}

#endif
