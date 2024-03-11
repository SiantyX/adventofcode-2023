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

int instr[20000][2];
string map;
int maplen;

/*
const int getPos(const int& pos, const char& way) noexcept {
    if (way == 'L') {
        return instr[pos][0];
    } else {
        return instr[pos][1];
    }
}
*/

struct eager_worker {
    eager_worker() = default;
    inline ~eager_worker() { stop_thread(); }
    inline void stop_thread() {
        exiting.store(true);
        has_work.store(true);
        if (thread.joinable()) {
            thread.join();
        }
    }

    inline void work() {
        has_work.store(true);
    }

    inline void finish() {
        while (has_work.load()) {
        }
    }

    inline bool is_working() {
        return has_work.load();
    }

    int pos;
    long long j{0};
    int id;
    char z;

private:
    std::atomic<bool> has_work{false};

    std::atomic<bool> exiting{false};
    std::atomic<bool> thread_started{false};

    void iter() {
        while (true) {
            for (int i = 0; i < maplen; i++) {
                if (map[i] == 'L') {
                    pos = instr[pos][0];
                } else {
                    pos = instr[pos][1];
                }
            }
            j = j + maplen;

            z = pos / 730;

            if (z == ('Z' - 'A') + 1) {
                return;
            }
        }
    }

    std::thread thread = std::thread([this] {
        thread_started.store(true);
        while (true) {
            while (!has_work.load()) {
                if (exiting.load()) {
                    return;
                }
            }
            iter();
            has_work.store(false);
        }
    });
};

int main() {
    fstream f;
    f.open("day8.txt", ios::in);
    string row; 

    vector<string> rows;
    vector<int> poses;

    getline(f, map);
    maplen = map.length();
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
        constexpr int y = 'Z' - 'A';
        instr[(a[0] - 'A' + 1) + ((a[1] - 'A' + 1) * 27) + ((a[2] - 'A' + 1) * 730)][0] = (b[0] - 'A' + 1) + ((b[1] - 'A' + 1) * 27) + ((b[2] - 'A' + 1) * 730);
        instr[(a[0] - 'A' + 1) + ((a[1] - 'A' + 1) * 27) + ((a[2] - 'A' + 1) * 730)][1] = (c[0] - 'A' + 1) + ((c[1] - 'A' + 1) * 27) + ((c[2] - 'A' + 1) * 730);
        if (a[2] == 'A') {
            poses.push_back((a[0] - 'A' + 1) + ((a[1] - 'A' + 1) * 27) + ((a[2] - 'A' + 1) * 730));
        }
    }

    // THREADS
    eager_worker* workers = new eager_worker[poses.size()];
    for (int i = 0; i < poses.size(); i++) {
        workers[i].pos = poses[i];
        workers[i].id = i;
    }

    long long max = 1;
    bool working;
    int done;

    cout << setprecision(6) << fixed;
    auto start = std::chrono::system_clock::now();

    int loopc = 0;

    cout << "start" << endl;

    while (true) {

        done = 0;
        for (int i = 0; i < poses.size(); i++) {
            if (!workers[i].is_working()) {
                if (workers[i].j >= max) {
                    max = workers[i].j;
                    done++;
                } else {
                    workers[i].work();
                }
            }
        }

        // STATS
        loopc++;
        if (loopc % 10000000 == 0) {
            auto now = std::chrono::system_clock::now();
            auto elapsed = now - start;
            auto perc = max / 21366921060721.0;
            auto ant = 21366921060721.0 / max;
            cout << "max: " << max << " percentage: " << 100 * perc << "%" << " elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() << " seconds" << endl;
            cout << "Time remaining: " << std::chrono::duration_cast<std::chrono::minutes>(elapsed * ant).count() << " minutes" << endl;
        }
        if (done == poses.size()) {
            break;
        }
    }

    // ==========
    cout << workers[0].j << endl;

    return 0;
}
