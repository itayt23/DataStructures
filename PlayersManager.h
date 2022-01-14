#ifndef PLAYERSMANAGER
#define PLAYERSMANAGER

#include "ChainHashTable.h"
#include "UnionFind.h"
#include "RankTree.h"
#include "Group.h"
#include "library2.h"

const int INITIAL_TOTAL_PLAYERS = 0;

class PlayersManager{
        ChainHashTable* players_table;      // hash table, using the chaining method
        UnionFind<Group>* groups_uf;        // inion find for the groups
        RankTree<int>* all_players_tree;    // ranked tree (with add. information) of all the active players
        int total_players;                  // number of active players in the game
        int groups_size;                    // number of groups, is set on startup
        int score_scale;                    // score scale, set on startup

    public:
    /* C'tors & D'tors */
        PlayersManager(int k, int scale);
        ~PlayersManager();
        PlayersManager(const PlayersManager& p) = delete;
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


};






#endif