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
#include <algorithm>

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

int64_t godown(vector<int64_t> &ys) {
    /*
    for (size_t i = 1; i < ys.size(); i++) {
        cout << ys[i] << " ";
    }
    cout << endl;
    */

    vector<int64_t> ks;
    bool done = true;
    for (size_t i = 1; i < ys.size(); i++) {
        int64_t tmp = ys[i] - ys[i-1];
        if (tmp != 0) {
            done = false;
        }
        ks.push_back(tmp);
    }
    /*
    for (size_t i = 1; i < ks.size(); i++) {
        cout << ks[i] << " ";
    }
    cout << endl;
    */

    if (!done) {
        int64_t toadd = godown(ks);
        //cout << "toadd: " << toadd << endl;
        return ys[0] - toadd;
    } else {
        //cout << "done, reversing" << endl;
        return ys[0];
    }
}

int main() {
    auto start = std::chrono::system_clock::now();

    fstream f;
    f.open("day9.txt", ios::in);
    string row; 

    std::cout << "reading" << endl;

    uint64_t res = 0;
    while (!f.eof()) {
        getline(f, row);
        vector<string> tmp;
        split(row, " ", tmp);
        vector<int64_t> nums;
        for (auto &t : tmp) {
            nums.push_back(stoll(t));
        }
        int64_t r = godown(nums);
        cout << "r: " << r << endl;
        res += r;
    }

    //auto end = std::chrono::system_clock::now();
    //std::cout << "Setup Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    
    //std::cout << "working" << endl;
    


    auto end = std::chrono::system_clock::now();
    std::cout << "Working Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    std::cout << "RESULT: " << res << endl << endl;
    // 250764353 too low??

    return 0;
}