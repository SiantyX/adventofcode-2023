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



int main() {
    auto start = std::chrono::system_clock::now();

    fstream f;
    f.open("day4_ex.txt", ios::in);
    string row; 
    vector<string> rows;
    vector<string> rows2;
    rows.reserve(2000);
    rows2.reserve(2000);
    auto pRows = &rows;
    auto pRows2 = &rows2;

    int j = 0;
    uint64_t res = 0;
    std::cout << "reading" << endl;
    while (!f.eof()) {
        getline(f, row);
        rows.push_back(row);
        j++;
    }

    auto end = std::chrono::system_clock::now();
    std::cout << "Setup Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    
    std::cout << "working" << endl;

    res += pRows->size();
    cout << "added: " << pRows->size() << endl;
    while (true) {
        if (pRows->size() == 0) {
            break;
        }
        pRows2->clear();
        //pRows2->reserve(2000);
        //cout << pRows << " " << pRows2 << endl;
        for (size_t i = 0; i < pRows->size(); i++) {
            string row = (*pRows)[i];
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
                //cout << "have: '" << have;
                for (auto &win : str_winners) {
                    if (win == have) {
                //        cout << " winner!";
                        wins++;
                        break;
                    }
                }
                //cout << endl;
            }
            //cout << "wins: " << wins << endl;
            for (int j = 1; (j <= wins) && ((i+j) < pRows->size()); j++) {
                //cout << "pushing.. size: " << pRows2->size() << " len of str: " << rows[<< endl;
                //string tmp = (*pRows)[i+j];
                //cout << "created: " << tmp << endl;
                pRows2->push_back((*pRows)[i+j]);
                //cout << "pushed" << endl;
            }

        
            //if (wins != 0) {
            //    wins = pow(2, wins-1);
            //}
            //cout << "win sum: " << wins << endl;
            //res += wins;
        }
        cout << "added: " << pRows2->size() << endl;
        res += pRows2->size();
        swap(pRows, pRows2);
    }

    end = std::chrono::system_clock::now();
    std::cout << "Working Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    std::cout << "RESULT: " << res << endl << endl;

    return 0;
}