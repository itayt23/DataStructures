#ifndef PLAYERSMANAGER
#define PLAYERSMANAGER

#include "ChainHashTable.h"
#include "UnionFind.h"
#include "RankTree.h"
#include "Group.h"
#include "library2.h"

const int INITIAL_TOTAL_PLAYERS = 0;

class PlayersManager{
        ChainHashTable* players_table;
        UnionFind<Group>* groups_uf;         // <Player>  or <PlayerNode>   note: my PlayerNode with data and next only. 
        RankTree<int>* all_players_tree;
        int total_players;
        int groups_size;
        int score_scale;

    public:
    /* C'tors & D'tors */
        PlayersManager(int k, int scale);
        ~PlayersManager();
        PlayersManager(const PlayersManager& p) = delete; // we wouldn't want a second DS holding our data.
    /* Meow DS functions */
    /* Primary DS functions */
        StatusType MergeGroups(int GroupID1, int GroupID2);
        StatusType AddPlayer(int PlayerID, int GroupID, int score);
        StatusType RemovePlayer(int PlayerID);
        StatusType IncreasePlayerIDLevel(int PlayerID, int LevelIncrease);
        StatusType ChangePlayerIDScore(int PlayerID, int NewScore);
        StatusType GetPercentOfPlayersWithScoreInBounds(int GroupID, int score, int lowerLevel, int higherLevel, 
                                                        double * players);
        StatusType AverageHighestPlayerLevelByGroup(int GroupID, int m, double * level);
        StatusType GetPlayersBound(int GroupID, int score, int m, int* LowerBoundPlayers, int* HigherBoundPlayers);
        void Quit();


};






#endif