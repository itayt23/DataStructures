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
    delete player;
    this->player = nullptr;
    //this->next = nullptr;
    //this->clearChain();
}

void PlayerNode::clearChain()
{
    PlayerNode* node = this;
    PlayerNode* next_node = this;
    while(next_node != nullptr)
    {
        node = next_node;
        next_node = node->getNext();
        delete node->getPlayer();
        node->setPlayer(nullptr);
        node->setNext(nullptr);
        node = nullptr;
        delete node;
    }
    //delete node;
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
