#ifndef PLAYER
#define PLAYER

typedef enum { ACTIVE = true, INACTIVE = false } ActivityStatus;
const int INITIAL_LEVEL = 0;

class Player
{   
    private:
        int player_id;          // the player's unique ID
        int level;              // the player's level
        int score;              // the player's score
        int player_groupID;     // the player's affiliated group
	public:
        Player(); 
    	Player(int player_id, int GroupID, int score, int level = INITIAL_LEVEL);
		~Player() = default;  
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
        bool operator== (const Player& player) const;
        bool operator> (const Player& player) const;
        bool operator< (const Player& player) const;
        Player& operator= (const Player& player);
        bool operator!=(const Player& player);

	};

#endif