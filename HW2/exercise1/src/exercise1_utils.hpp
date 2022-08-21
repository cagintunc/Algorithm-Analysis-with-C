#ifndef EXERCISE1_UTILS_HPP
#define EXERCISE1_UTILS_HPP

#include <iostream>

/*
You are supposed to use Node and print_complete_binary_tree.
*/

struct Node {
    int score;
    bool is_home; // true if home, false if away
    int shirt_no;
};

void print_node(const Node &node) {
    std::cout << node.score << " (" << (node.is_home ? "H" : "A") << " " << node.shirt_no << ")" << std::endl;
}

void print_binary_subtree(const std::string& prefix, const Node* tree, const int tree_size, const int index) {
    bool isRight = index % 2 == 0;
    std::cout << prefix << (isRight ? "└─RIGHT───" : "├─LEFT────");
    if(index < tree_size) {
        print_node(tree[index]);
        std::string newPrefix = prefix + (isRight ? "          " : "│         ");
        std::cout << newPrefix << "│" << std::endl;
        print_binary_subtree(newPrefix, tree, tree_size, 2 * index + 1);
        std::cout << newPrefix << "│" << std::endl;
        print_binary_subtree(newPrefix, tree, tree_size, 2 * index + 2);
    } else {
        std::cout << "[]" << std::endl;
    }
}

void print_complete_binary_tree(const Node* tree, const int size) {
    if (size == 0) {
        return;
    }
    print_node(tree[0]);
    std::cout << "│" << std::endl;
    print_binary_subtree("", tree, size, 1);
    std::cout << "│" << std::endl;
    print_binary_subtree("", tree, size, 2);
    std::cout << std::endl;
}

#endif
