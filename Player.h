#ifndef PLAYER
#define PLAYER

typedef enum { ACTIVE = true, INACTIVE = false } ActivityStatus;
const int INITIAL_LEVEL = 0;

class Player
{   
    private:
        int player_id;
        int level;
        int score;
        int player_groupID;
	public:
        Player(); 
    	Player(int player_id, int GroupID, int score, int level = INITIAL_LEVEL);
		~Player();  
        Player(const Player& to_copy); 

        /* Get funcitons */
        int getPlayerID() const;
        int getPlayerLevel() const;
        int getPlayerScore() const;
        int getPlayerGroupID() const;

        /* Set functions */
        void setPlayerLevel(int new_level);
        void setPlayerScore(int new_score);

        /* Auxiliary functions */
        bool SameID(Player& p1);
        bool differentID(Player& p1);

        /* Operators */
        //! NOTE: The sorting is done by the level. 
        bool operator== (const Player& player) const;
        bool operator> (const Player& player) const;
        bool operator< (const Player& player) const;
        Player& operator= (const Player& player);
        bool operator!=(const Player& player);

	};
    //std::ostream& operator<<(std::ostream& os, const Player& player);

#endif