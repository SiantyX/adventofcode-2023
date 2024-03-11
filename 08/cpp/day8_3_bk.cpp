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
    f.open("day8.txt", ios::in);
    string row, map;
    vector<string> rows;
    vector<int> poses;
    //vector<char>  poses_val;
    //char instr[26][26][26][6];
    int instr[20000][2];

    getline(f, map);
    getline(f, row); // empty trash
    while (!f.eof()) {
        getline(f, row);
        rows.push_back(row);
    }
    cout << "setup" << endl;
    for (auto &r : rows) {
        string a = r.substr(0,3);
        string b = r.substr(7,3);
        string c = r.substr(12,3);

        instr[(a[0] - 'A' + 1) + ((a[1] - 'A' + 1) * 27) + ((a[2] - 'A' + 1) * 702)][0] = (b[0] - 'A' + 1) + ((b[1] - 'A' + 1) * 27) + ((b[2] - 'A' + 1) * 702);
        instr[(a[0] - 'A' + 1) + ((a[1] - 'A' + 1) * 27) + ((a[2] - 'A' + 1) * 702)][1] = (c[0] - 'A' + 1) + ((c[1] - 'A' + 1) * 27) + ((c[2] - 'A' + 1) * 702);
        /*
        for (int i = 0; i < 3; i++) {
            instr[a[0] - 'A'][a[1] - 'A'][a[2] - 'A'][i] = b[i] - 'A';
        }
        for (int i = 3; i < 6; i++) {
            instr[a[0] - 'A'][a[1] - 'A'][a[2] - 'A'][i] = c[i-3] - 'A';
        }

        //strcpy(instr[a[0] - 'A'][a[1] - 'A'][a[2] - 'A'][0], b.c_str());
        //strcpy(instr[a[0] - 'A'][a[1] - 'A'][a[2] - 'A'][1], c.c_str());
        */
        if (a[2] == 'A') {
            //poses.push_back(k);
            poses.push_back((a[0] - 'A' + 1) + ((a[1] - 'A' + 1) * 27) + ((a[2] - 'A' + 1) * 54));
            //poses_val.push_back(a[2] - 'A');
        }
        /*
        cout << "Value for " << a << ": ";
        for (int i = 0; i < 3; i++) {
            cout << (int)instr[a[0] - 'A'][a[1] - 'A'][a[2] - 'A'][i] << " ";
        }
        cout << ", ";
        for (int i = 3; i < 6; i++) {
            cout << (int)instr[a[0] - 'A'][a[1] - 'A'][a[2] - 'A'][i] << " ";
        }
        cout << endl;
        */
    }

    // THREADS


    // ==========

    cout << "start" << endl;
    long long j = 0;
    cout << setprecision(6) << fixed;
    auto start = std::chrono::system_clock::now();
    long loopc = 0;
    int maplen = map.length();
    int posessize = poses.size();
    while (true) {
        if (loopc % 1000000 == 0) {
            cout << "1 mil" << endl;
            auto now = std::chrono::system_clock::now();
            auto elapsed = now - start;
            auto perc = j / 21366921060721.0;
            auto ant = 21366921060721.0 / j;
            cout << "percentage: " << 100 * perc << "%" << " elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() << " seconds" << endl;
            cout << "Time remaining: " << std::chrono::duration_cast<std::chrono::minutes>(elapsed * ant).count() << " minutes" << endl;
        }

        int x = 0;
        for (int i = 0; i < posessize; i++) {
            //char z = ((char) poses[i]) & 0x1F;
            char z = poses[i] / 702;
            if (z == ('Z' - 'A')) {
                x++;
            }
        }
        
        if (x == posessize) {
            break;
        }

        for (int k = 0; k < maplen; k++) {
            for (int i = 0; i < posessize; i++) {
                if (map[k] == 'L') {
                    poses[i] = instr[poses[i]][0];
                    //cout << poses[i][0][0];
                    //poses_val[i] = poses[i][2];
                    //poses[i] = instr[poses[i][0]][poses[i][1]][poses[i][2]];
                    //poses[i] = instr[poses[i]][0];
                } else if (map[k] == 'R') {
                    poses[i] = instr[poses[i]][1];
                    //poses[i] = instr[poses[i]][1];
                    //poses_val[i] = poses[i][5];
                    //poses[i] = instr[poses[i][3]][poses[i][4]][poses[i][5]];
                } else {
                    //cout << poses[i] << " error" << endl;
                    exit(4);
                }
            }
        }
        j = j + maplen;
        loopc++;
    }

    cout << j << endl;

    return 0;
}
