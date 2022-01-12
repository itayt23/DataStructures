#include "Group.h"


const int UF_INITIAL_SIZE = 1;
/* C'tors */
Group::Group(int group_id, int scale, int group_size)
{
    this->group_id = group_id;
    this->group_size = group_size;
    //this->group_uf_size = UF_INITIAL_SIZE;
    this->rank_group_players = new RankTree<int>(scale); 
    //this->min_group_player = nullptr;
}

Group::~Group(){
    if(this->rank_group_players != nullptr)
    {
        this->rank_group_players->clearTree();
        this->rank_group_players = nullptr;
    }    
}

Group::Group(Group& group)
{
    this->group_id = group.group_id;
    this->group_size = group.group_size;
    this->rank_group_players = group.getGroupPlayersTree()->copyTree();  // TODO: this is not a copy but an assignment. A place for bugs. 
}

/* Get functions */
int const Group::getGroupID() const{
    return this->group_id;
}

int Group::getGroupSize() const{
    return this->group_size;
}

RankTree<int>* Group::getGroupPlayersTree(){
    return this->rank_group_players;
}



/* Set functions */
void Group::setGroupSize(int new_size){
    this->group_size = new_size;
}


/**
 * @brief ASSIGNS the given pointer, does not copy the whole content!
 * 
 * @param new_min_player 
 */
 /*
void Group::setMinGroupPlayer(RankNode<int>* new_min_player){
    this->min_group_player = min_group_player;    // this an assignment and not a copy of the value
}
*/


/**
 * @brief ASSIGNS the given pointer, does not copy the whole content!
 * 
 * @param tree 
 */
void Group::setGroupPlayersTree(RankTree<int>* tree)
{
    this->rank_group_players = tree;
}




// OPERATORS:
bool Group::operator==(const Group& group) const
{
    if(this->group_id == group.group_id)
    {
        return true;
    }
    return false;
}

bool Group::operator<(const Group& group) const
{
    if(this->group_id < group.group_id)
    {
        return true;
    }
    return false;
}

bool Group::operator>(const Group& group) const {
    if(this->group_id > group.group_id)
    {
        return true;
    }
    return false;
}


