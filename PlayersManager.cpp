#include "PlayersManager.h"
#include "exceptions.h"
#include <cmath>


static double getBestPlayersAverageLevel(RankNode<int>* node, int m);
static int highestPlayersByLevel(RankNode<int>* node, int* amount, int players_by_level);
static void treeToArray(RankNode<int>* node, RankNode<int>** array, int* array_size);
static void mergeArrays(RankNode<int>** group1_array, RankNode<int>** group2_array, RankNode<int>** merge_array,
                         int group1_size, int group2_size, int merge_size, int scale);
static int findTreeHeight(int n);    
static RankNode<int>* buildCompleteTree(int h, RankNode<int>* parent, int scale);
static RankNode<int>* removeExtraLeafs(RankNode<int>* node, int* n);
static void arrayToTree(RankNode<int>* node, RankNode<int>** array, int* array_size, int* index);
static RankNode<int>** fixMergedArray(RankNode<int>** merge_array, int merged_array_size, int merged_array_size_final);
static void updateLevelZero(RankNode<int>* level_zero_final, RankNode<int>* group_tree1_level_zero, RankNode<int>* group_tree2_level_zero);
static double getAverageHighestLevel(RankNode<int>* iter_node, int m);


/* C'tors & D'tors -------------------------------------------------------------------------------------------------- */
PlayersManager::PlayersManager(int k, int scale) {
    this->players_table = new ChainHashTable();
    this->groups_uf = new UnionFind<Group>(k, scale);
    this->all_players_tree = new RankTree<int>(scale);
    this->total_players = INITIAL_TOTAL_PLAYERS;
    this->groups_size = k;
    this->score_scale = scale;
}

PlayersManager::~PlayersManager(){
    this->all_players_tree->clearTree();
    delete this->players_table;
    delete this->groups_uf;
    this->all_players_tree = nullptr;
    this->players_table = nullptr;
    this->groups_uf = nullptr;
}

/* Primary DS functions ----------------------------------------------------------------------------------------------*/
StatusType PlayersManager::MergeGroups(int GroupID1, int GroupID2){
    if(GroupID1 <= 0 || GroupID2 <= 0 || GroupID1 > this->groups_size || GroupID2 > this->groups_size) return INVALID_INPUT;
    RankTree<int>* group_tree1 = this->groups_uf->find(GroupID1)->getGroupPlayersTree();
    RankTree<int>* group_tree2 = this->groups_uf->find(GroupID2)->getGroupPlayersTree();
    RankNode<int>* group_tree1_level_zero =  group_tree1->getLevelZero();
    RankNode<int>* group_tree2_level_zero =  group_tree2->getLevelZero();
    int group1_array_size = group_tree1->getRootSize();
    int group2_array_size = group_tree2->getRootSize();
    int merged_array_size = group1_array_size + group2_array_size;
    int merged_array_size_final = 0;
    //group_tree1->printTree();
    //group_tree2->printTree();
    if(this->groups_uf->find(GroupID1)->getGroupID() == this->groups_uf->find(GroupID2)->getGroupID()) return SUCCESS;
    if(group_tree1->getRootNode() == nullptr && group_tree1->getLevelZero() == nullptr)
    {
        this->groups_uf->unionGroups(GroupID1, GroupID2);
        Group* root_group = this->groups_uf->find(GroupID1);
        root_group->setGroupPlayersTree(group_tree2);
        if(root_group->getGroupID() == GroupID1)
        {
            Group* son_gorup = this->groups_uf->getGroup(GroupID2);
            son_gorup->setGroupPlayersTree(nullptr);
        }
        else
        {
            Group* son_gorup = this->groups_uf->getGroup(GroupID1);
            son_gorup->setGroupPlayersTree(nullptr);
        }
        return SUCCESS;
    } 
    if(group_tree2->getRootNode() == nullptr && group_tree2->getLevelZero() == nullptr)
    {
        this->groups_uf->unionGroups(GroupID1, GroupID2);
        Group* root_group = this->groups_uf->find(GroupID1);
        root_group->setGroupPlayersTree(group_tree1);
        if(root_group->getGroupID() == GroupID1)
        {
            Group* son_gorup = this->groups_uf->getGroup(GroupID2);
            son_gorup->setGroupPlayersTree(nullptr);
        }
        else
        {
            Group* son_gorup = this->groups_uf->getGroup(GroupID1);
            son_gorup->setGroupPlayersTree(nullptr);
        }
        return SUCCESS;
    } 
    try{
        int temp_group1_array_size = group1_array_size;
        int temp_group2_array_size = group2_array_size;
        RankNode<int>** group1_array = new RankNode<int>*[group1_array_size];
        RankNode<int>** group2_array = new RankNode<int>*[group2_array_size];
        RankNode<int>** merged_array = new RankNode<int>*[merged_array_size];
        treeToArray(group_tree1->getRootNode(), group1_array, &temp_group1_array_size);
        treeToArray(group_tree2->getRootNode(), group2_array, &temp_group2_array_size);
        mergeArrays(group1_array, group2_array, merged_array, group1_array_size, group2_array_size, merged_array_size, this->score_scale);
        // count how many cells are actually used:
        for(int i = 0; i< merged_array_size; i++){
            if((merged_array[i]->getKey()) != nullptr){
                merged_array_size_final++;
            }
        }
        //reducting the unused chunk of @merged_array to @merged_array_final
        RankNode<int>** merged_array_final = fixMergedArray(merged_array, merged_array_size, merged_array_size_final);

        int new_tree_height = findTreeHeight(merged_array_size_final); // merged_array_size == number of nodes in the new tree;
        RankNode<int>* final_tree_root = buildCompleteTree(new_tree_height, nullptr, this->score_scale);
        int full_tree_size = pow(2, new_tree_height + 1) - 1;
        int leaves_to_remove = full_tree_size - merged_array_size_final;
        final_tree_root = removeExtraLeafs(final_tree_root, &leaves_to_remove);
        // write an array to tree function which assign shit from the array to the tree
        int index = 0;
        int temp_size = merged_array_size_final;
        arrayToTree(final_tree_root, merged_array, &temp_size, &index);
        int* zero = new int(0);
        RankNode<int>* level_zero_final = new RankNode<int>(zero, nullptr, 0 ,this->score_scale);
        updateLevelZero(level_zero_final, group_tree1_level_zero, group_tree2_level_zero);
        RankTree<int>* final_tree = new RankTree<int>(final_tree_root, level_zero_final, this->score_scale, merged_array_size_final);
        if(group_tree1_level_zero == nullptr && group_tree2_level_zero == nullptr)
        {
            final_tree->getLevelZero()->setPlayersAmount(0);
            final_tree->getLevelZero()->setPlayersAmountSubTree(0);
        }
        //group_tree1->printZeroTree();
        //group_tree2->printZeroTree();
        //final_tree->printZeroTree();
        group_tree1->clearTree();
        group_tree2->clearTree();
        this->groups_uf->unionGroups(GroupID1, GroupID2);
        Group* root_group = this->groups_uf->find(GroupID1);
        root_group->setGroupPlayersTree(final_tree);
        if(root_group->getGroupID() == GroupID1)
        {
            Group* son_gorup = this->groups_uf->getGroup(GroupID2);
            son_gorup->setGroupPlayersTree(nullptr);
        }
        else
        {
            Group* son_gorup = this->groups_uf->getGroup(GroupID1);
            son_gorup->setGroupPlayersTree(nullptr);
        }
        //root_group->getGroupPlayersTree()->printTree();
        for(int i=0; i<merged_array_size_final;i++)
        {
            delete merged_array_final[i];
        }
        delete[] group1_array;
        delete[] group2_array;
        delete[] merged_array;
        delete[] merged_array_final;
        //delete[] zero;
    } catch (AllocationError &e) {
        return ALLOCATION_ERROR;
    } catch (NodeDosentExist &e) {
        return FAILURE;
    }
    return SUCCESS;

}
StatusType PlayersManager::AddPlayer(int PlayerID, int GroupID, int score){
    if(GroupID <= 0 || GroupID > this->groups_size || PlayerID <= 0 || score > this->score_scale|| score <= 0) return INVALID_INPUT;
    Player new_player(PlayerID, GroupID, score);    // At the end of this scope this player will die but the hash saves a copy of this object so dont worry be happy
    try{
        HT_status status = this->players_table->insertHT(&new_player);
        if(status == HT_ALREADY_EXISTS) return FAILURE;
        this->all_players_tree->insert(new_player.getPlayerLevel(), new_player.getPlayerScore());   
        this->groups_uf->find(GroupID)->getGroupPlayersTree()->insert(new_player.getPlayerLevel(), score);
        //this->groups_uf->find(GroupID)->getGroupPlayersTree()->printZeroTree();
        this->total_players++;
    } catch (AllocationError &e) {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}
StatusType PlayersManager::RemovePlayer(int PlayerID){
    if(PlayerID <= 0) return INVALID_INPUT;
    Player* removed_player = this->players_table->findHT(PlayerID); // ~O(1) hash table find
    if(removed_player == nullptr) { return FAILURE; } // the wanted players doesn't exist.
    int removed_player_groupid = removed_player->getPlayerGroupID();
    int removed_player_level = removed_player->getPlayerLevel();
    int removed_player_score = removed_player->getPlayerScore();
    this->players_table->deleteHT(PlayerID);  // ~O(1) hash table find   // no need to check HT_status, we can assert that it's a success.
    this->all_players_tree->remove(removed_player_level, removed_player_score); // O(log n) rank tree, n is the players
    //if(removed_player_level !=0)
        //this->all_players_tree->printTree();
    this->groups_uf->find(removed_player_groupid)->getGroupPlayersTree()->remove(removed_player_level, removed_player_score); // O(log* k) - find and remove in UnionFind.
    //if(removed_player_groupid == 25)
    //this->groups_uf->find(removed_player_groupid)->getGroupPlayersTree()->printTree();
    //this->groups_uf->find(removed_player_groupid)->getGroupPlayersTree()->printZeroTree();
    this->total_players--;
    return SUCCESS;
}

StatusType PlayersManager::IncreasePlayerIDLevel(int PlayerID, int LevelIncrease){
    if(PlayerID <= 0 || LevelIncrease <= 0) return INVALID_INPUT;
    Player* target_player = this->players_table->findHT(PlayerID); // ~O(1) hash table find
    if(target_player == nullptr) { return FAILURE; } // the wanted players doesn't exist.
    int player_groupid = target_player->getPlayerGroupID();
    int player_score = target_player->getPlayerScore();
    int old_player_level = target_player->getPlayerLevel();
    int new_player_level = old_player_level + LevelIncrease;
    target_player->setPlayerLevel(new_player_level);
    //removing the old data, and then insert the updated data.
    this->all_players_tree->remove(old_player_level, player_score); // O(log n) rank tree, n is the players
    //this->all_players_tree->printTree();
    this->groups_uf->find(player_groupid)->getGroupPlayersTree()->remove(old_player_level, player_score); // O(log* k) - find and remove in UnionFind.
    //this->groups_uf->find(player_groupid)->getGroupPlayersTree()->printTree();
    //this->groups_uf->find(player_groupid)->getGroupPlayersTree()->printZeroTree();
    try {
        this->all_players_tree->insert(new_player_level, player_score); // O(log n) rank tree, n is the players
        this->groups_uf->find(player_groupid)->getGroupPlayersTree()->insert(new_player_level, player_score); // O(log* k) - find and remove in UnionFind.
        //if(player_groupid == 25)
        //this->groups_uf->find(player_groupid)->getGroupPlayersTree()->printTree();
        //this->groups_uf->find(player_groupid)->getGroupPlayersTree()->printZeroTree();
    } catch (AllocationError& e) { return ALLOCATION_ERROR; }
    return SUCCESS;
}

StatusType PlayersManager::ChangePlayerIDScore(int PlayerID, int NewScore){
    if(PlayerID <= 0 || NewScore > this->score_scale || NewScore <= 0) return INVALID_INPUT;
    Player* target_player = this->players_table->findHT(PlayerID); // ~O(1) hash table find
    if(target_player == nullptr) { return FAILURE; } // the wanted players doesn't exist.
    int player_groupid = target_player->getPlayerGroupID();
    int old_player_score = target_player->getPlayerScore();
    int player_level = target_player->getPlayerLevel();
    target_player->setPlayerScore(NewScore);
    //removing the old data, and then insert the updated data.
    this->all_players_tree->remove(player_level, old_player_score); // O(log n) rank tree, n is the players
    //this->all_players_tree->printTree();
    this->groups_uf->find(player_groupid)->getGroupPlayersTree()->remove(player_level, old_player_score); // O(log* k) - find and remove in UnionFind.
    try {
        this->all_players_tree->insert(player_level, NewScore); // O(log n) rank tree, n is the players
        this->groups_uf->find(player_groupid)->getGroupPlayersTree()->insert(player_level, NewScore); // O(log* k) - find and remove in UnionFind.
        //if(player_groupid == 25)
        //this->groups_uf->find(player_groupid)->getGroupPlayersTree()->printZeroTree();
        //this->groups_uf->find(player_groupid)->getGroupPlayersTree()->printTree();
        //this->all_players_tree->printTree();
    } catch (AllocationError& e) { return ALLOCATION_ERROR; }
    return SUCCESS;
}
//TODO need to handle with failure
StatusType PlayersManager::GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel,
                                                                 int higherLevel, double * players){
    if(GroupID < 0|| GroupID > this->groups_size) return INVALID_INPUT;
    int* total_players_min = new int(0);
    int* total_players_in_score_min = new int(0);
    int* total_players_max = new int(0);
    int* total_players_in_score_max = new int(0);
    double level_players = 0;
    double level_score = 0;
    double score_final = 0;
    double player_final = 0;
    if(lowerLevel > higherLevel) return FAILURE; 
    if(GroupID == 0)
    {
        //this->all_players_tree->printTree();
        if(this->all_players_tree->getLevelZero() == nullptr && this->all_players_tree->getRootNode() == nullptr) return FAILURE;
        if(higherLevel == 0 && this->all_players_tree->getLevelZero() == nullptr) return FAILURE;
        if(this->all_players_tree->getRootNode() != nullptr)
        {
            if(lowerLevel > *( this->all_players_tree->getRootNode()->getMaxNode()->getKey())) return FAILURE;
        }
        if(higherLevel == 0)
        {
            level_players = this->all_players_tree->getLevelZero()->getPlayersAmount();
            level_score = (this->all_players_tree->getLevelZero()->getScoreArr())[score];
            *players = (level_score / level_players)*100; 
            return SUCCESS;
        }
        if(this->all_players_tree->getRootNode() == nullptr)
        {
            level_players = this->all_players_tree->getLevelZero()->getPlayersAmount();
            level_score = (this->all_players_tree->getLevelZero()->getScoreArr())[score];
        }
        else
        {
            this->all_players_tree->rankMin(lowerLevel,score,total_players_min,total_players_in_score_min);
            this->all_players_tree->rankMax(higherLevel,score,total_players_max,total_players_in_score_max);
            if(lowerLevel == 0)
            {

                level_players = this->all_players_tree->getLevelZero()->getPlayersAmount();
                level_score = (this->all_players_tree->getLevelZero()->getScoreArr())[score];
            }
        }
        score_final = *(total_players_in_score_max) - *(total_players_in_score_min) + level_score;
        player_final = *(total_players_max) - *(total_players_min) + level_players;
    }
    else
    {
        RankTree<int>* group_tree = this->groups_uf->find(GroupID)->getGroupPlayersTree();
        //group_tree->printTree();
        //group_tree->printZeroTree();
        if(group_tree->getRootNode() != nullptr)
        {
            if(lowerLevel > *(group_tree->getRootNode()->getMaxNode()->getKey())) return FAILURE;
        }
        if(higherLevel == 0 && group_tree->getLevelZero() == nullptr) return FAILURE;
        if(higherLevel == 0)
        {
            level_players = group_tree->getLevelZero()->getPlayersAmount();
            level_score = (group_tree->getLevelZero()->getScoreArr())[score];
            *players = (level_score / level_players)*100; 
            return SUCCESS;
        }
        if(group_tree->getRootNode() == nullptr)
        {
            level_players = group_tree->getLevelZero()->getPlayersAmount();
            level_score = (group_tree->getLevelZero()->getScoreArr())[score];
        }
        else
        {
            group_tree->rankMin(lowerLevel,score,total_players_min,total_players_in_score_min);
            group_tree->rankMax(higherLevel,score,total_players_max,total_players_in_score_max);
            if(lowerLevel == 0)
            {
                level_players = group_tree->getLevelZero()->getPlayersAmount();
                level_score = (group_tree->getLevelZero()->getScoreArr())[score];
            }
        }
        score_final = *(total_players_in_score_max) - *(total_players_in_score_min) + level_score;
        player_final = *(total_players_max) - *(total_players_min) + level_players;
    }
    *(players) = double((score_final / player_final)*100); 
    delete[] total_players_min;
    delete[] total_players_in_score_min;
    delete[] total_players_in_score_max;
    delete[] total_players_max;
    return SUCCESS;

}

StatusType PlayersManager::AverageHighestPlayerLevelByGroup(int GroupID, int m, double * avgLevel){
    if(GroupID < 0|| GroupID > this->groups_size || m <= 0) return INVALID_INPUT;
    if( m > this->total_players) return FAILURE;
    if(GroupID == 0)
    {
        if(this->all_players_tree->getRootNode() == nullptr)
        {
            *(avgLevel) = 0;
            return SUCCESS;
        }
        //this->all_players_tree->printTree();
        *(avgLevel) = getAverageHighestLevel(this->all_players_tree->getRootNode(), m);
        return SUCCESS;
    }
    RankTree<int>* group_tree = this->groups_uf->find(GroupID)->getGroupPlayersTree();
    //group_tree->printTree();
    if(group_tree->getRootNode() == nullptr)
    {
        *(avgLevel) = 0;
        return SUCCESS;
    }
    *(avgLevel) = getAverageHighestLevel(group_tree->getRootNode(), m);
    return SUCCESS;
}


void PlayersManager::Quit(){
    delete this;
}


StatusType PlayersManager::GetPlayersBound(int GroupID, int score, int m, int* LowerBoundPlayers, int* HigherBoundPlayers){
   return SUCCESS;
}

/* Auxiliary inner functions ---------------------------------------------------------------------------------------- */

/**
 * @brief Get the Best @m Players Average
 * 
 * @param node 
 * @param m - number of wanted players, ASSUMES a valid m.
 * @param aux_sum - on start up shall be 0
 * @param aux_numOfPlayers - on startup shall be 0
 * @return double 
 */
static double getBestPlayersAverageLevel(RankNode<int>* node, int m){
    int current_subtree_players = node->getPlayersAmountSubTree();  // the whole subtree (including the node)
    //int current_node_players = node->getPlayersAmount();    // only the players in the node
    int root_level = *(node->getKey());     // the key is the pointer to the level.
    int root_only_numOfPlayers = node->getPlayersAmount();
    int left_son_level = *(node->getLeftSon()->getKey());
    int right_son_level = *(node->getRightSon()->getKey());

    if(m == current_subtree_players) { return (node->getLevelsSumSubTree() / m); }
    if(node->hasRightSon() == false && node->hasLeftSon() == false ){
        return root_level;
    } else if( node->hasOnlyOneSon() && node->hasRightSon() ){
        int right_son_numOfPlayers = node->getRightSon()->getPlayersAmount();
        int remiaining_numOfPlayers = m - right_son_numOfPlayers;
        if( m <= right_son_numOfPlayers) { return right_son_level; } 
        else {
            return (root_level*remiaining_numOfPlayers + right_son_level*right_son_numOfPlayers)/(m);
        }
    } else if( node->hasOnlyOneSon() && node->hasLeftSon() ){
        int remiaining_numOfPlayers = m - root_only_numOfPlayers;
        if( m <= root_only_numOfPlayers) { return root_level; }
        else { 
            return (root_level*root_only_numOfPlayers + left_son_level*remiaining_numOfPlayers)/(m);
        }
    } else if(node->hasTwoSon()) {
        if( m < node->getPlayersAmountSubTree() && m <= node->getRightSon()->getPlayersAmountSubTree()) {       // m <= #(v->right) && m < #(v)
            return getBestPlayersAverageLevel(node->getRightSon(), m);
        } else if(m < node->getPlayersAmountSubTree() && m > node->getRightSon()->getPlayersAmountSubTree()) {   // #(v->right) < m < #(v)
                int right_subtree_numOfPlayers = node->getRightSon()->getPlayersAmountSubTree();
                int right_subtree_sum = node->getRightSon()->getLevelsSumSubTree();
                int remaining_numOfPlayers = m - right_subtree_numOfPlayers;
                if(root_only_numOfPlayers + right_subtree_numOfPlayers > m) {
                    return ( remaining_numOfPlayers*root_level + right_subtree_sum )/(m);
                } else {    // m=18 in my example while
                    int sum = remaining_numOfPlayers*getBestPlayersAverageLevel(node->getLeftSon(), remaining_numOfPlayers)
                            + root_only_numOfPlayers*root_level + right_subtree_sum;
                    return sum/m;
                }
        }
    }
    return -1; // we shall NEVER get here. 
}


static int highestPlayersByLevel(RankNode<int>* node, int* amount, int players_by_level){
    if(node == nullptr) return players_by_level; // before it was just return...
    if((*amount) == 0) return players_by_level;
    highestPlayersByLevel(node->getRightSon(), amount, players_by_level);
    if((*amount) > node->getPlayersAmount())
    {
        players_by_level += (node->getPlayersAmount()) * (*(node->getKey()));
        *(amount) = *(amount) - (node->getPlayersAmount());    // There are "leftovers"
    }
    else {
        players_by_level += (*amount) * (*(node->getKey()));
        (*amount) = 0;
    }
    highestPlayersByLevel(node->getLeftSon(), amount, players_by_level); 
    return players_by_level;
}

static void treeToArray(RankNode<int>* node, RankNode<int>** array, int* array_size){
    if(*array_size == 0 || node == nullptr){
        return;
    }
    treeToArray(node->getRightSon(), array, array_size);
    array[*(array_size) - 1] = node;
    (*array_size)--;
    treeToArray(node->getLeftSon(), array, array_size); 

}

/**
 * @brief merges @group1_array and @group2_array into @merge_array in an ascending order using merge (O(n)).
 * 
 * @param group1_array 
 * @param group2_array 
 * @param merge_array 
 * @param group1_size 
 * @param group2_size 
 * @param merge_size 
 */
static void mergeArrays(RankNode<int>** group1_array, RankNode<int>** group2_array, RankNode<int>** merge_array, 
                        int group1_size, int group2_size, int merge_size, int scale){
    int i_a=0;
    int i_b=0;
    int j=0;
    while( j < merge_size ){
       if(i_a < group1_size && i_b < group2_size){
            if( *(group1_array[i_a]->getKey()) > *(group2_array[i_b]->getKey())){
                merge_array[j++] = group2_array[i_b++]->copyNode();
            }            
            else if(*(group1_array[i_a]->getKey()) < *(group2_array[i_b]->getKey())){ 
                merge_array[j++] = group1_array[i_a++]->copyNode();
            }
            else if (*(group1_array[i_a]->getKey()) == *(group2_array[i_b]->getKey())){
                merge_array[j] =  group1_array[i_a]->copyNode();
                merge_array[j]->setPlayersAmount(merge_array[j]->getPlayersAmount() + group2_array[i_b]->getPlayersAmount());
                merge_array[j]->setNodeLevelSum((merge_array[j]->getPlayersAmount()) * *(merge_array[j]->getKey()));
                for(int i=1; i<=group2_array[i_b]->getScale(); i++){
                    merge_array[j]->getScoreArr()[i] = group1_array[i_a]->getScoreArr()[i] + group2_array[i_b]->getScoreArr()[i];
                }
                j++;
                i_a++;
                i_b++;
            }
        }
        else    // either (i_a >= group1_size) or (i_b >= group2_size) - means we got to the end of one of the arrays
        {
            if(i_a >= group1_size && i_b >= group2_size)
            {
                merge_array[j++] = new RankNode<int>(nullptr, nullptr, 0, scale);
            }
            else if(i_a >= group1_size)
            {
                merge_array[j++] = group2_array[i_b++]->copyNode();       
            }     
            else if (i_b >= group2_size)
            {
                merge_array[j++] = group1_array[i_a++]->copyNode();
            }
        }
   }
}


static int findTreeHeight(int n){
    double h = log2(n + 1) - 1;
    return (int)ceil(h);
}

/**
 * @brief builds a complete binary tree with height @h. The caller must call the function with @parent=nullptr 
          (as per definition of the root's parent pointing to nullptr)
 * 
 * @param h - the height
 * @param parent - must be nullptr on calling, used for the recursive build of the tree
 * @return RankNode<int>* - the root to the newly built tree
 */
static RankNode<int>* buildCompleteTree(int h, RankNode<int>* parent, int scale){
    if(h < 0){
        return nullptr;
    }
    //hwo to: RankNode<T>::RankNode(T* key, RankNode<T>* parent, int score, int scale)
    RankNode<int>* node = new RankNode<int>(nullptr, parent, 0, scale);    // in case of 'out of mem' the father funciton shall catch the exception.    
    node->setHeight(h);
    node->setRightSon(buildCompleteTree(h-1, node, scale));
    node->setLeftSon(buildCompleteTree(h-1, node, scale));
    return node;
}

/**
 * @brief removes the extra leaves from the complete tree (from right to left)
 * 
 * @param node - the tree's root
 * @param n - how many leaves to remove
 * @return RankNode<int>* 
 */
static RankNode<int>* removeExtraLeafs(RankNode<int>* node, int* n)
{
    if(*n == 0 || node == nullptr){
        return node;
    }
    if(node->isLeaf()){
        (*n)--; 
		delete node;
        return nullptr;
    }
    node->setRightSon(removeExtraLeafs(node->getRightSon(), n));
    node->setLeftSon(removeExtraLeafs(node->getLeftSon(), n));
    return node;
}

/**
 * @brief recieves a tree's root @node, and an filled array @merged_array, it's @array_size and @index to write into 
          recursively. Fills the given tree with the data from the array.
 * 
 * @param node 
 * @param merged_array 
 * @param array_size 
 * @param index 
 */
static void arrayToTree(RankNode<int>* node, RankNode<int>** merged_array, int* array_size, int* index){
    if( (*index) == (*array_size) || node == nullptr){
        return;
    }
    arrayToTree(node->getLeftSon(), merged_array, array_size, index);   // recursive call to the left subtree
    int* key = new int(*(merged_array[*index]->getKey()));
    //node->setKey(merged_array[*index]->getKey());
    node->setKey(key);
    node->setPlayersAmount(merged_array[*index]->getPlayersAmount());
    node->setScoreArr(merged_array[*index]->copyScoreArr());
    node->setNodeLevelSum(merged_array[*index]->getLevelSum());
    node->updateNodeFeatures();
    node->updateLevelSumSubTree();
    (*index)++;

    arrayToTree(node->getRightSon(), merged_array, array_size, index);  // recursive call to the rught subtree
    node->updateNodeFeatures();
    node->updateLevelSumSubTree();
}

static RankNode<int>** fixMergedArray(RankNode<int>** merge_array, int merged_array_size, int merged_array_size_final)
{
    RankNode<int>** merged_array_final = new RankNode<int>*[merged_array_size_final];
    for(int i = 0; i< merged_array_size_final; i++)
    {
        merged_array_final[i] = merge_array[i];
    }
    return merged_array_final;
}

static void updateLevelZero(RankNode<int>* level_zero_final, RankNode<int>* group_tree1_level_zero, RankNode<int>* group_tree2_level_zero)
{
    if(group_tree1_level_zero == nullptr && group_tree2_level_zero == nullptr) level_zero_final = nullptr;
    else if(group_tree1_level_zero != nullptr && group_tree2_level_zero == nullptr)
    {
        for(int i = 1; i<= group_tree1_level_zero->getScale(); i++)
        {
            level_zero_final->getScoreArr()[i] = group_tree1_level_zero->getScoreArr()[i];
            level_zero_final->getScoreArrSubTree()[i] = group_tree1_level_zero->getScoreArrSubTree()[i];
        }
        level_zero_final->setPlayersAmount(group_tree1_level_zero->getPlayersAmount());
        level_zero_final->setPlayersAmountSubTree(group_tree1_level_zero->getPlayersAmountSubTree());
    }
    else if(group_tree1_level_zero == nullptr && group_tree2_level_zero != nullptr)
    {
        for(int i = 1; i<= group_tree2_level_zero->getScale(); i++)
        {
            level_zero_final->getScoreArr()[i] = group_tree2_level_zero->getScoreArr()[i];
            level_zero_final->getScoreArrSubTree()[i] = group_tree2_level_zero->getScoreArrSubTree()[i];
        }
        level_zero_final->setPlayersAmount(group_tree2_level_zero->getPlayersAmount());
        level_zero_final->setPlayersAmountSubTree(group_tree2_level_zero->getPlayersAmountSubTree());
    }
    else if(group_tree1_level_zero != nullptr && group_tree2_level_zero != nullptr)
    {
        for(int i = 1; i<= group_tree1_level_zero->getScale(); i++)
        {
            level_zero_final->getScoreArr()[i] = group_tree1_level_zero->getScoreArr()[i] + group_tree2_level_zero->getScoreArr()[i];
            level_zero_final->getScoreArrSubTree()[i] = group_tree1_level_zero->getScoreArrSubTree()[i] + group_tree2_level_zero->getScoreArrSubTree()[i];
        }
        level_zero_final->setPlayersAmount(group_tree1_level_zero->getPlayersAmount() + group_tree2_level_zero->getPlayersAmount());
        level_zero_final->setPlayersAmountSubTree(group_tree1_level_zero->getPlayersAmountSubTree() + group_tree2_level_zero->getPlayersAmountSubTree());
    }
}

static double getAverageHighestLevel(RankNode<int>* iter_node, int m)
{
    int temp_size = m;
    double sum_total = 0;
    bool found_node = false;
    if(iter_node->getPlayersAmountSubTree() <= m)
    {
        sum_total = iter_node->getLevelsSumSubTree();
        return sum_total/m;
    }
    while(temp_size > 0 && iter_node != nullptr)
    {
        if(!found_node)
        {
            if(iter_node->getRightSon() == nullptr && temp_size == iter_node->getPlayersAmount())
            {
                sum_total += iter_node->getLevelSum();
                temp_size = 0;
                return sum_total/m;
            }
            if(temp_size == iter_node->getPlayersAmountSubTree())
            {
                sum_total += iter_node->getLevelsSumSubTree();
                temp_size = 0;
                return sum_total/m;
            }
            else if(temp_size < iter_node->getPlayersAmountSubTree())
            {
                if(iter_node->getRightSon() == nullptr)
                {
                    found_node = true;
                }
                else iter_node = iter_node->getRightSon();
            }
            else if(temp_size > iter_node->getPlayersAmountSubTree())
            {
                sum_total += iter_node->getLevelsSumSubTree();
                temp_size -= iter_node->getPlayersAmountSubTree();
                iter_node = iter_node->getParent();
                found_node = true;
            }
        }
        else if(found_node && iter_node !=nullptr)
        {
            if(iter_node->getPlayersAmount() >= temp_size)
            {
                sum_total += temp_size * *(iter_node->getKey());
                temp_size = 0;
                return sum_total/m;
            
            }
            else // if playeramount < temp_size
            {
                sum_total += iter_node->getPlayersAmount() * *(iter_node->getKey());
                temp_size -= iter_node->getPlayersAmount();
                iter_node = iter_node->getLeftSon();
                found_node = false;
            }
        }
    }
    return sum_total/m;
}