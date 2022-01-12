#include "PlayerNode.h"
//#include "Player.h"

PlayerNode::PlayerNode()
{
    this->player = nullptr;
    this->next = nullptr;
}

PlayerNode::PlayerNode(Player* player_t){
    this->player = new Player(player_t->getPlayerID(), player_t->getPlayerGroupID(), player_t->getPlayerScore(), player_t->getPlayerLevel());   // calling copy c'tor, player is pointing to the newly created obj.
    this->next = nullptr;
}

PlayerNode::~PlayerNode(){
    if(player != nullptr)
        delete player;
    this->player = nullptr;
    this->next = nullptr;
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
