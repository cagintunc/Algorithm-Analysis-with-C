#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
struct Key {
    string team = "Nan";
    int shirt_number = 0;
};

struct Value {
    int assist;
    int goals;
    int score;
};
struct Node{
    Key* key = new Key();
    Value* value = new Value();
    bool initialized = false;
};
struct Event {
    int minute = 0;
    string team;
    int shirt = 0;
    string code;
};

class HashTable {
public:
    Node total[28];
    void show_table();
    void adjust_table(Node new_node);

};

int count_line(ifstream* stream);
void show_events(Event* events, int size);
int h(string team, int shirt);
int fhash(int h, int i, int p);
int get_score(string code);
int power(int x, int n);

int main(int argc, char** argv) {

    // This time no utility functions are provided.
    HashTable* table = new HashTable();
    // Do not forget to parse the command line argument.
    //string path = "../../../../data/example/events.csv";
    string path = argv[1];
    int min = atoi(argv[2]);
    ifstream testFile;
    testFile.open(path, ios::in);
    int count = count_line(&testFile);
    //cout << count << endl;
    testFile.open(path, ios::in);
    Event* events = new Event[count];
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
                events[current].minute = stoi(tokens[0]);
                events[current].team = tokens[1];
                events[current].shirt = stoi(tokens[2]);
                events[current].code = tokens[3];
                current++;
            }
        }
        n++;
    }
    for (int i = 0; i < count; i++) {
        if (events[i].minute <= min) {
            Node* new_node = new Node();
            (*new_node).key->team = events[i].team;
            (*new_node).key->shirt_number = events[i].shirt;
            bool is_located = false;
            int n = 0;
            int fh;
            while (!is_located) {
                fh = h((*new_node).key->team, (*new_node).key->shirt_number);
                int hashed = fhash(fh, n, 2);
                if (!table->total[hashed].initialized) {
                    table->total[hashed] = *new_node;
                    int updated = get_score(events[i].code);
                    table->total[hashed].value->score += updated;
                    table->total[hashed].initialized = true;
                    if (events[i].code == "S")
                        table->total[hashed].value->goals++;
                    else if (events[i].code == "A")
                        table->total[hashed].value->assist++;
                    is_located = true;
                }
                else {
                    bool same_num = table->total[hashed].key->shirt_number == (*new_node).key->shirt_number;
                    bool same_team = (table->total[hashed].key->team == (*new_node).key->team);
                    if (same_num && same_team) {
                        int updated = get_score(events[i].code);
                        table->total[hashed].value->score += updated;
                        if (events[i].code == "S")
                            table->total[hashed].value->goals++;
                        else if(events[i].code == "A")
                            table->total[hashed].value->assist++;
                        break;
                    }
                    else {
                        n++;
                    }
                }
            }
        }
        else
            break;
    }
    table->show_table();
    return 0;
}

void HashTable::show_table()
{
    cout << "Index\tTeam\tNo\tGoals\tAssists\tScore\n";
    int i = 0;
    while (i < 28) {
        if ((*total[i].key).team == "Nan")
            cout << i << endl;
        else {
            cout << i << "\t" << (*total[i].key).team << "\t" << (*total[i].key).shirt_number << "\t";
            cout << (*total[i].value).goals << "\t" << (*total[i].value).assist << "\t";
            cout << (*total[i].value).score << endl;
        }
        i++;
    }
}

int count_line(ifstream* stream) {
    int result = 0;
    while (stream->good()) {
        string temp;
        getline(*stream, temp, '\n');
        result++;
    }
    stream->close();
    return result - 1;
}

void show_events(Event* events, int size) {
    for (int i = 0; i < size; i++) {
        cout << events[i].minute << endl;
    }
}
int h(string team, int shirt) {
    int f = 99;
    if (team == "H") {
        f = 0;
    }
    return (f + shirt) % 28;
}

int fhash(int h, int i, int p) {
    return (int)(h + power(i, p)) % 28;
}

int get_score(string code) {
    int result = 0;
    if (code == "S")
        result = 20;
    else if (code == "A")
        result = 10;
    else if (code == "T")
        result = 1;
    else if (code == "P")
        result = 1;
    else if (code == "SH")
        result = 2;
    else if (code == "SG")
        result = 3;
    else if (code == "IP")
        result = -1;
    else if (code == "YC")
        result = -10;
    else if (code == "RC")
        result = -20;
    else if (code == "L")
        result = -2;
    else if (code == "CG")
        result = -5;
    return result;
}

int power(int x, int n) {
    if (n == 0)
        return 1;
    else
        return x * power(x, n - 1);
}