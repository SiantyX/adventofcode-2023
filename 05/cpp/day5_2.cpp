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

long long num_seeds;
long long seeds_per_t;

const int num_ways = 7;
int cway = -1;
vector<string> rows[7];
vector<array<long long, 3>> ways[7];
vector<array<long long, 2>> tseeds;
vector<long long> seeds; //(1753244663);

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

    long long pos;
    long long j{0};
    long long min{LLONG_MAX};
    int id;
    char z;

private:
    std::atomic<bool> has_work{false};

    std::atomic<bool> exiting{false};
    std::atomic<bool> thread_started{false};

    void iter() {
        //long long tmp = seeds_per_t;

        for (long long j = pos; j < seeds_per_t * (id + 1); j++) {
            auto seed = seeds[j];
            for (int i = 0; i < num_ways; i++) {
                for (auto &path : ways[i]) {
                    if (seed >= path[0] && seed < path[2]) {
                        seed = path[1] + (seed - path[0]);
                        //cout << seed << " ";
                        break;
                    }
                }
                //cout << seed << " ";
            }
            //cout << endl << tseed << " to " << seed << endl << endl;
            if (seed < min) {
                min = seed;
            }
            //answers.push_back(seed);
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
    auto start = std::chrono::system_clock::now();

    fstream f;
    f.open("day5.txt", ios::in);
    string row, seedstr; 

    //seeds.reserve(1753244663);


    getline(f, seedstr);
    getline(f, row); // trash line

    std::cout << "reading" << endl;
    while (!f.eof()) {
        getline(f, row);
        if (row[0] >= 'a' && row[0] <= 'z') {
            cway++;
        } else if (row[0] >= '0' && row[0] <= '9') {
            rows[cway].push_back(row);
        } 
    }

    std::cout << "setup seeds" << endl;
    std::cout << seedstr << endl;
    size_t pos = 0;
    seedstr.erase(0, 7); // "seeds: "
    pos = seedstr.find(' ');
    while (pos != std::string::npos) {
        long long seed = stoll(seedstr.substr(0, pos));
        //seeds.push_back(stoll(seed));
        //std::cout << seed << " ";
        seedstr.erase(0, pos + 1);
        pos = seedstr.find(' ');
        long long seed_range = stoll(seedstr.substr(0, pos));
        seedstr.erase(0, pos + 1);
        tseeds.push_back({seed, seed_range});
        pos = seedstr.find(' ');
    }

    std::cout << "unrolling" << endl;
    for (auto &s : tseeds) {
        for(long long i = 0; i < s[1]; i++) {
            seeds.push_back(s[0] + i);
        }
    }
    
    std::cout << "=====" << endl;
    std::cout << "setup ways" << endl;

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
            range = src + range;
            ways[i].push_back({src, dest, range});
            
            //cout << "dest: " << dest << " src: " << src << " range: " << range << endl;
        }
    }

    for (int i = 0; i < num_ways; i++) {
        std::cout << endl << "cway: " << i << endl;
        for (auto &way : ways[i]) {
            std::cout << "src: " << way[0] << " dest: " << way[1] << " range: " << way[2] << endl;
        }
    }

    std::cout << endl;

    std::vector<long long> answers;

    num_seeds = seeds.size();
    std::cout << "num seeds: " << num_seeds << endl;

    // THREADS
    const int num_workers = 24;
    seeds_per_t = num_seeds / num_workers;
    eager_worker* workers = new eager_worker[num_workers];

    auto now = std::chrono::system_clock::now();
    auto elapsed = now - start;
    std::cout << " elapsed setup in main: " << std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() << " seconds" << endl;

    std::cout << "start" << endl;
    start = std::chrono::system_clock::now();

    for (int i = 0; i < num_workers; i++) {
        workers[i].pos = num_seeds / 24 * i;
        workers[i].id = i;
        workers[i].work();
    }

    bool working = true;
    while (working) {
        working = false;
        for (int i = 0; i < num_workers; i++) {
            if (workers[i].is_working()) {
                working = true;
            }
        }
    }

    now = std::chrono::system_clock::now();
    elapsed = now - start;
    std::cout << " elapsed calc in threads: " << std::chrono::duration_cast<std::chrono::seconds>(elapsed).count() << " seconds" << endl;


    long long min = LLONG_MAX;
    for (int i = 0; i < num_workers; i++) {
        if (workers[i].min < min) {
            min = workers[i].min;
        }
    }

    std::cout << endl << "MIN: " << min << endl;

    return 0;
}
