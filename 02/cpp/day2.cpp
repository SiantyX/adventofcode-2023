#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <cmath>
#include <iomanip>
#include <chrono>
#include <atomic>
#include <thread>
#include <cstdint>
#include <iomanip>
#include <set>
#include <string_view>
#include <openssl/sha.h>

//#define DEBUG

using namespace std;

struct Game {
    int id;
    int max[3]; //R G B
};

void split(string s, string delim, vector<string> &res) {
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        res.push_back(token);
        s.erase(0, pos + delim.length());
    }
    res.push_back(s);
}

int main() {
    auto start = std::chrono::system_clock::now();

    fstream f;
    f.open("day2.txt", ios::in);
    string row; 
    //vector<string> rows;
    vector<Game> games;

    int j = 0;
    std::cout << "reading" << endl;
    while (!f.eof()) {
        getline(f, row);
        auto pos = row.find("Game ");
        pos = pos + 5;
        auto epos = row.find(":");
        string str_id = row.substr(pos, epos-pos);
        int id = stoi(str_id);
        //row = row.erase(0, pos+2);
        //cout << row << endl;
        //cout << "'" << id << "'" << endl;
        Game new_game{0, 0, 0, 0};
        new_game.id = id;
        pos = epos + 2;
        row = row.substr(pos, row.length() - pos);

        vector<string> sets;
        split(row, "; ", sets);
        //cout << "new game" << endl;
        for (auto &set : sets) {
            vector<string> pulls;
            split(set, ", ", pulls);
            //cout << "new set" << endl;
            int max[3]{0, 0, 0};
            for (auto &pull : pulls) {
                //cout << "new pull" << endl;
                vector<string> num_color;
                split(pull, " ", num_color);
                //cout << "num: " << num_color[0] << " color: " << num_color[1] << endl;
                if (num_color[1] == "red") {
                    max[0] += stoi(num_color[0]);
                } else if (num_color[1] == "green") {
                    max[1] += stoi(num_color[0]);
                } else if (num_color[1] == "blue") {
                    max[2] += stoi(num_color[0]);
                } else {
                    cout << "ERROR" << endl;
                    exit(8);
                }
            }
            for (int i = 0; i < 3; i++) {
                if (max[i] > new_game.max[i]) {
                    new_game.max[i] = max[i];
                }
            }
        }

        games.push_back(new_game);

        j++;
    }

    auto end = std::chrono::system_clock::now();
    cout << "Setup Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    
    cout << "working" << endl;
    uint64_t res = 0;

    //int bagMax[3]{12, 13, 14};
    for(auto &game : games) {
        // PART TWO
        uint64_t tmp = 1;
        for (int i = 0; i < 3; i++) {
            tmp *= game.max[i];
        }
        res += tmp;

        /* PART ONE
        bool possible = true;
        for (int i = 0; i < 3; i++) {
            if (game.max[i] > bagMax[i]) {
                possible = false;
            }
        }
        if (possible) {
            res += game.id;
        }
        */
    }

    end = std::chrono::system_clock::now();
    cout << "Working Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    cout << "RESULT: " << res << endl << endl;

    return 0;
}