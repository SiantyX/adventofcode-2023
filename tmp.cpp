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
    f.open("day1.txt", ios::in);
    string row; 
    vector<string> rows;

    int j = 0;
    std::cout << "reading" << endl;
    while (!f.eof()) {
        getline(f, row);
        rows.push_back(row);
        j++;
    }

    auto end = std::chrono::system_clock::now();
    std::cout << "Setup Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    
    std::cout << "working" << endl;
    uint64_t res = 0;

    end = std::chrono::system_clock::now();
    std::cout << "Working Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    std::cout << "RESULT: " << res << endl << endl;

    return 0;
}