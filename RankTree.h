#ifndef RANKTREE
#define RANKTREE

#include <cmath>
#include "RankNode.h"
#include "exceptions.h"



template<class T> 
class RankTree {	
	RankNode<T>* root;
    RankNode<T>* level_zero;
	int 		scale;
	int 		root_size;
	public:
		RankTree(int scale);
		RankTree(RankNode<T>* root, RankNode<T>* level_zero, int scale, int root_size);
		~RankTree();
		void insert(T key, int score);
		void remove(T key, int score);
		void rankMin(int level, int score, int* total_players, int* players_in_score);
		void rankMax(int level, int score, int* total_players, int* players_in_score);		
		RankNode<T>* search(T& key);	
		RankNode<T>* getRootNode();
		RankNode<T>* getLevelZero();
		RankNode<T>* getMaxNode();
		RankTree<T>*  copyTree();
		int getRootSize();
		void setRootNode(RankNode<T>* new_root);
		bool isEmpty();
		void inOrder();	 
		void inOrderReverse();
		void clearTree();
		int getScale() const;
		class Empty {};
		class NodeAlreadyExists {};
		class DoesntExist {};
};

template<class T>
RankTree<T>::RankTree(int scale) : scale(scale), root(nullptr), level_zero(nullptr){ 
	this->root_size = 0; 
	}

template<class T>
RankTree<T>::RankTree(RankNode<T>* root, RankNode<T>* level_zero, int scale, int root_size) : root(root), level_zero(level_zero), scale(scale), root_size(root_size){}

template<class T>
RankTree<T>::~RankTree()
{
	if(this->root != nullptr){
		root->clearAll();
		delete root;
		root = nullptr;
	}
	if(this->level_zero != nullptr)
	{
		level_zero->clearAll();
		delete level_zero;
		level_zero = nullptr;
	}
	this->scale = 0;
}

template<class T>
bool RankTree<T>::isEmpty(){
	if(this->level_zero == nullptr && this->root == nullptr) return true;
	return false;
}

template<class T>
RankNode<T>* RankTree<T>::getRootNode(){
	return this->root;
}


template<class T>
RankTree<T>* RankTree<T>::copyTree(){ //TODO need fucking check the copy function is ugly as HELL!
	return new RankTree<T>(this->root->copyAll(this->root, nullptr),this->level_zero->copyAll(this->level_zero, nullptr), this->scale, this->root_size);
}

/**
 * @brief recieves a data and stores it inside a node in the tree.
 * @tparam the type of the oobject being insert must have operators: ==, <, >
 * @throw NodeExist - if there's a node with the same key
 * 			std::bad_alloc& exception - out of memory
 * @param level - the object we wish to store. 
 */

template<class T>
void RankTree<T>::insert(T level, int score){
    T* temp_add = nullptr;
    temp_add = new T(level);
    if(level == 0)
    {
        if(this->level_zero == nullptr) //need to create new level zero node
        {
            try
            {
                this->level_zero = new RankNode<T>(temp_add,nullptr, score, this->scale);
            } catch (std::bad_alloc& e) {	// this block is not futile.
			    delete temp_add;
			    throw e; // throw std::bad_alloc
			}
		}
        else // the levelzero node exist!
        {
            this->level_zero->updateNodeAtInsert(score);
        }
    }
	else
	{
		if(this->root == nullptr)
		{
			try
			{
				this->root = new RankNode<T>(temp_add,nullptr, score, this->scale);
				this->root_size++;
            }catch (std::bad_alloc& e) {	// this block is not futile.
				delete temp_add;
				throw e; // throw std::bad_alloc
			}
				
			return;
		}
		RankNode<T>* node_level = this->root->search(temp_add);
		if(node_level != nullptr) //there is a node of this level
		{
			this->root->searchAndUpdateAtInsert(temp_add, score);
			node_level->updateLevelSumSubTreesAtInsert();
		}
		else //there is no node like this yet
		{
			this->root = this->root->insert(temp_add, score, this->getScale());
			this->root_size++;
			RankNode<T>* node_inserted = this->root->search(temp_add);
			node_inserted->updateLevelSumSubTreesAtInsert();
			//this->root->searchAndUpdateAtInsert(temp_add, score);
			//RankNode<T>* node_added = this->root->search(temp_add);
			//node_added->updateNodeAtRemove(score); //imdoing it because insert and search updating him twice(insert at the constructor)
		}
		this->root->updateNodeFeatures();
		this->root->updateLevelSumSubTree();
	}
}

/**
 * @brief searches for a given key in the tree
 * @throw Empty if the tree is empty
 * 		std::bad_alloc& - out of memory
		NodeDosentExist - if there no node with the given key. 
 * @param key - the searches key.
 * @return RankNode<T>* - returns a pointer to the node containing the the key. 
 */
template<class T>
RankNode<T>* RankTree<T>::search(T& key){
	if(key == 0)
	{
		if(this->level_zero ==nullptr) throw NodeDosentExist();
		return this->level_zero;
	}
	else
	{
		if(this->root == nullptr) throw NodeDosentExist();
		RankNode<T>* ret_node = this->root->search(&key);
		if(ret_node == nullptr)
		{
			throw NodeDosentExist();
		}
		else return ret_node;
	}
	
}

/**
 * @brief removes the node with the given key from the tree
 * @throw Empty() if the tree is empty
 * @throw DoesntExist if the node with the given key doesnot exist
 * @param key - the key to remove. 
 */
template<class T>
void RankTree<T>::remove(T key, int score){
	if(this->search(key) == nullptr) throw DoesntExist();
	T* temp_key = new T(key);
	if(key == 0) 
	{
		if(this->level_zero->getPlayersAmount() == 1)
		{
			this->level_zero = this->level_zero->remove(temp_key);
			delete temp_key;
		}
		else
		{
			this->level_zero->updateNodeAtRemove(score);
		}
	}
	else
	{
		RankNode<T>* node_to_remove = this->root->search(temp_key);
		if(node_to_remove->getPlayersAmount() == 1)
		{
			this->root->searchAndUpdateAtRemove(temp_key, score);
			this->root = this->root->remove(temp_key);
			this->root_size--;
		}
		else
		{
			this->root->searchAndUpdateAtRemove(temp_key, score);
			//node_to_remove->updateLevelSumSubTrees();
		}
		
		this->root->updateNodeFeatures();
		this->root->updateLevelSumSubTree(); 
	}
}

template<class T>
void RankTree<T>::rankMin(int level, int score, int* total_players, int* players_in_score) //TODO is it problem to work with pointers? check level_min!
{
	
	RankNode<T>* level_node = this->root->searchMin(&level);
	if(level_node == nullptr) return;
	int level_min = *(level_node->getKey());
	this->root->rank(level_node, level_min, score, total_players, players_in_score);
	int add_player_amount = level_node->getPlayersAmount();
	int add_player_score = (level_node->getScoreArr())[score];
	*(total_players) = *(total_players) - add_player_amount;
	*(players_in_score) = *(players_in_score) - add_player_score;
}

template<class T>
void RankTree<T>::rankMax(int level, int score, int* total_players, int* players_in_score) //TODO is it problem to work with pointers? check level_min!
{
	RankNode<T>* level_node = this->root->searchMax(&level);
	if(level_node == nullptr) return;
	int level_min = *(level_node->getKey());
	this->root->rank(level_node, level_min, score, total_players, players_in_score);
}

/**
 * @brief inOrder print to the stdout
 * 
 * @param root - the root of the tree. 
 */
template<class T>
void RankTree<T>::inOrder(){
	if (this->root == nullptr) {
		return;
	}
	this->root->inOrder();
	
} 

template<class T>
void RankTree<T>::inOrderReverse(){
	if (this->root == nullptr) {
		return;	
	}
	this->root->inOrderReverse();
	
} 

/**
 * @brief removes all nodes from the tree
 * 
 * @tparam T the type of the data stored in the nodes.
 */
template<class T>
void RankTree<T>::clearTree(){
	if (this->root != nullptr) {
		this->root->clearAll();
		delete this->root;
		this->root =nullptr;
	}
	
	if (this->level_zero != nullptr) {
		this->level_zero->clearAll();
		delete this->level_zero;
		this->level_zero =nullptr;
	}
	
} 

template <class T>
int RankTree<T>::getScale() const{
	return this->scale;
}

template <class T>
void RankTree<T>::setRootNode(RankNode<T>* new_root){
	this->root = new_root;
}

template <class T>
RankNode<T>* RankTree<T>::getMaxNode()
{
	return this->root->getMaxNode();
}

template <class T>
RankNode<T>* RankTree<T>::getLevelZero()
{
	return this->level_zero;
}

template <class T>
int RankTree<T>::getRootSize()
{
	return this->root_size;
}


#endif