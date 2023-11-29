#ifndef H_Player
#define H_Player
#include <iostream>

class player {
public:
    player();
    player(std::string name);
    int timesPlayed;
    double score;
    std::string name;
};


#endif
