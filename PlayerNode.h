#include "Player.h"

class PlayerNode
{
private:
    Player* player;
    PlayerNode* next;
public:
    PlayerNode();
    PlayerNode(Player* player_t);
    ~PlayerNode();
    Player* getPlayer() const;
    PlayerNode* getNext() const;
    void setNext(PlayerNode* new_next);
};


