#include "ChainHashTable.h"


/* C'tors ----------------------------------------------------------------------------------------------------------- */

ChainHashTable::ChainHashTable(int table_size_t)
{
    table = new PlayerNode*[table_size_t];      // default size is 10.
    this->table_size=table_size_t;
    this->capacity = INITIAL_CAPACITY;
    for(int i=0; i < this->table_size ; i++){
        table[i] = nullptr;
    }
}

ChainHashTable::~ChainHashTable(){
    for(int i=0; i < this->table_size ; i++){
        table[i]->clearChain();
        //delete table[i];
        table[i] = nullptr;     // setting the deleted to nullptr is standard practive
    }
    //delete[] *table;
    if(this->table != nullptr)
        delete[] table;
    this->table=nullptr;        // setting the deleted to nullptr is standard practive
}

PlayerNode** ChainHashTable::getTable(){
    return this->table;
}



/* Auxiliary private functions -------------------------------------------------------------------------------------- */

int ChainHashTable::calculateHash(int key) const{
    return key % table_size;
}

bool ChainHashTable::existsAt(int index, int key) const{
    PlayerNode* iterator = this->table[index];
    if(iterator == nullptr){
        return false;
    }
    while(iterator != nullptr){
        if(iterator->getPlayer()->getPlayerID() == key){
            return true;    // found.
        }
        iterator = iterator->getNext();
    }
    return false; // was not found.
}

void ChainHashTable::upsize(){
    //prolog
    int new_size = this->table_size * GROW_FACTOR;
    int old_size = this->table_size;
    PlayerNode** new_table = new PlayerNode*[new_size];
    PlayerNode** old_table = this->table;
    PlayerNode* iterator = nullptr;
    //int new_HT_index;
    this->capacity = 0; // we insert the old values and that will grow the capacity to it's former size. 
    //focues on the new table content
    this->table = new_table;
    this->table_size = new_size;
    // resetting up the new array
    for(int i=0; i < new_size; i++){
        this->table[i] = nullptr;
    }
    // settling the old cells
    for(int i=0; i < old_size; i++){
        iterator=old_table[i];      // takes the head of the chain
        while(iterator != nullptr){
            //new_HT_index = calculateHash(iterator->getPlayer()->getPlayerID());
            this->insertHT(iterator->getPlayer());      // assure that the new table is already updated for hash index calculations 
            iterator=iterator->getNext();
        }
    }
    for(int i=0; i < old_size ; i++){
        delete old_table[i];
        old_table[i] = nullptr;   
    }
    delete[] old_table;
    return;
} 


/* Auxliary public functions ---------------------------------------------------------------------------------------- */

int ChainHashTable::getSize() const {
    return this->table_size;
}

/* Primary Hash Table functions ------------------------------------------------------------------------------------- */

/**
 * @brief insert a new player into the table. a WHOLE new copy is inserted into the table.

 * 
 * @param new_player 
 * @return HT_status HT_ALREADY_EXIST - if matching player ID is already inside the table
            HT_SUCCESS
 */
HT_status ChainHashTable::insertHT(Player* new_player){
    int HT_index = calculateHash(new_player->getPlayerID());
    int new_player_key = new_player->getPlayerID();
    if (existsAt(HT_index, new_player_key)){
        return HT_ALREADY_EXISTS; // do nothing
    }
    PlayerNode* new_node = new PlayerNode(new_player);
    if(table[HT_index] == nullptr){ // no nodes in the cell
        table[HT_index] = new_node;
    } else {    
        // perform a head insert
        PlayerNode* current_first = table[HT_index];
        new_node->setNext(current_first);
        table[HT_index] = new_node;
    }
    capacity++;
    int overload_factor = this->table_size * GROW_FACTOR;   // this is the only trigger for upsizing.
    if(capacity == overload_factor){
        this->upsize();
    }
    return HT_SUCCESS;    
}

/**
 * @brief deletes the player from the table. in case the list empties from that cell we insert nullptr.
 * 
 * @param key - deletes the player with the @key ID.
 * @return HT_status HT_DOESNT_EXIST - if the player doesnt exist.
            HT_SUCCESS - success.
 */
HT_status ChainHashTable::deleteHT(int key){
    int HT_index = calculateHash(key);
    if( existsAt(HT_index, key) == false ){
        return HT_DOESNT_EXIST;     // do nothing, target does not exist in the table
    } else {
        PlayerNode* iterator = table[HT_index];
        PlayerNode* prev_iterator = nullptr;
        while(iterator != nullptr){
            if(iterator->getPlayer()->getPlayerID() == key){
                if(prev_iterator == nullptr){   // the first node is the target
                    table[HT_index] = iterator->getNext();  // update the pointers
                    delete iterator;    // delete the node that is pointed by the iterator
                    } else {
                    prev_iterator->setNext(iterator->getNext());
                    delete iterator;
                }
                capacity--;
                return HT_SUCCESS;
            }
            prev_iterator = iterator;
            iterator = iterator->getNext();
        }
    }
    return HT_SUCCESS;  // we shall never get here, this is here just for the compiler to shut up
}

/**
 * @brief recieves a key and returns a pointer to the player with the given key if found.
 *      in case the player doesnt exist in the table, returns null ptr
 * 
 * @param key 
 * @return Player*   
 */
Player* ChainHashTable::findHT(int key) const{
    int HT_index = calculateHash(key);
    if( existsAt(HT_index, key) == false ){
        return nullptr;     //target does not exist in the table
    } else {
        PlayerNode* iterator = table[HT_index];
        while(iterator != nullptr){
            if(iterator->getPlayer()->getPlayerID() == key){ return iterator->getPlayer(); }
            iterator=iterator->getNext();
        }
    }
    
    return nullptr; // we shall never get here, this is just for the compiler to shut up
}

