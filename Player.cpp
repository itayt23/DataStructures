#include "Player.h"

#include <iostream>


/* C'tors ------------------------------------------------------------------------------------------------------------*/
Player::Player(int player_id, int GroupID, int score, int level){
    this->player_id = player_id;
    this->level = level;
    this->score = score;
    this->player_groupID = GroupID;
}

Player::Player(const Player& to_copy){
    this->player_id = to_copy.player_id;
    this->level = to_copy.level;
    this->player_groupID = to_copy.player_groupID; 
    this->score = to_copy.score;
}

Player::~Player() {} 

/* Get functions----------------------------------------------------------------------------------------------------- */
int Player::getPlayerID() const {
    return this->player_id;
}

int Player::getPlayerLevel() const {
    return this->level;
}

int Player::getPlayerScore() const{
    return this->score;
}

int Player::getPlayerGroupID() const{
    return this->player_groupID;
}


/* Set functions----------------------------------------------------------------------------------------------------- */
void Player::setPlayerLevel(int new_level){
    this->level = new_level;
}

void Player::setPlayerScore(int new_score){
    this->score = new_score;
}


/* Auxiliary functions----------------------------------------------------------------------------------------------- */

bool Player::SameID(Player& p1){
    return this->player_id == p1.player_id;
}

bool Player::differentID(Player& p1){
    return this->player_id != p1.player_id;
}

/* Operators ---------------------------------------------------------------------------------------------------------*/
bool Player::operator==(const Player& player) const {
    if(this->level == player.level) {
        return true;
    }
    return false;
}

//! NOTE: < is <= when using levels - meaning that the matching levels will be sorted to the left ! Note: sorting is done by the levels
bool Player::operator<(const Player& player) const {
    if(this->level < player.level) {
        return true;
    }
    return false;
}

bool Player::operator>(const Player& player) const {
    if(this->level > player.level){
        return true;
    }
    return false;
}


Player& Player::operator=(const Player& player){
    if(this == &player){
        return *this;
    } else {
        this->player_id = player.player_id;
        this->level = player.level;
        this->score = player.score;
        this->player_groupID = player.player_groupID;
        return *this;
    }
}

bool Player::operator!=(const Player& player){
    if(this==&player || this->level == player.level){
        return false;
    } else {
        return true;
    }

}
/*
std::ostream& operator<<(std::ostream& os, const Player& player){
    return os << player.getPlayerID() << " " << player.getPlayerLevel();
}
*/





