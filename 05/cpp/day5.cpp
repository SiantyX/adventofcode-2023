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

using namespace std;

int main() {
    fstream f;
    f.open("day5.txt", ios::in);
    string row, seedstr; 

    const int num_ways = 7;
    int cway = -1;
    vector<string> rows[7];
    vector<array<long long, 3>> ways[7];
    vector<long long> seeds;

    getline(f, seedstr);
    getline(f, row); // trash line

    cout << "reading" << endl;
    while (!f.eof()) {
        getline(f, row);
        if (row[0] >= 'a' && row[0] <= 'z') {
            cway++;
        } else if (row[0] >= '0' && row[0] <= '9') {
            rows[cway].push_back(row);
        } 
    }

    cout << "setup" << endl;
    cout << seedstr << endl;
    size_t pos = 0;
    seedstr.erase(0, 7); // "seeds: "
    while ((pos = seedstr.find(' ')) != std::string::npos) {
        string seed = seedstr.substr(0, pos);
        seeds.push_back(stoll(seed));
        std::cout << seed << " ";
        seedstr.erase(0, pos + 1);
    }
    seeds.push_back(stoll(seedstr));
    std::cout << seedstr << std::endl;

    cout << "=====" << endl;

    for (int i = 0; i < num_ways; i++) {
        //cout << "cway: " << i << endl;
        for (auto &r : rows[i]) {
            //cout << r << endl;

            pos = r.find(' ');
            long long dest = stoll(r.substr(0, pos));
            r.erase(0, pos + 1);
            pos = r.find(' ');
            long long src = stoll(r.substr(0, pos));
            r.erase(0, pos + 1);
            long long range = stoll(r);
            ways[i].push_back({src, dest, range});
            
            //cout << "dest: " << dest << " src: " << src << " range: " << range << endl;
        }
    }

    for (int i = 0; i < num_ways; i++) {
        cout << endl << "cway: " << i << endl;
        for (auto &way : ways[i]) {
            cout << "src: " << way[0] << " dest: " << way[1] << " range: " << way[2] << endl;
        }
    }

    cout << endl;

    std::vector<long long> answers;

    for (auto &tseed : seeds) {
        auto seed = tseed;
        for (int i = 0; i < num_ways; i++) {
            for (auto &path : ways[i]) {
                if (seed >= path[0] && seed < (path[0] + path[2])) {
                    seed = path[1] + (seed - path[0]);
                    //cout << seed << " ";
                    break;
                }
            }
            //cout << seed << " ";
        }
        //cout << endl << tseed << " to " << seed << endl << endl;
        answers.push_back(seed);
    }

    long long min = LLONG_MAX;
    for (auto &answer : answers) {
        cout << answer << " ";
        if (answer < min) {
            min = answer;
        }
    }

    cout << endl << "MIN: " << min << endl;

    return 0;
}
