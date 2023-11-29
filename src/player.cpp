#include "player.h"

player::player() {
    this->name = "N/A";
    this->score = 0;
    this->timesPlayed = 0;
}
player::player(std::string name) {
    this->name = name;
    this->score = 0;
    this->timesPlayed = 0;
}