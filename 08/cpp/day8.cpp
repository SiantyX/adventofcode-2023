#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <windows.h>
#include <cmath>
#include <iomanip>
#include <chrono>

using namespace std;

int main() {
    fstream f;
    f.open("day8.txt", ios::in);
    string row, map;
    vector<string> rows;
    vector<string> poses;
    unordered_map<string, array<string, 2>> instr; 

    getline(f, map);
    getline(f, row); // empty trash
    while (!f.eof()) {
        getline(f, row);
        rows.push_back(row);
    }
    for (auto &r : rows) {
        string a = r.substr(0,3);
        string b = r.substr(7,3);
        string c = r.substr(12,3);
        instr[a] = {b, c};
        if (a[2] == 'A') {
            poses.push_back(a);
        }

        cout << "Value for " << a << ": " << instr[a][0] << ", " << instr[a][1] << std::endl;
    }

    long long j = 0;
    cout << setprecision(6) << fixed;
    auto start = std::chrono::system_clock::now();
    while (true) {
        if ((j % 10000000 == 0) && (j > 0)) {
            cout << "1 mil" << endl;
            auto now = std::chrono::system_clock::now();
            auto elapsed = now - start;
            auto perc = j / 21366921060721.0;
            auto ant = 21366921060721.0 / j;
            cout << "percentage: " << 100 * perc << "%" << " elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() << " seconds" << endl;
            cout << "Time remaining: " << std::chrono::duration_cast<std::chrono::hours>(elapsed * ant).count()/24 << " days" << endl;
        }

        if (j % map.length() == 0) {
            int x = 0;
            for (auto &pos : poses) {
                if (pos[2] == 'Z') {
                    x++;
                }
            }
            if (x == poses.size()) {
                break;
            }
        }

        for (int i = 0; i < poses.size(); i++) {
            if (map[j % map.length()] == 'L') {
                poses[i] = instr[poses[i]][0];
            } else if (map[j % map.length()] == 'R') {
                poses[i] = instr[poses[i]][1];
            } else {
                cout << poses[i] << " error" << endl;
                exit(4);
            }
        }
        j++;
    }

    cout << j << endl;

    return 0;
}