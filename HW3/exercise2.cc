#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>


using namespace std;

void printingPath(int player_number,vector <vector<float>> player_matrix, vector  <int> pla_path){
    while(player_matrix[player_number][player_number]==0){
        pla_path.push_back(player_number);
        //content.erase(content.begin()+player_number-1);
        float min_time ;
        int next_player_number=-1;
        vector <int> player_row;

        for(int k=0; k<player_matrix[player_number].size();k++){
            if(player_matrix[player_number][k]!=0 ) {

                if (!(std::count(pla_path.begin(), pla_path.end(), k))) {


                    player_row.push_back(k);
                }
            }
        }



        min_time=player_matrix[player_number][player_row[0]];
        next_player_number=player_row[0];
        for(int n=1; n<player_row.size();n++) {


            if(player_matrix[player_number][player_row[n]]< min_time) {
                min_time = player_matrix[player_number][player_row[n]];
                next_player_number = player_row[n];
            }
        }
        player_row.clear();
        player_number=next_player_number;
    }
    pla_path.push_back(player_number);
    for(float f: pla_path) {
        cout << f << " ";
    }
    cout << "\n";

}

int main(int argc, char** argv) {

    string path = argv[1];
    int s = atoi(argv[2]);

    vector <vector<float>> content;
    vector <float> row;
    string line, word;
    vector  <int> player_path;

    fstream file(path, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();

            stringstream str(line);

            while (getline(str, word, ','))
                row.push_back(std::stof(word));
            content.push_back(row);
        }
    }
    else
        cout << "Could not open the file\n";

    for (int i = 0; i < content.size(); i++) {
        for (int j = 0; j < content[i].size(); j++) {
            if (j != 10)
                cout << content[i][j] << ",";
            else
                cout << content[i][j];
        };
        cout << "\n";
    }

    printingPath(s,content,player_path);
    file.close();
    return 0;
    }









