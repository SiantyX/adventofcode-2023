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
#include <algorithm> 

//#define DEBUG

using namespace std;

struct num_pos {
    int num;
    int pos;
};

const string str_numbers[] = {
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

int str2int(const char* str, int len)
{
    int i;
    int ret = 0;
    for(i = 0; i < len; ++i)
    {
        ret = ret * 10 + (str[i] - '0');
    }
    return ret;
}

void findFirstANum(string &row, num_pos &res, bool rev = false) {
    int min_pos = INT_MAX;
    int max_pos = 0;
    int latest_i = 0;
    for (unsigned int i = 0; i < 9; i++) {
        size_t pos;
        if (rev) {
            pos = row.rfind(str_numbers[i]);
        } else {
            pos = row.find(str_numbers[i]);
        }

        if (!rev && pos != string::npos && pos < min_pos) {
            min_pos = pos;
            latest_i = i;
        } else if (rev && pos != string::npos && pos > max_pos) {
            max_pos = pos;
            latest_i = i;
        }
    }
    res.num = latest_i+1;
    if (rev) {
        res.pos = max_pos;
    } else {
        res.pos = min_pos;
    }
}

void findFirstDNum(string &row, num_pos &res, bool rev = false) {
    char tmpnum[1];
    char *pEnd;
    string tmpstr = row;
    if (rev) {
        reverse(tmpstr.begin(), tmpstr.end()); 
    }
    bool found = false;
    int i = 0;
    for (;i < tmpstr.length(); i++) {
        
        if (tmpstr[i] >= '0' && tmpstr[i] <= '9') {
            tmpnum[0] = tmpstr[i];
            found = true;
            break;
        }
    }

    if (!found) {
        if (rev) {
            res.pos = INT_MIN;
        } else {
            res.pos = INT_MAX;
        }
        return;
    }

    if (rev) {
        i = tmpstr.length() - i - 1;
    }

    //res.num = strtol(tmpnum, &pEnd, 10);
    res.num = str2int(tmpnum, 1);
    res.pos = i;
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
    num_pos alph;
    num_pos dec;
    int tmpnum = 0;
    for (auto &r : rows) {
        std::cout << r << endl;

        findFirstANum(r, alph);
        findFirstDNum(r, dec);
        std::cout << "pos alph: " << alph.pos << " dec: " << dec.pos << endl;
        if (alph.pos < dec.pos) {
            tmpnum = alph.num * 10;
        } else {
            tmpnum = dec.num * 10;
        }
        std::cout << tmpnum << endl;

        findFirstANum(r, alph, true);
        findFirstDNum(r, dec, true);
        std::cout << "reversed pos alph: " << alph.pos << " dec: " << dec.pos << endl;
        if (alph.pos > dec.pos) {
            tmpnum += alph.num;
        } else {
            tmpnum += dec.num;
        }
        std::cout << tmpnum << endl;

        //std::cout << tmp << endl;
        if(tmpnum % 10 == 0) {
            exit(8);
        }   

        res += tmpnum;
    }

    end = std::chrono::system_clock::now();
    std::cout << "Working Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    std::cout << "RESULT: " << res << endl << endl;

    return 0;
}