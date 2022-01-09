#ifndef GROUP
#define GROUP

#include "Player.h"
#include "RankNode.h"
#include "RankTree.h"
#include <iostream>


const int EMPTY_GROUP_SIZE = 0;

class Group
{
    int                         group_id;
    int                         group_size;
    int                         group_uf_size;  // for the union.
    RankTree<int>*              rank_group_players;
    //RankNode<int>*            min_group_player;   
    
    public:

    /* C'tors */
        Group(int group_id, int scale, int group_size = EMPTY_GROUP_SIZE);  
        Group(Group& to_copy);
        ~Group();
    /* Get functions */
        int const getGroupID() const; 
        int getGroupSize() const;
        RankTree<int>* getGroupPlayersTree();
        //RankNode<int>* getMinPlayer() const;

    /* Set functions */  
        void setGroupSize(int new_size);
        //void setMinGroupPlayer(RankNode<int>* new_min_player);
        void setGroupPlayersTree(RankTree<int>* tree);

    /* Operators */
        
        bool operator==(const Group& group) const;
        bool operator>(const Group& group) const;
        bool operator<(const Group& group) const;  
        
};

#endif