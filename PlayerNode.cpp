#include "PlayerNode.h"

PlayerNode::PlayerNode()
{
    this->player = nullptr;
    this->next = nullptr;
}

PlayerNode::PlayerNode(Player* player_t){
    this->player = new Player(player_t->getPlayerID(), player_t->getPlayerGroupID(), player_t->getPlayerScore(), player_t->getPlayerLevel()); 
    this->next = nullptr;
}

PlayerNode::~PlayerNode(){
    delete this->player;
    this->player = nullptr;
    this->next = nullptr;
}

void PlayerNode::setPlayer(Player* player)
{
    this->player = player;
}

Player* PlayerNode::getPlayer() const{
    return this->player;
}
PlayerNode* PlayerNode::getNext() const{
    return this->next;
}

void PlayerNode::setNext(PlayerNode* new_next){
    this->next = new_next;
}
