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

void split(string s, string delim, vector<string> &res) {
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        if (token != delim && pos > 0) {
            res.push_back(token);
        }
        s.erase(0, pos + delim.length());
    }
    res.push_back(s);
}

vector<vector<int>> refs;

int handle(vector<int> &ref) {
    int sum = 0;

    if (ref.size() == 0) {
        return 0;
    }

    sum += ref.size();
    for (int i = 0; i < ref.size(); i++) {
        sum += handle(refs[ref[i]]);
    }
    return sum;
}

int main() {
    auto start = std::chrono::system_clock::now();

    fstream f;
    f.open("day4.txt", ios::in);
    string row; 
    //vector<string> rows;
    //vector<string> rows2;
    //rows.reserve(2000);
    //rows2.reserve(2000);
    //auto pRows = &rows;
    //auto pRows2 = &rows2;

    int j = 0;
    uint64_t res = 0;
    std::cout << "reading" << endl;
    while (!f.eof()) {
        getline(f, row);
        cout << row << endl;

        auto epos = row.find(": ");
        int id = stoi(row.substr(5, epos-5)) - 1;
        epos = epos + 2;
        row = row.substr(epos, row.length() - epos);

        vector<string> sides;
        split(row, " | ", sides);
        vector<string> str_winners;
        vector<string> str_haves;
        split(sides[0], " ", str_winners);
        split(sides[1], " ", str_haves);
        int wins = 0;
        for (auto &have : str_haves) {
            for (auto &win : str_winners) {
                if (win == have) {
                    wins++;
                    break;
                }
            }
        }
        cout << "wins: " << wins << endl;
        if (id != j) {
            cout << "NOT EQUAL" << endl;
        }
        vector<int> copies;
        for (int j = id+1; j <= (id+wins); j++) {
            copies.push_back(j);
        }
        refs.push_back(copies);
        
        j++;
    }

    auto end = std::chrono::system_clock::now();
    std::cout << "Setup Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    
    std::cout << "working" << endl;

    res += refs.size();
    for (auto &ref : refs) {
        res += handle(ref);
    }



    end = std::chrono::system_clock::now();
    std::cout << "Working Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    std::cout << "RESULT: " << res << endl << endl;

    return 0;
}