
#include "PlayerNode.h"

const int DEFAULT_SIZE = 10;
const int INITIAL_CAPACITY = 0;
const int GROW_SHRINK_FACTOR = 2;

typedef enum { HT_ALREADY_EXISTS, HT_DOESNT_EXIST, HT_SUCCESS} HT_status;


class ChainHashTable
{
private:
     PlayerNode** table;
     int table_size;
     int capacity;  

/* Auxiliary private functionse */
    int calculateHash(int key) const;
    bool alreadyExistsInList(int index, int key) const;
    bool existsAt(int index, int key) const;
    void upsize();
    void downsize();
    
public:
    ChainHashTable(int table_size_t = DEFAULT_SIZE);
    ~ChainHashTable();
    PlayerNode** getTable();

/* Auxiliary public functions */
int getSize() const;

/* Primary Hash Table functions */
    HT_status insertHT(Player* new_player);
    HT_status deleteHT(int key);
    Player* findHT(int key) const;

};










