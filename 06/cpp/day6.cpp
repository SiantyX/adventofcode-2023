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
#include <cassert>

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
    f.open("day6.txt", ios::in);
    string row; 
    vector<string> rows;

    string tmpstr = "";
    stringstream ss;
    std::cout << "reading" << endl;

    getline(f, row);
    auto epos = row.find(": ");
    row.erase(0, epos+1);
    cout << "'" << row << "'" << endl;
    vector<string> str_times;
    vector<uint64_t> times;
    split(row, " ", str_times);
    // MERGE FOR PART2
    ss.str("");
    for (auto &s : str_times) {
        ss << s;
    }
    str_times.clear();
    str_times.push_back(ss.str());
    //========
    for (auto &t : str_times) {
        uint64_t tmp = stoll(t);
        times.push_back(tmp);
        cout << tmp << endl;
    }

    getline(f, row);
    epos = row.find(": ");
    row.erase(0, epos+1);
    cout << "'" << row << "'" << endl;
    vector<string> str_distances;
    vector<uint64_t> distances;
    split(row, " ", str_distances);
    // MERGE FOR PART2
    ss.str("");
    for (auto &s : str_distances) {
        ss << s;
    }
    str_distances.clear();
    str_distances.push_back(ss.str());
    //========
    for (auto &d : str_distances) {
        uint64_t tmp = stoll(d);
        distances.push_back(tmp);
        cout << tmp << endl;
    }

    assert(times.size() == distances.size());

    auto end = std::chrono::system_clock::now();
    std::cout << "Setup Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    
    std::cout << "working" << endl;
    uint64_t res = 1;

    for (size_t i = 0; i < times.size(); i++) {
        uint64_t time = times[i];
        uint64_t dist = distances[i];

        uint64_t min_hold = 0;
        for (size_t j = 1; j < time; j++) {
            size_t try_dist = j*(time-j);
            if (try_dist > dist) {
                min_hold = j;
                break;
            }
        }

        uint64_t max_hold = time;
        for (size_t j = time - 1; j > 0; j--) {
            size_t try_dist = j*(time-j);
            if (try_dist > dist) {
                max_hold = j;
                break;
            }
        }

        uint64_t ways = max_hold - min_hold + 1;
        cout << "max hold: " << max_hold << " min hold: " << min_hold << " ways: " << ways << endl;
        res = res * ways;
    }

    end = std::chrono::system_clock::now();
    std::cout << "Working Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << " milliseconds" << endl; 
    std::cout << "RESULT: " << res << endl << endl;

    return 0;
}