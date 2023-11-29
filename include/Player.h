#ifndef H_Player
#define H_Player
#include <iostream>

class Player {
public:
    Player();
    Player(std::string name);
    int timesPlayed;
    double score;
    std::string name;
};


#endif
