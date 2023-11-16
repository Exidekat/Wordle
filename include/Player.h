#ifndef H_PLAYER
#define H_PLAYER
#include <iostream>

class Player {
public:
    Player();
    Player(std::string name);
    int timesPlayed;
    double score;
    std::string name;
};


#endif //H_PLAYER
