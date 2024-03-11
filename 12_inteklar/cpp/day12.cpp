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

struct record {
    string rec;
    int id;
    vector<char> info;
    //string info;
};

unordered_map<uint64_t, uint64_t> arrmap;
#define RETURN_AND_CACHE(_cur_rec, _spos, _ipos, _ret) arrmap[hashfunc(_cur_rec, _spos, _ipos)] = _ret; return _ret

std::hash<string_view> hash_s;
uint64_t hashfunc(record *cur_rec, unsigned int spos, unsigned int ipos) {

    std::string_view sv{cur_rec->rec.c_str() + spos, cur_rec->rec.length() - spos};
    uint64_t h = hash_s(sv);

    for (unsigned int i = ipos; i < cur_rec->info.size(); i++) {
        h = (h + (324723947 + cur_rec->info[i])) ^93485734985;
    }

    //uint64_t h = cur_rec->id + (1101 * (spos+1)) + (121001 * (ipos+1));
    //h = (h + (324723947 + cur_rec->id)) ^93485734985;
    //h = (h + (324723947 + spos)) ^93485734985;
    //h = (h + (324723947 + ipos)) ^93485734985;
    
    //tmp_arg args{cur_rec->id, spos, ipos};
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(cur_rec->info.begin(), cur_rec->info.size() * 4, h);

    return h;
}

const char working = '.';
const char broken = '#';

uint64_t handle(record *cur_rec, unsigned int spos, unsigned int ipos) {
    if (ipos == cur_rec->info.size()) {
        // check no more broken, else not ok
        for (; spos < cur_rec->rec.length(); spos++) {
            if (cur_rec->rec[spos] == broken) {
                return 0;
            }
        }
        return 1;
    }

    auto got = arrmap.find(hashfunc(cur_rec, spos, ipos));
    if (got != arrmap.end()) {
        //cout << got->first << endl;
        //cout << "found arr: " << got->second << endl;
       //Sleep(1000);
        return got->second;
    }

    // count latest start
    int l = cur_rec->rec.length();
    int sum = 0;
    for (unsigned int i = ipos; i < cur_rec->info.size(); i++) {
        sum += cur_rec->info[i];
    }
    sum += cur_rec->info.size() - 1 - ipos;
    unsigned int latest_start = l - sum;

    uint64_t ret = 0;
    for (; spos <= latest_start; spos++) {
        int num_broken = cur_rec->info[ipos];
        // not possible anymore, exit
        if (spos+num_broken > cur_rec->rec.length()) {
            RETURN_AND_CACHE(cur_rec, spos, ipos, ret);
        }

        bool ok_arr = true;
        //string new_rec = cur_rec->rec;
        for (int i = 0; i < num_broken; i++) {
            if (cur_rec->rec[spos+i] == working) {
                ok_arr = false;
                break;
            } else if (cur_rec->rec[spos+i] == broken && latest_start > spos+i) {
                latest_start = spos+i;
            }
        }
        if (ok_arr && spos+num_broken < cur_rec->rec.length()) {
            if (ok_arr && cur_rec->rec[spos+num_broken] == broken) {
                ok_arr = false;
            } 
        }
        if (ok_arr) {
            ret += handle(cur_rec, spos + num_broken + 1, ipos + 1);
        }
    }

    RETURN_AND_CACHE(cur_rec, spos, ipos, ret);
}

int main() {
    auto start = std::chrono::system_clock::now();

    fstream f;
    f.open("day12.txt", ios::in);
    string row; 
    vector<record*> records;

    int j = 0;
    std::cout << "reading" << endl;
    while (!f.eof()) {
        j++;
        getline(f, row);
        auto pos = row.find(' ');
        string rec = row.substr(0, pos);
        row.erase(0, pos + 1);
        record* tmprec = new record(); 
        tmprec->rec = rec;

        pos = row.find(',');
        while (pos != std::string::npos) {
            int i = stoi(row.substr(0, pos));
            tmprec->info.push_back(i);
            row.erase(0, pos + 1);
            pos = row.find(',');
        }
        int i = stoi(row);
        tmprec->id = j;
        tmprec->info.push_back(i);
        
        // 5 times itself
        tmprec->rec.append("?");
        tmprec->rec.append(rec);
        tmprec->rec.append("?");
        tmprec->rec.append(rec);
        tmprec->rec.append("?");
        tmprec->rec.append(rec);
        tmprec->rec.append("?");
        tmprec->rec.append(rec);

        size_t initlen = tmprec->info.size();
        for (int k = 0; k < 4; k++) {
            for (unsigned int i = 0; i < initlen; i++) {
                tmprec->info.push_back(tmprec->info[i]);
            }
        }

        records.push_back(tmprec);
    }

    auto end = std::chrono::system_clock::now();
    
    cout << "working" << endl;
    uint64_t arr = 0;
    stringstream ss_rec;
    stringstream ss_inf;
    int index = 0;
    start = std::chrono::system_clock::now();
    #ifdef DEBUG
    records = {records.begin()+794-1, records.begin()+794};
    #endif
    for (auto &cur_rec : records) {
        index++;
        ss_rec.str("");
        ss_inf.str("");
        ss_rec << cur_rec->rec;
        for (unsigned int i = 0; i < cur_rec->info.size(); i++) {
            if (cur_rec->info.size()-1 == i) {
                ss_inf << cur_rec->info[i];
            } else {
                ss_inf << cur_rec->info[i] << ",";
            }
        }

        cout << left << setw(4) << index << ": ";
        cout << right << setw(50) << ss_rec.str();
        cout << " | ";
        cout << left << setw(50) << ss_inf.str();
        cout << " ARRANGEMENTS: ";

        uint64_t t = handle(cur_rec, 0, 0);

        arr += t;

        cout << t << endl;
    }
    end = std::chrono::system_clock::now();
    cout << "Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 

    cout << "RESULT: " << arr << endl << endl;

    return 0;
}