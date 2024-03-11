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

enum class Hand {nothing, pair, twopair, threekind, house, fourkind, fivekind};
unordered_map<char, int> cardnum = {
    {'T', 10},
    {'J', 1}, // 11
    {'Q', 12},
    {'K', 13},
    {'A', 14},
};

struct Game {
    int strength;
    int cards[5];
    int bid;
    int rank;
    int sort_cards[5];
    int num_equal[5];
};

int parseCard(char &c) {
    auto got = cardnum.find(c);
    if (got != cardnum.end()) {
        return got->second;
    }
    return c - 48;
}

Hand set_if_bigger(Hand &have, Hand found) {
    if (found > have) {
        return found;
    }
    return have;
}

bool cardEqual(int card1, int card2) {
    if (card1 == card2 || card1 == 1 || card2 == 1) {
        return true;
    }
    return false;
}

Hand parseHand(Game &g) {
    auto tcards = g.cards;
    auto num_equal = g.num_equal; //[5]{0,0,0,0,0};
    Hand stat = Hand::nothing;

    // create new cards array with jokers at the end for easier algo
    int cards[5];
    int num_jokers = 0;
    int count = 0;
    int fpos = 0;
    int tpos = 0;
    while (fpos < 5) {
        if (tcards[fpos] != 1) {
            cards[tpos] = tcards[fpos];
            fpos++;
            tpos++;
        } else {
            num_jokers++;
            fpos++;
            cards[5-num_jokers] = 1;
        }
    }
    g.sort_cards[0] = cards[0];
    g.sort_cards[1] = cards[1];
    g.sort_cards[2] = cards[2];
    g.sort_cards[3] = cards[3];
    g.sort_cards[4] = cards[4];

    for (int i = 0; i < 5; i++) {
        num_equal[i] = 1;
        bool already_counted = false;
        for (int j = i - 1; j >= 0; j--) {
            if (j >= 0 && cards[i] == cards[j]) {
                already_counted = true; // dont count same card again
                break;
            }
        }
        if (already_counted) continue;
        if ((cards[0]+cards[1]+cards[2]+cards[3]+cards[4]) == 5) return Hand::fivekind; // highest, all jokers
        if (cards[i] == 1) break; // dont check what joker matches with

        for (int j = i + 1; j < 5; j++) {
            if (cardEqual(cards[i], cards[j])) {
                num_equal[i]++;
            }
        }

        if (num_equal[i] == 5) {
            return Hand::fivekind;
        } else if (num_equal[i] == 4) {
            return Hand::fourkind;
        }
    }

    for (int i = 0; i < 5; i++) {
        if (num_equal[i] == 3) {
            if (stat == Hand::threekind) {
                stat = set_if_bigger(stat, Hand::house); // special for joker
            } else if (stat == Hand::pair) {
                stat = set_if_bigger(stat, Hand::house);
            } else {
                stat = set_if_bigger(stat, Hand::threekind);
            }
        } else if (num_equal[i] == 2) {
            if (stat == Hand::pair) {
                stat = set_if_bigger(stat, Hand::twopair);
            } else if (stat == Hand::threekind) {
                stat = set_if_bigger(stat, Hand::house);
            } else {
                stat = set_if_bigger(stat, Hand::pair);
            }
        }
    }

    if (stat == Hand::twopair) {
        if (num_jokers == 1) {
            stat = Hand::pair;
        }
    }
    if (stat == Hand::house) {
        if (num_jokers == 1) {
            // if two at num_equal == 3 then it is house, otherwise no threekind
            int tmp = 0;
            for (int i = 0; i < 5; i++) {
                if (num_equal[i] == 3) {
                    tmp++;
                }
            }
            if (tmp < 2) {
                stat = Hand::threekind;
            }
        } else if (num_jokers == 2) {
            stat = Hand::threekind;
        }
    }

    return stat;
}

bool compare_hand(Game &g1, Game &g2) {
    int *p1 = &g1.strength;
    int *p2 = &g2.strength;
    for (int i = 0; i < 6; i++) {
        if (*p1 > *p2) {
            return true;
        } else if (*p2 > *p1) {
            return false;
        }

        p1++;
        p2++;
    }
    return false;
}

int main() {
    auto start = std::chrono::system_clock::now();

    fstream f;
    f.open("day7.txt", ios::in);
    string row; 

    std::cout << "reading" << endl;
    vector<Game> games;

    while (!f.eof()) {
        getline(f, row);
        Game g;
        cout << "Hand: ";
        for (int i = 0; i < 5; i++) {
            g.cards[i] = parseCard(row[i]);
            cout << setw(2) << g.cards[i] << " ";
        }
        g.strength = (int) parseHand(g);
        g.bid = stoll(row.substr(5,row.length() - 5));
        cout << "Strength: " << (int) g.strength << " bid: " << g.bid << endl;
        games.push_back(g);
    }

    auto end = std::chrono::system_clock::now();
    std::cout << "Setup Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    
    std::cout << "working" << endl;
    uint64_t res = 0;

    sort(games.begin(), games.end(), compare_hand);

    cout << endl << "After sort" << endl;
    for (size_t i = 0; i < games.size(); i++) {
        games[i].rank = games.size() - i;
        cout << "Hand: ";
        for (int j = 0; j < 5; j++) {
            cout << setw(2) << games[i].cards[j] << " ";
        }
        cout << "Sorted hand: ";
        for (int j = 0; j < 5; j++) {
            cout << setw(2) << games[i].sort_cards[j] << " ";
        }
        cout << "Num equal: ";
        for (int j = 0; j < 5; j++) {
            cout << setw(2) << games[i].num_equal[j] << " ";
        }
        cout << "Strength: " << (int) games[i].strength << " bid: " << games[i].bid << " rank: " << games[i].rank << endl;
        res += (long long) games[i].rank * (long long) games[i].bid;
    }


    end = std::chrono::system_clock::now();
    std::cout << "Working Took: " << std::chrono::duration_cast<std::chrono::seconds>(end-start).count() << " seconds" << endl; 
    std::cout << "RESULT: " << res << endl << endl;
    // 250764353 too low??
    // 251029473
    // 251429803 too high
    // 251067004 too high
    // 251351227
    // 251274372
    // 249684748 too low
    // 250501691 not correct
    // Hand: 13 14  1  2  2 Sorted hand: 13 14  2  2  1 Strength: 1 bid: 346 rank: 338 wrong ska vara triss
    // 250637576 not correct
    // Hand: 13 13 12  1 12 Sorted hand: 13 13 12 12  1 Num equal:  3  1  3  1  1 Strength: 3 bid: 449 rank: 698 wrong ska vara house
    // 250344087
    // Hand: 14  1  1  9  4 Sorted hand: 14  9  4  1  1 Num equal:  3  3  3  1  1 Strength: 4 bid: 351 rank: 804 wrong ska vara triss inte house
    // 250637576
    // Hand: 13 13 12  1 12 Sorted hand: 13 13 12 12  1 Num equal:  3  1  3  1  1 Strength: 3 bid: 449 rank: 698 wrong ska vara house inte triss
    // 251003917

    return 0;
}