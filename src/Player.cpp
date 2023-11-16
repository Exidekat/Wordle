#include "Player.h"

Player::Player() {
    this->name = "N/A";
    this->score = 0;
    this->timesPlayed = 0;
}
Player::Player(std::string name) {
    this->name = name;
    this->score = 0;
    this->timesPlayed = 0;
}