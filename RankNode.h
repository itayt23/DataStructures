#ifndef RANKNODE
#define RANKNODE

#include <assert.h>
#include <iostream>


const int EMPTY = 0;
const int EMPTY_HEIGHT = 0;
const int LEAF_HEIGHT = 0;
const int L_ROTATE = 2;
const int R_ROTATE = -2;

template<class T> 
class RankNode{
	T *key;			// the key is the level
	int height;
	int players_amount;
	int scale;
    int players_amount_sub_tree;			// including the node itself
	int* players_score_arr;					
    int* players_score_arr_sub_tree;
	int level_sum;
	int levels_sum_sub_tree;
	RankNode* parent;
	RankNode* left;
	RankNode* right;
	RankNode<T>* lrRotate();
	RankNode<T>* llRotate();
	RankNode<T>* rlRotate();
	RankNode<T>* rrRotate();
	
	RankNode<T>* balanceTree();		


	public:
	/* C'tors % D'tors */
		RankNode(T* key, RankNode<T>* parent, int score, int scale);
		RankNode(T* key, RankNode<T>* parent, int height, int players_amount, int players_amount_sub_tree, int* players_score_arr,
		int* players_score_arr_sub_tree, int scale, int level_sum, int levels_sum_sub_tree);
		~RankNode();

	/* aux functions */
		bool isTheLeftSon();
		bool isTheRightSon();
		bool hasLeftSon();
		bool hasRightSon();
		bool hasOnlyOneSon();
		bool hasTwoSon();
		void rank(RankNode<T>* level_node, int level, int score, int* total_players, int* players_in_score);
		void clearAll();				
		void updateHeight();
		void updateNodeFeaturesAtInsert(int score); 
		void updateNodeFeatures();
		void updateLLFeatures();
		void updateRRFeatures();
		void updateLevelSumSubTreesAtInsert();
		void updateLevelSumSubTree();
		void updateNodeAtInsert(int score);
		void updateNodeFeaturesAtRemove(int score, T* key_find); 	
		void updateNodeAtRemove(int score); 	 	
		int max(int a, int b);
		void inOrder();
		void inOrderReverse();
		bool isLeaf();
		int* copyScoreArr();
		int* copyScoreArrSubTree();
		RankNode<T>* insert(T* key_add, int score, int scale);	
    	RankNode<T>* remove(T* key_remove);
		RankNode<T>* search(T* key_find);
		RankNode<T>* searchMin(T* key_find);
		RankNode<T>* searchMax(T* key_find);
		void searchAndUpdateAtInsert(T* key_find, int score);
		void searchAndUpdateAtRemove(T* key_find, int score);
		//void updateAtInsert(T* key_find, int score);
		T* getKey();
		RankNode<T>* removeNode(); 
		RankNode<T>* copyAll(RankNode<T>* node, RankNode<T>* parent);
		RankNode<T>* copyNode();

		/* Set Functions */
		void setLeftSon(RankNode<T>* new_son);
		void setRightSon(RankNode<T>* new_son);
		void setParent(RankNode<T>* new_parent);
		void setHeight(int h);
		void setKey(T* new_key);
		void setScoreArrSubTree(int* score_arr);
		void setScoreArr(int* score_arr);
		void setPlayersAmountSubTree(int players_amount);
		void setPlayersAmount(int players_amount);
		void setNodeLevelSum(int level_sum);
		void setLevelSumSubTree(int level_sum_sub);

		/* Get functions */
		int getHeight() const;			
		int getBalanceFactor();
		int getLeftHeight() const;
		int getRightHeight() const;
		RankNode<T>* getLeftSon();
		RankNode<T>* getRightSon();
		RankNode<T>* getParent();
		int getPlayersAmount();
		int getPlayersAmountSubTree();
		int* getScoreArr();
		int* getScoreArrSubTree();
		int getScale();
		int getLevelSum();
		int getLevelsSumSubTree();
		RankNode<T>* getMinNode();
		RankNode<T>* getMaxNode();
};

template<class T> 
RankNode<T>::RankNode(T* key, RankNode<T>* parent, int score, int scale) : key(key), parent(parent)
{
	this->height = EMPTY_HEIGHT;
	this->left = nullptr;
	this->right = nullptr;
    this->players_amount = 1;
    this->players_amount_sub_tree = 1;
	this->scale = scale;
	this->players_score_arr = new int[scale+1];
    this->players_score_arr_sub_tree = new int[scale+1];
    for(int i =1; i<=scale; i++)
    {
        this->players_score_arr[i] = 0;				// score=0 is not a valid starting score therefore we'll use it as a blank
        this->players_score_arr_sub_tree[i] = 0;	// score=0 is not a valid starting score therefore we'll use it as a blank
    }
	if(score != EMPTY)
	{
		this->players_score_arr[score]++;	
    	this->players_score_arr_sub_tree[score]++;
	}
	if(key != nullptr)
	{
		this->level_sum = *(key) * this->players_amount;
		this->levels_sum_sub_tree = this->level_sum;
	}
	else
	{
		this->level_sum = 0;
		this->levels_sum_sub_tree = 0;
	}
	
}

template<class T> 
RankNode<T>::RankNode(T* key, RankNode<T>* parent, int height, int players_amount, int players_amount_sub_tree,
int* players_score_arr, int* players_score_arr_sub_tree, int scale, int level_sum, int levels_sum_sub_tree):key(key), parent(parent)
{
	this->height = height;
	this->left = nullptr;
	this->right = nullptr;
    this->players_amount = players_amount;
    this->players_amount_sub_tree = players_amount_sub_tree;
	this->scale = scale;
	this->players_score_arr = new int[this->getScale()+1];
	this->players_score_arr_sub_tree = new int[this->getScale()+1];
	for(int i=1;i<=scale;i++)
	{
		this->players_score_arr[i] = players_score_arr[i];
		this->players_score_arr_sub_tree[i] = players_score_arr_sub_tree[i];
	}
	this->level_sum = level_sum;
	this->levels_sum_sub_tree = levels_sum_sub_tree;
}

template<class T> 
RankNode<T>::~RankNode()
{
	if(this->key != nullptr){
		delete this->key;
		this->key = nullptr;
	}
	if(this->players_score_arr != nullptr)
	{
		delete[] this->players_score_arr;
		this->players_score_arr = nullptr;
	}
	if(this->players_score_arr_sub_tree != nullptr)
	{
		delete[] this->players_score_arr_sub_tree;
		this->players_score_arr_sub_tree = nullptr;
	}
	this->left = nullptr;
	this->right = nullptr;
	this->parent = nullptr;
}


/*
 finds out is 'this' node is a left son to a node
	true - if the parent has a left pointer to me
	false - if the parent has no left pointer to me or 'this' is the root
 */
template<class T> 
bool RankNode<T>::isTheLeftSon(){
	if(this->parent == nullptr ){
		return false; 
	} else if(this->parent->left != nullptr && this->parent->left->getKey() == this->getKey()){	//
		return true;
	} else return false; 
}


/* finds out is 'this' node is a right son to a node
	true - if the parent has a right pointer to me
	false - if the parent has no right pointer to me or 'this' is the root
 */
template<class T> 
bool RankNode<T>::isTheRightSon()
{
	if(this->parent == nullptr )
	{
		return false; 
	}
	else if(this->parent->right != nullptr && this->parent->right->getKey() == this->getKey())
	{
		return true;
	}
	else return false; 
}


/* Checks if the node a left son
true - if it does
false - does not
 */
template<class T> 
bool RankNode<T>::hasLeftSon()
{
	return this->left ? true : false; 
}

/* Checks if the node a right son
true - if it does
false - does not
 */
template<class T> 
bool RankNode<T>::hasRightSon()
{
	return this->right ? true : false; 
}

template<class T> 
bool RankNode<T>::hasOnlyOneSon()
{
	return ((this->hasLeftSon() == true && this->hasRightSon() == false) || (this->hasLeftSon() == false && this->hasRightSon() == true));
}

template<class T> 
bool RankNode<T>::hasTwoSon(){
	return (this->hasLeftSon() == true && this->hasRightSon() == true);
}

template<class T> 
void RankNode<T>::clearAll()	{

	if(this->left != nullptr){
		this->left->clearAll();
		delete left;
		left = nullptr;
	}
	if(this->players_score_arr != nullptr)
	{
		delete[] this->players_score_arr;
		this->players_score_arr = nullptr;
	}
	if(this->players_score_arr_sub_tree != nullptr)
	{
		delete[] this->players_score_arr_sub_tree;
		this->players_score_arr_sub_tree = nullptr;
	}
	if(this->right != nullptr){
		this->right->clearAll();
		delete right;
		right = nullptr;
	}
}

template<class T> 
RankNode<T>* RankNode<T>::copyAll(RankNode<T>* root, RankNode<T>* parent)
{
	if(root == nullptr) return nullptr;
	T* key = new T(*(root->getKey()));
	RankNode<T>* node = new RankNode<T>(key, parent, root->getHeight(), root->getPlayersAmount(), root->getPlayersAmountSubTree(),
	root->getScoreArr(), root->getScoreArrSubTree(), root->getScale(), root->getLevelSum(), root->getLevelsSumSubTree());
	node->setLeftSon(copyAll(root->getLeftSon(), node));
	node->setRightSon(copyAll(root->getRightSon(), node));
	return node;	
}

template<class T> 
RankNode<T>* RankNode<T>::copyNode()
{
	T* key = new T(*(this->getKey()));
	RankNode<T>* copy_node = new RankNode<T>(key, this->getParent(), this->getHeight(), this->getPlayersAmount(), this->getPlayersAmountSubTree(),
	this->getScoreArr(), this->getScoreArrSubTree(), this->getScale(), this->getLevelSum(), this->getLevelsSumSubTree());
	copy_node->setLeftSon(nullptr);
	copy_node->setRightSon(nullptr);
	return copy_node;	
}

template<class T> 
int RankNode<T>::getHeight() const {
	return this->height;
}

template<class T>
T* RankNode<T>::getKey() {
	return this->key;
}

template<class T>
int RankNode<T>::getLeftHeight() const {
	if(this->left == nullptr){
		return EMPTY_HEIGHT;
	}
	return this->left->getHeight() + 1;
}

template<class T>
int RankNode<T>::getRightHeight() const {
	if(this->right == nullptr){
		return EMPTY_HEIGHT;
	}
	return this->right->getHeight() + 1;
}

template<class T>
int RankNode<T>::getBalanceFactor(){
	this->updateHeight();
	return (this->getLeftHeight() - this->getRightHeight());
}

/* Updates the height of the sub-tree passed by this. Does not update the whole tree */
template<class T>
void RankNode<T>::updateHeight() {
	this->height = max(this->getLeftHeight(),this->getRightHeight()); 
}

template<class T>
void RankNode<T>::updateLevelSumSubTreesAtInsert() {
	RankNode<T>* iter_node = this;
	while(iter_node != nullptr)
	{
		if(iter_node->isLeaf())
		{
			iter_node->levels_sum_sub_tree = iter_node->getLevelSum();
		}
		else if(iter_node->hasRightSon() && !(iter_node->hasLeftSon()))
		{
			iter_node->levels_sum_sub_tree = iter_node->right->getLevelsSumSubTree() + iter_node->getLevelSum();
		}
		else if(!(iter_node->hasRightSon()) && iter_node->hasLeftSon())
		{
			iter_node->levels_sum_sub_tree = iter_node->left->getLevelsSumSubTree() + iter_node->getLevelSum();
		}
		else if(iter_node->hasRightSon() && iter_node->hasLeftSon())
		{
			iter_node->levels_sum_sub_tree = iter_node->left->getLevelsSumSubTree() + iter_node->right->getLevelsSumSubTree() + iter_node->getLevelSum();
		}
		iter_node = iter_node->getParent();
	}
}

template<class T>
void RankNode<T>::updateLevelSumSubTree() {
	RankNode<T>* temp = this;
	if(temp == nullptr) return;
	if(this->isLeaf())
	{
		this->levels_sum_sub_tree = this->level_sum;
	}
	else if(this->hasRightSon() && !(this->hasLeftSon()))
	{
		this->levels_sum_sub_tree = this->right->getLevelsSumSubTree() + this->level_sum;
	}
	else if(!(this->hasRightSon()) && this->hasLeftSon())
	{
		this->levels_sum_sub_tree = this->left->getLevelsSumSubTree() + this->level_sum;
	}
	else if(this->hasRightSon() && this->hasLeftSon())
	{
		this->levels_sum_sub_tree = this->left->getLevelsSumSubTree() + this->right->getLevelsSumSubTree() + this->level_sum;
	}
}

template<class T>
void RankNode<T>::updateNodeFeatures() {
	RankNode<T>* temp = this;
	if(temp == nullptr) return;
	if(this->isLeaf())
	{
		for(int i=1; i<=this->getScale(); i++)
    	{
        	this->getScoreArrSubTree()[i] = this->getScoreArr()[i];
    	}
		//this->setScoreArrSubTree(this->copyScoreArr());
		this->setPlayersAmountSubTree(this->getPlayersAmount());
	}
	else if(this->hasRightSon() && !(this->hasLeftSon()))
	{
		for(int i=1; i<=this->getScale(); i++)
    	{
        	this->getScoreArrSubTree()[i] = this->getRightSon()->getScoreArrSubTree()[i] + this->getScoreArr()[i];
    	}
		this->setPlayersAmountSubTree(this->getRightSon()->getPlayersAmountSubTree() + this->getPlayersAmount()); 
	}
	else if(!(this->hasRightSon()) && this->hasLeftSon())
	{
		for(int i=1; i<=this->getScale(); i++)
    	{
        	this->getScoreArrSubTree()[i] = this->getLeftSon()->getScoreArrSubTree()[i] + this->getScoreArr()[i];
    	}
		this->setPlayersAmountSubTree(this->getLeftSon()->getPlayersAmountSubTree() + this->getPlayersAmount()); 
	}
	else if(this->hasRightSon() && this->hasLeftSon())
	{
		for(int i=1; i<=this->getScale(); i++)
    	{
        	this->getScoreArrSubTree()[i] = this->getLeftSon()->getScoreArrSubTree()[i] + this->getRightSon()->getScoreArrSubTree()[i] + this->getScoreArr()[i];
    	}
		this->setPlayersAmountSubTree(this->getLeftSon()->getPlayersAmountSubTree() + this->getRightSon()->getPlayersAmountSubTree() + this->getPlayersAmount()); 
	}
}

template<class T>
void RankNode<T>::updateNodeFeaturesAtInsert(int score) {
	this->players_score_arr_sub_tree[score]++;
	this->players_amount_sub_tree++;
}

template<class T>
void RankNode<T>::updateNodeAtInsert(int score) {
	this->players_score_arr[score]++;
	this->players_score_arr_sub_tree[score]++;
	this->players_amount++;
	this->players_amount_sub_tree++;
	this->level_sum = *(this->getKey()) * this->players_amount;
	this->updateLevelSumSubTree();
}

template<class T>
void RankNode<T>::updateNodeFeaturesAtRemove(int score, T* key_find) {
	this->players_score_arr_sub_tree[score]--;
	this->players_amount_sub_tree--;
	this->levels_sum_sub_tree = this->levels_sum_sub_tree - *(key_find);
}

template<class T>
void RankNode<T>::updateNodeAtRemove(int score) {
	this->players_score_arr[score]--;
	this->players_score_arr_sub_tree[score]--;
	this->players_amount--;
	this->players_amount_sub_tree--;
	this->level_sum = *this->getKey() * this->players_amount;
	this->updateLevelSumSubTree();
}

template<class T>
void RankNode<T>::updateLLFeatures() {
	int* A_score_arr_sub = this->getScoreArrSubTree();
	int* B_score_arr_sub = this->right->getScoreArrSubTree();
	int* B_score_arr = this->right->getScoreArr();
	int* Ar_score_arr_sub = nullptr;
	int Ar_players_amount_sub = 0;
	int Br_players_amount_sub = 0;
	int* Br_score_arr_sub = nullptr;
	if(this->right->left != nullptr)
	{
		Ar_score_arr_sub = this->right->left->getScoreArrSubTree();
		Ar_players_amount_sub = this->right->left->getPlayersAmountSubTree();
	}
	if(this->right->right != nullptr)
	{
		Br_players_amount_sub = this->right->right->getPlayersAmountSubTree();
		Br_score_arr_sub = this->right->right->getScoreArrSubTree();
	} 
	//updating B node
	//*******************************************
	this->right->setPlayersAmountSubTree(Ar_players_amount_sub+Br_players_amount_sub+(this->right->getPlayersAmount()));
	if(this->right->isLeaf())
	{
		for(int i=1;i<=this->getScale();i++)
		{
			B_score_arr_sub[i] = B_score_arr[i];
		}
		//this->right->setScoreArrSubTree(this->right->copyScoreArr());
		this->right->setLevelSumSubTree(this->right->getLevelSum());
	}
	else if(this->right->hasRightSon() && !(this->right->hasLeftSon()))
	{
		for(int i=1;i<=this->getScale();i++)
		{
			B_score_arr_sub[i] = Br_score_arr_sub[i] + B_score_arr[i];
		}
		this->right->setLevelSumSubTree(this->right->right->getLevelsSumSubTree() + this->right->getLevelSum());
	}
	else if(!(this->right->hasRightSon()) && this->right->hasLeftSon())
	{
		for(int i=1;i<=this->getScale();i++)
		{
			B_score_arr_sub[i] = Ar_score_arr_sub[i] + B_score_arr[i];
		}
		this->right->setLevelSumSubTree(this->right->left->getLevelsSumSubTree() + this->right->getLevelSum());
	}
	else if(this->right->hasRightSon() && this->right->hasLeftSon())
	{
		for(int i=1;i<=this->getScale();i++)
		{
			B_score_arr_sub[i] = Ar_score_arr_sub[i] + Br_score_arr_sub[i] + B_score_arr[i];
		}
		this->right->setLevelSumSubTree(this->right->right->getLevelsSumSubTree() + this->right->left->getLevelsSumSubTree() + this->right->getLevelSum());
	}
	
	//*******************************************
	//updating A node
	//******************************************
	if(this->left != nullptr)
	{
		this->setPlayersAmountSubTree(this->left->getPlayersAmountSubTree()+this->right->getPlayersAmountSubTree()+this->getPlayersAmount());
		for(int i=1;i<=this->getScale();i++)
		{
			A_score_arr_sub[i] = B_score_arr_sub[i] + this->left->getScoreArrSubTree()[i] + this->getScoreArr()[i];
		}
		this->setLevelSumSubTree(this->right->getLevelsSumSubTree() + this->left->getLevelsSumSubTree() + this->getLevelSum());
		return;
	}
	else
	{
		this->setPlayersAmountSubTree(this->right->getPlayersAmountSubTree()+this->getPlayersAmount());
		for(int i=1;i<=this->getScale();i++)
		{
			A_score_arr_sub[i] = B_score_arr_sub[i] + this->getScoreArr()[i];
		}
		this->setLevelSumSubTree(this->right->getLevelsSumSubTree() + this->getLevelSum());
		return;
	}
	//*******************************************
}

template<class T>
void RankNode<T>::updateRRFeatures() {
	int* A_score_arr_sub = this->getScoreArrSubTree();
	int* B_score_arr_sub = this->left->getScoreArrSubTree();
	int* B_score_arr = this->left->getScoreArr();
	int* Al_score_arr_sub = nullptr;
	int Al_players_amount_sub = 0;
	int Br_players_amount_sub = 0;
	int* Br_score_arr_sub = nullptr;
	if(this->left->right != nullptr)
	{
		Al_score_arr_sub = this->left->right->getScoreArrSubTree();
		Al_players_amount_sub = this->left->right->getPlayersAmountSubTree();
	}
	if(this->left->left != nullptr)
	{
		Br_players_amount_sub = this->left->left->getPlayersAmountSubTree();
		Br_score_arr_sub = this->left->left->getScoreArrSubTree();
	} 
	//updating B node
	//*******************************************
	this->left->setPlayersAmountSubTree(Al_players_amount_sub+Br_players_amount_sub+(this->left->getPlayersAmount()));
	if(this->left->isLeaf())
	{
		for(int i=1;i<=this->getScale();i++)
		{
			B_score_arr_sub[i] = B_score_arr[i];
		}
		//this->left->setScoreArrSubTree(this->left->copyScoreArr());
		this->left->setLevelSumSubTree(this->left->getLevelSum());
	}
	else if(this->left->hasRightSon() && !(this->left->hasLeftSon()))
	{
		for(int i=1;i<=this->getScale();i++)
		{
			B_score_arr_sub[i] = Al_score_arr_sub[i] + B_score_arr[i];
		}
		this->left->setLevelSumSubTree(this->left->right->getLevelsSumSubTree() + this->left->getLevelSum());
	}
	else if(!(this->left->hasRightSon()) && this->left->hasLeftSon())
	{
		for(int i=1;i<=this->getScale();i++)
		{
			B_score_arr_sub[i] = Br_score_arr_sub[i] + B_score_arr[i];
		}
		this->left->setLevelSumSubTree(this->left->left->getLevelsSumSubTree() + this->left->getLevelSum());
	}
	else if(this->left->hasRightSon() && this->left->hasLeftSon())
	{
		for(int i=1;i<=this->getScale();i++)
		{
			B_score_arr_sub[i] = Al_score_arr_sub[i] + Br_score_arr_sub[i] + B_score_arr[i];
		}
		this->left->setLevelSumSubTree(this->left->left->getLevelsSumSubTree() + this->left->right->getLevelsSumSubTree() + this->left->getLevelSum());
	}
	//*******************************************

	//updating A node
	//******************************************
	if(this->right != nullptr)
	{
		this->setPlayersAmountSubTree(this->left->getPlayersAmountSubTree()+this->right->getPlayersAmountSubTree()+this->getPlayersAmount());
		for(int i=1;i<=this->getScale();i++)
		{
			A_score_arr_sub[i] = B_score_arr_sub[i] + this->right->getScoreArrSubTree()[i] + this->getScoreArr()[i];
		}
		this->setLevelSumSubTree(this->left->getLevelsSumSubTree() + this->right->getLevelsSumSubTree() + this->getLevelSum());
		return;
	}
	else
	{
		this->setPlayersAmountSubTree(this->left->getPlayersAmountSubTree()+this->getPlayersAmount());
		for(int i=1;i<=this->getScale();i++)
		{
			A_score_arr_sub[i] = B_score_arr_sub[i] + this->getScoreArr()[i];
		}
		this->setLevelSumSubTree(this->left->getLevelsSumSubTree() + this->getLevelSum());
		return;
	}
	//*******************************************
}



template<class T>
RankNode<T>* RankNode<T>::getLeftSon(){
	return this->left;
}

template<class T>
RankNode<T>* RankNode<T>::getRightSon(){
	return this->right;
}

template<class T>
RankNode<T>* RankNode<T>::getParent(){
	return this->parent;
}

template<class T>
void RankNode<T>::rank(RankNode<T>* level_node, int level, int score, int* total_players, int* players_in_score)
{
	int total_players_final = 0;
	int players_in_score_final = 0;
	RankNode<T>* iter_node = this;
	while(iter_node->getKey() != level_node->getKey())
	{
		if(level < *(iter_node->getKey()))
		{
			iter_node = iter_node->left;
		}
		else if(level > *(iter_node->getKey()))
		{
			if(iter_node->left != nullptr)
			{
				total_players_final += iter_node->left->getPlayersAmountSubTree();
				players_in_score_final += (iter_node->left->getScoreArrSubTree())[score];
			}
			total_players_final += iter_node->getPlayersAmount();
			players_in_score_final += (iter_node->getScoreArr())[score];
			iter_node = iter_node->right;
		}
	}
	//found the specific NOde:
	if(iter_node->left == nullptr)
	{
		total_players_final += iter_node->getPlayersAmount();
		players_in_score_final += (iter_node->getScoreArr())[score];
	}
	else
	{
		total_players_final += iter_node->left->getPlayersAmountSubTree();
		total_players_final += iter_node->getPlayersAmount();
		players_in_score_final += (iter_node->left->getScoreArrSubTree())[score];
		players_in_score_final += (iter_node->getScoreArr())[score];
	}
	*(players_in_score) = players_in_score_final;
	*(total_players) = total_players_final;
}

/* Inserts node into the tree in an AVL fashion, thus inkoving the rotations if ones are needed. ASSUMES THT THERE IS NO NODE
	WITH THE SAME KEY
@param: key_add - a point of the T* data of the newly added node.
return value - the root of the new tree after the insertion and the rotations.
 */
template<class T>
RankNode<T>* RankNode<T>::insert(T* key_add, int score, int scale){

	if(*key_add < *(this->key)){
		if(this->left == nullptr){
			try{
				this->updateNodeFeaturesAtInsert(score);
				this->left = new RankNode<T>(key_add, this, score, scale);	
			} 	catch (std::bad_alloc& e){
				throw e;
			}
		}
		else{
			this->updateNodeFeaturesAtInsert(score);
			this->left->insert(key_add, score, scale);
		}
	}
	else if(*key_add > *(this->key)) {
		if(this->right == nullptr) {
			try {
				this->updateNodeFeaturesAtInsert(score);
				this->right = new RankNode<T>(key_add , this, score, scale);
			} 	catch (std::bad_alloc& e){
				throw e;
			}
		} 
		else {
			this->updateNodeFeaturesAtInsert(score);
			this->right->insert(key_add, score, scale);
		}
	}
	return this->balanceTree();	
}

template<class T>
RankNode<T>* RankNode<T>::llRotate(){
	RankNode<T>* temp_left = this->left; 
	this->left = this->left->right;	
	if(this->left != nullptr){	
		this->left->parent = this;
	}
	temp_left->right = this;
	temp_left->parent = this->parent;
	// Updating the parent of the current offended BF node to point to the new root of the sub-tree
	if(this->parent != nullptr){
		if(this->parent->left != nullptr && this->parent->left->getKey() == this->getKey()){
			this->parent->left = temp_left;
		}
		else{
			this->parent->right = temp_left;
		}
	}
	this->parent = temp_left;	// this->parent shall not be nullptr in case there's more trees above to cover.
	temp_left->updateHeight();
	this->updateHeight();
	temp_left->updateLLFeatures();
	return temp_left;
}

template<class T>
RankNode<T>* RankNode<T>::rrRotate()
{
	RankNode<T>* temp_right = this->right; 	
	this->right = this->right->left;	
	if(this->right != nullptr)	
	{
		this->right->parent = this;
	}
	temp_right->left = this;
	temp_right->parent = this->parent;
	
	// Updating the parent of the current offended BF node to point to the new root of the sub-tree
	if(this->parent != nullptr){
		if(this->parent->left != nullptr && this->parent->left->getKey() == this->getKey()) 
		{
			this->parent->left = temp_right;
		}
		else{
			this->parent->right = temp_right;
		}
	}
	this->parent = temp_right;		// this->parent shall not be nullptr in case there's more trees above to cover.
	this->updateHeight();
	temp_right->updateHeight();
	temp_right->updateRRFeatures();
	return temp_right;
}

template<class T>
RankNode<T>* RankNode<T>::lrRotate()
{
	this->left->rrRotate();
	return this->llRotate();
}

template<class T>
RankNode<T>* RankNode<T>::rlRotate(){
	this->right->llRotate();
	return this->rrRotate();
}

/* 
	Returns the tree's new root
 */
template<class T>
RankNode<T>* RankNode<T>::balanceTree(){ 
	RankNode<T>* temp_iterator = this;  
	RankNode<T>* new_tree_root = this; 
	while(temp_iterator != nullptr){
		temp_iterator->updateNodeFeatures();
		temp_iterator->updateLevelSumSubTree();
		if(temp_iterator->getBalanceFactor() == L_ROTATE){	// means LR or LL rotation
		
			if(temp_iterator->getLeftSon()->getBalanceFactor() >= 0 )
			{
			temp_iterator->llRotate();
			} else if (temp_iterator->getLeftSon()->getBalanceFactor() == -1 )
			{
				temp_iterator->lrRotate();
			}
		}
		else if(temp_iterator->getBalanceFactor() == R_ROTATE){	// mean RR or RL rotation
				if(temp_iterator->getRightSon()->getBalanceFactor() <= 0){
					temp_iterator->rrRotate();
				}
				else if(temp_iterator->getRightSon()->getBalanceFactor() == 1){
				temp_iterator->rlRotate();
			}
		}
		new_tree_root = temp_iterator;
		temp_iterator = temp_iterator->parent; 	
	}
	return new_tree_root;
}	

template<class T>
RankNode<T>* RankNode<T>::getMinNode()
{
	RankNode<T>* min_node = this;
	while(min_node->left != nullptr)
	{
		min_node = min_node->getLeftSon();
	}
	return min_node;
}

template<class T>
RankNode<T>* RankNode<T>::getMaxNode()
{
	RankNode<T>* max_node = this;
	while(max_node->right != nullptr)
	{
		max_node = max_node->getRightSon();
	}
	return max_node;
}


/**
 * @brief removes 'this' and updates the parents left and right sons accordingly
 * removeNode() is called only by remove()
 * Does not balance the WHOLE tree after removal, only the sub tree! The update for the whole is done in reomve() as recursion ends.
 * @tparam T 
 * @return RankNode<T>* new subtree root, balanced.
 */
template<class T>
RankNode<T>* RankNode<T>::removeNode(){
	RankNode<T>* temp_this = this; // for backup up 
	if(this->hasLeftSon() == false && this->hasRightSon() == false) { // 'this' is a leaf, we just need to remove him. Regardless if it is a root.
		if(this->isTheLeftSon()) { // 'this' node is a leaf and the left son of a parent
			this->parent->left = nullptr; 
		}
		if(this->isTheRightSon()) { // this, is and a leaf and is the right son
			this->parent->right = nullptr;
		}
		temp_this = this->parent;
		delete this;
		return temp_this;
	} 
	// Deletion algorithm STEP 2: Assigment of a son to the parent
	else { // not a leaf
		if(this->hasOnlyOneSon()) { 	// not a leaf, has one son atleast. 
			if(this->parent == nullptr){
				if(this->hasLeftSon()){
					this->left->parent = nullptr;
					temp_this = this->left; 		
					if(temp_this->parent == nullptr){
					temp_this->updateHeight();			//this if is for when the deleted root has only one son which becomes the root.
					} else {
					temp_this->parent->updateHeight();						
					}
					delete this;
					temp_this->updateNodeFeatures();
					temp_this->updateLevelSumSubTree();
					return temp_this;	
				} else if(this->hasRightSon()){
					temp_this = this->right;	
					this->right->parent = nullptr;
					if(temp_this->parent == nullptr){	// this if is for when the deleted root has only one son which becomes the root.
					temp_this->updateHeight();						
					} else {
					temp_this->parent->updateHeight();						
					}
					delete this;
					temp_this->updateNodeFeatures();
					temp_this->updateLevelSumSubTree();	
					return temp_this;	
				}
			}
			if(this->hasRightSon())	{		//he got only right son and it is the only son
				this->right->parent = this->parent;
				if(this->isTheRightSon()) {
					this->parent->right = this->right;	
					
				}
				else if(this->isTheLeftSon()) {
					this->parent->left = this->right;
				}
			temp_this = temp_this->right;	
			temp_this->parent->updateHeight();
			delete this;
			temp_this->updateNodeFeatures();
			temp_this->updateLevelSumSubTree();
			return temp_this;
			}
			else if(this->hasLeftSon()){
				this->left->parent = this->parent;
				if(this->isTheRightSon()){
					this->parent->right = this->left;
				}
				else if(this->isTheLeftSon()){
					this->parent->left = this->left;
				}
				temp_this = temp_this->left;
				temp_this->parent->updateHeight();
				delete this;
				temp_this->updateNodeFeatures();
				temp_this->updateLevelSumSubTree();
				return temp_this;
			}
		}
	else {
		// got 2 sons
		//swapping between min and this;
		RankNode<T>* min_node = this->right->getMinNode();
		bool adjacent = false;
		RankNode<T>* min_node_right = min_node->right; 	// can also be null
		RankNode<T>* min_node_parent = min_node->parent;	// will never be null
		/* Special case */
		if(this->right->getKey() == min_node->getKey()){
			adjacent = true;
			min_node->right = this;
		}
		else {
			min_node->right = this->right;	
			this->right->parent = min_node;
		}
		/* updating min_node pointers and its new sons pointers */
		min_node->left = this->left;
		this->left->parent = min_node;
		/* updating the min_node new sons parent pointers, if the parent is nullptr, which means the parent is the root 
		the updating is done by the reutrn value and not here (in the case Min_node is the new root)*/
		min_node->parent = this->parent;
		if(this->parent != nullptr){	
			if(this->isTheLeftSon()){
				this->parent->left = min_node;
			} else if(this->isTheRightSon()){
				this->parent->right = min_node;
			}
		}
		/* Updating 'this's new pointers and its new sons pointers */
		this->right = min_node_right;
		if(this->right != nullptr){ // if min_node->right parent exist, update them. 
			this->right->parent= this;
		}
		this->left = nullptr;
		if(adjacent == true){
			this->parent = min_node;
			
		} else {
			min_node_parent->left = this;
			this->parent = min_node_parent;
		}
		RankNode<T>* temp_iter = this;
		while(temp_iter != nullptr)
		{
			temp_iter->updateHeight();
			temp_iter->updateNodeFeatures();
			temp_iter->updateLevelSumSubTree();
			temp_iter = temp_iter->getParent();
		}
		this->removeNode();	// now 'this' is removed, maybe as a leaf and maybe as a father to one right son.
		min_node->updateHeight();// the parent of the left subtree's root.
		min_node->updateNodeFeatures();
		min_node->updateLevelSumSubTree();  
		return min_node;	// min node is the new subtree root.
	}
}
return nullptr;  //note:  we should never get here. The return here is for the compiler to shut up. 
}

/**
 * @brief the function removes the node with the same key value as passed in the @key_remove param. 
the function ASSUMES that the wanted value is in the node or in its subtrees, since the check for the existance of
the vaulue is done before-hand in the remove function of the AVLTree_remove!
 * ASSUMES that tree is not empty. 
 * @tparam T
 * @param key_remove - a pointer to the value we wish to remove.
 * @return RankNode<T>* Returns the new tree's root after balancing it.
 * */
template<class T>
RankNode<T>* RankNode<T>::remove(T* key_remove){
	RankNode<T>* new_subtree = this; // the subtree after removal of 'this'
	RankNode<T>* temp_parent = this->parent; // the subtree after removal of 'this'
	RankNode<T>* temp_iterator = this->parent;   // for later usage for upstream update.
	if(*(this->key) < *key_remove){
			this->right->remove(key_remove);
	} else if((*this->key) > *key_remove){
		this->left->remove(key_remove);
	} else { // found the key
		new_subtree = this->removeNode(); 
		temp_iterator = new_subtree;
		while(temp_iterator != nullptr){
			temp_iterator->updateHeight();
			temp_iterator->updateNodeFeatures();
			temp_iterator->updateLevelSumSubTree();
			temp_iterator = temp_iterator->parent;
		}
	}
	if(new_subtree == nullptr && temp_parent == nullptr){ // root, is a leaf and is removed OR a leaf with a parent
		return nullptr;
	} else {
		return new_subtree->balanceTree(); // not optimal for perfomance but works (some calls will be futile O(1) times)
	}
}

/**
 * @brief searches for a key inside the AVL tree
 *
 * @param key_find - the key
 * @return RankNode<T>* - a pointer to the node if found. NULLPTR is not found.
 */
template<class T>
RankNode<T>* RankNode<T>::search(T* key_find)
{
	RankNode<T>* node_iterator = this; 
	if(node_iterator == nullptr){ 	// The stop condition that we have arrived at a leaf
		return nullptr;
	}
	while(node_iterator != nullptr)	{
		node_iterator->updateNodeFeatures();
		node_iterator->updateLevelSumSubTree();
		if(*(node_iterator->getKey()) == *key_find){
			return node_iterator;
		}
		else if( *(node_iterator->getKey()) > *key_find){
			node_iterator = node_iterator->left;
		} else if( *(node_iterator->getKey()) < *key_find){
			node_iterator = node_iterator->right;
		}
	}
	return node_iterator;
}

template<class T>
RankNode<T>* RankNode<T>::searchMin(T* key_find)
{
	RankNode<T>* node_iterator = this; 
	if(node_iterator == nullptr){ 	// The stop condition that we have arrived at a leaf
		return nullptr;
	}
	while(node_iterator != nullptr)	{
		if(*(node_iterator->getKey()) == *key_find){
			return node_iterator;
		}
		else if( *(node_iterator->getKey()) > *key_find){
			if(node_iterator->left == nullptr) return node_iterator;
			node_iterator = node_iterator->left;
		} else if( *(node_iterator->getKey()) < *key_find){
			node_iterator = node_iterator->right;
		}
	}
	return node_iterator;
}

template<class T>
RankNode<T>* RankNode<T>::searchMax(T* key_find)
{
	RankNode<T>* node_iterator = this; 
	if(node_iterator == nullptr){ 	// The stop condition that we have arrived at a leaf
		return nullptr;
	}
	while(node_iterator != nullptr)	{
		if(*(node_iterator->getKey()) == *key_find){
			return node_iterator;
		}
		else if( *(node_iterator->getKey()) > *key_find){
			node_iterator = node_iterator->left;
		} else if( *(node_iterator->getKey()) < *key_find){
			if(node_iterator->right == nullptr) return node_iterator;
			node_iterator = node_iterator->right;
		}
	}
	return node_iterator;
}

template<class T>
void RankNode<T>::searchAndUpdateAtInsert(T* key_find, int score)
{
	RankNode<T>* node_iterator = this;
	if(*(node_iterator->getKey()) == *key_find)
	{
		this->updateNodeAtInsert(score);
		return;
	}
	this->updateNodeFeaturesAtInsert(score); 
	while(node_iterator != nullptr)	{
		if(*(node_iterator->getKey()) == *key_find){
			node_iterator->updateNodeAtInsert(score);
			return;
		}
		else if( *(node_iterator->getKey()) > *key_find){
			if(*(node_iterator->left->getKey()) != *key_find)
			{
				node_iterator->left->updateNodeFeaturesAtInsert(score);
			}
			node_iterator = node_iterator->left;
		} else if( *(node_iterator->getKey()) < *key_find){
			if(*(node_iterator->right->getKey()) != *key_find)
			{
				node_iterator->right->updateNodeFeaturesAtInsert(score);
			}
			node_iterator = node_iterator->right;
		}
	}
	return;
}

template<class T>
void RankNode<T>::searchAndUpdateAtRemove(T* key_find, int score)
{
	RankNode<T>* node_iterator = this;
	if(*(node_iterator->getKey()) == *key_find)
	{
		this->updateNodeAtRemove(score);
		return;
	}
	this->updateNodeFeaturesAtRemove(score, key_find); 
	while(node_iterator != nullptr)	{
		if(*(node_iterator->getKey()) == *key_find){
			node_iterator->updateNodeAtRemove(score);
			return;
		}
		else if( *(node_iterator->getKey()) > *key_find){
			if(*(node_iterator->left->getKey()) != *key_find)
			{
				node_iterator->left->updateNodeFeaturesAtRemove(score, key_find);
			}
			node_iterator = node_iterator->left;
		} else if( *(node_iterator->getKey()) < *key_find){
			if(*(node_iterator->right->getKey()) != *key_find)
			{
				node_iterator->right->updateNodeFeaturesAtRemove(score, key_find);
			}
			node_iterator = node_iterator->right;
		}
	}
	return;
}

template<class T>
int RankNode<T>::max(int a, int b){
	if(a > b) return a;
	if(a < b) return b;
	return a;
}

template<class T>
void RankNode<T>::inOrder(){
	if(this == nullptr) return;
	this->left->inOrder();
	std::cout << (*(this->key))  << std::endl;
	this->right->inOrder();
}

template<class T>
void RankNode<T>::inOrderReverse(){
	if(this == nullptr) return;
	this->right->inOrderReverse();
	std::cout << *(this->key) << std::endl;
	this->left->inOrderReverse();

}

template<class T>
void RankNode<T>::setRightSon(RankNode<T>* new_son){
	this->right = new_son;
}

template<class T>
void RankNode<T>::setLeftSon(RankNode<T>* new_son){
	this->left = new_son;
}

template<class T>
void RankNode<T>::setParent(RankNode<T>* new_parent){
	this->parent = new_parent;
}

template<class T>
void RankNode<T>::setHeight(int h){
	this->height = h;
}

template<class T>
bool RankNode<T>::isLeaf(){
	return (this->left == nullptr && this->right == nullptr);
}

template<class T>
void RankNode<T>::setKey(T* new_key){
	this->key = new_key;
}

template<class T>
int RankNode<T>::getPlayersAmount(){
	return this->players_amount;
}

template<class T>
int RankNode<T>::getLevelSum(){
	return this->level_sum;
}

template<class T>
int RankNode<T>::getLevelsSumSubTree(){
	return this->levels_sum_sub_tree;
}

template<class T>
int RankNode<T>::getPlayersAmountSubTree(){
	return this->players_amount_sub_tree;
}

template<class T>
void RankNode<T>::setLevelSumSubTree(int level_sum_sub){
	this->levels_sum_sub_tree = level_sum_sub;
}

template<class T>
void RankNode<T>::setNodeLevelSum(int level_sum){
	this->level_sum = level_sum;
}

template<class T>
int RankNode<T>::getScale(){
	return this->scale;
}

template<class T>
int* RankNode<T>::getScoreArr(){
	return this->players_score_arr;
}

template<class T>
int* RankNode<T>::getScoreArrSubTree(){
	return this->players_score_arr_sub_tree;
}

template<class T>
int* RankNode<T>::copyScoreArr(){
	int* arr_to_copy = this->getScoreArr();
	int* new_arr = new int[this->getScale()+1];
	for(int i=1;i<=scale;i++)
	{
		new_arr[i] = arr_to_copy[i];
	}
	return new_arr;
}

template<class T>
int* RankNode<T>::copyScoreArrSubTree(){
	int* arr_to_copy = this->getScoreArrSubTree();
	int* new_arr = new int[this->getScale()+1];
	for(int i=1;i<=scale;i++)
	{
		new_arr[i] = arr_to_copy[i];
	}
	return new_arr;
}

template<class T>
void RankNode<T>::setScoreArrSubTree(int* score_arr){
	this->players_score_arr_sub_tree = score_arr;	
}

template<class T>
void RankNode<T>::setScoreArr(int* score_arr){
	this->players_score_arr = score_arr;	
}
template<class T>
void RankNode<T>::setPlayersAmountSubTree(int players_amount){
	this->players_amount_sub_tree = players_amount;	
}

template<class T>
void RankNode<T>::setPlayersAmount(int players_amount){
	this->players_amount = players_amount;	
}


#endif 