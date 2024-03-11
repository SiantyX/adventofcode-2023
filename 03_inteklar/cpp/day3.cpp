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
        res.push_back(token);
        s.erase(0, pos + delim.length());
    }
    res.push_back(s);
}

struct Part {
    int num;
    int spos;
    int epos;
    int rownum;
};

int main() {
    auto start = std::chrono::system_clock::now();

    fstream f;
    f.open("day3.txt", ios::in);
    string row; 
    vector<string> rows;
    vector<Part> parts;

    int j = 0;
    std::cout << "reading" << endl;
    while (!f.eof()) {
        getline(f, row);
        rows.push_back(row);
        std::cout << row << endl;
        size_t spos = 0;
        size_t str_pos = 0;

        while (spos < row.length()) {
            try {
                spos = 0;
                int part_num = stoi(row, &spos, 10);
                //std::cout << "spos: " << spos << " partnum: " << part_num << endl;
                row.erase(0, spos);
                Part part;
                part.num = part_num;
                part.spos = str_pos + 0;
                part.epos = str_pos + spos - 1;
                part.rownum = j;
                parts.push_back(part);
                str_pos += spos;
            } catch (std::invalid_argument) {
                row.erase(0, 1);
                str_pos += 1;
            }
        }

        j++;
    }

    auto end = std::chrono::system_clock::now();
    std::cout << "Setup Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    
    std::cout << "working" << endl;
    uint64_t res = 0;
    uint64_t tmpsum = 0;
    const int num_adj = 1;
    const string no_match = ".0123456789";
    const string test = ".";
    for(Part &part : parts) {
        std::cout << endl << endl << "Num: " << part.num << " spos: " << part.spos << " epos: " << part.epos << " rownum: " << part.rownum << endl;
        bool done = false;

        for (int i = -num_adj; i <= num_adj && !done; i++) {
            if ((part.rownum + i) < 0 || (part.rownum + i) > (rows.size()-1)) continue; // out of range
            std::cout << rows[part.rownum + i] << endl;
            
            int spos = part.spos - 1;
            int epos = part.epos + 1;
            if (spos < 0) spos = 0;
            if (epos > (rows[part.rownum + i].length() - 1)) epos = rows[part.rownum + i].length() - 1;
            for (int j = spos; j <= epos && !done; j++) {
                char c = rows[part.rownum + i][j];
                size_t pos = no_match.find(c);
                if (pos == string::npos) {
                    res += part.num;
                    std::cout << "adj: " << part.num << " res: " << res << endl;
                    if (part.rownum == (140 - 1)) {
                        int a = 0; // for break
                    }
                    done = true;
                } else if (test.find(c) == string::npos && i != 0) {
                    cout << "adj to another number, hmm" << endl;
                }   
            }
        }
        if (!done) {
            std::cout << "NOT FOUND" << endl;     
            tmpsum += part.num;
        }
    }

    end = std::chrono::system_clock::now();
    std::cout << "Working Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    std::cout << "RESULT: " << res << endl << endl;
    std::cout << "Not found sum: " << tmpsum << endl << endl;

    // 534482 too low
    // 600298 too high (alla partnums)

    return 0;
}