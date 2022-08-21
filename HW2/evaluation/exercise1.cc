#include "exercise1_utils.hpp"

#include <cstdlib>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Your heap functions can (but don't have to) be in these forms:
int count_line(ifstream* stream);
void heapify(Node* A, int n, int i);
void create_heap(Node* A, int capacity);
void push(Node heap[], int i, int size, Node entry);
Node pop(Node* heap, int size);


int main(int argc, char** argv) {

    /*string path = argv[1];
    int k = atoi(argv[2]);*/
    string path = "../../../../data/only3/scores.csv";
    int k = 2;
    ifstream testFile;
    testFile.open(path, ios::in);
    int count = count_line(&testFile);
    testFile.open(path, ios::in);
    Node* heap = new Node[count];
    int n = 0;
    int current = 0;
    while (testFile.good()) {
        string line;
        getline(testFile, line);
        if (n > 0) {
            vector<string> tokens;
            string token;
            stringstream ss(line);
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.capacity() > 0) {
                Node* new_node = new Node();
                if (tokens[0] == "A") {
                    new_node->is_home = false;
                }
                else {
                    new_node->is_home = true;
                }
                new_node->shirt_no = stoi(tokens[1]);
                new_node->score = stoi(tokens[2]);
                push(heap, current, count, *new_node);
                current++;
                print_complete_binary_tree(heap, current);
            }
        }
        n++;
    }
    for (int i = 0; i < k; i++) {
        Node removed = pop(heap, count);
        count--;
        if(removed.is_home) 
            cout << "H" << " " << removed.shirt_no << " " << removed.score << endl;
        else 
            cout << "A" << " " << removed.shirt_no << " " << removed.score << endl;
    }
}

void heapify(Node* A, int i, int n) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if ((left < n) && (A[largest].score < A[left].score))
        largest = left;
    if ((right < n) && (A[largest].score < A[right].score))
        largest = right;
    if (largest != i) {
        swap(A[i], A[largest]);
        heapify(A, largest, n);
    }
}
void create_heap(Node* a, int n) {
    int k;
    for (k = (n / 2)-1; k >= 0; k--) {
        heapify(a, k, n);
    }
}

int count_line(ifstream* stream) {
    int result = 0;
    while (stream->good()) {
        string temp;
        getline(*stream, temp, ' ');
        result++;
    }
    stream->close();
    return result - 1;
}
void push(Node heap[], int i, int size, Node entry) {
    heap[i] = entry;
    create_heap(heap, size);
}

Node pop(Node* heap, int size) {
    Node final_element = heap[size - 1];
    Node temp = heap[0];
    heap[0] = final_element;
    create_heap(heap, size-1);
    return temp;
}
