#include "Player.h"

class PlayerNode
{
private:
    Player* player;     // a pointer to the Player
    PlayerNode* next;   // a pointer to the next PlayerNode.
public:
    PlayerNode();
    PlayerNode(Player* player_t);
    ~PlayerNode();
    Player* getPlayer() const;
    PlayerNode* getNext() const;
    void setNext(PlayerNode* new_next);
    void clearChain();
    void setPlayer(Player* player);
};


