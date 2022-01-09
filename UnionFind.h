#ifndef UNIONFIND
#define UNIONFIND

#include "exceptions.h"
#include "UfNode.h"

//const int EMPTY_SIZE = 0; 


template<class T> 
class UnionFind {
    UfNode<T>**     uf_group_arr;
    int             uf_group_arr_size;
    void shrinkPath(UfNode<T>* root, UfNode<T>* origin);
	public:
		UnionFind(int k, int scale);
		~UnionFind();
        void unionGroups(const int group_id1, const int group_id2);
        //UfNode<T>* find(const int group_id);

        /**
         * @brief takes a group ID and returns a copy (why a copy and not a pointer???) of the representative (the root) of those in the set with the @group_id
         * 
         * @param group_id - wanted group ID
         * @return T 
         */
        T* find(const int group_id);
        UfNode<T>* getRoot(UfNode<T>* data);
        T* getGroup(const int group_id);
		class Empty {};
		class NodeAlreadyExists {};
		class DoesntExist {};
};

template<class T>
UnionFind<T>::UnionFind(int k, int scale)
{
    //T data = new T(0);
    this->uf_group_arr = new UfNode<T>*[k];
    this->uf_group_arr_size = k;
    //this->uf_group_arr = (UfNode<T>**)malloc(sizeof(*this->uf_group_arr)*k);
    for(int i = 1; i<=k; i++) // TODO: i is need to be 1 and i<=k?
    {
        uf_group_arr[i] = new UfNode<T>(i, scale);
    }
}

template<class T>
UnionFind<T>::~UnionFind()
{
    for(int i = 0; i < this->uf_group_arr_size; i++) 
    {
        delete uf_group_arr[i];
    }
    delete[] uf_group_arr;
}

/*
template<class T>
void UnionFind<T>::unionGroups(const int group_id1, const int group_id2)
{
    UfNode<T>* root_group1 = find(group_id1);
    UfNode<T>* root_group2 = find(group_id2);
    if(*(root_group1->getData()) == *(root_group2->getData())) return; //the groups already toghter //but not sure if i need it!
    if(root_group1->getUfSize() < root_group2->getUfSize())
    {
        root_group1->setParent(root_group2);
        root_group2->setUfSize(root_group2->getUfSize() + root_group1->getUfSize());

    }
    else if(root_group1->getUfSize() == root_group2->getUfSize())
    {
        if(*(root_group1->getData()) > *(root_group2->getData()))
        {
            root_group2->setParent(root_group1);
            root_group1->setUfSize(root_group2->getUfSize() + root_group1->getUfSize());
        }
        else
        {
            root_group1->setParent(root_group2);
            root_group2->setUfSize(root_group2->getUfSize() + root_group1->getUfSize());
        }
    }
    else
    {
        root_group2->setParent(root_group1);
        root_group1->setUfSize(root_group2->getUfSize() + root_group1->getUfSize());
    }
}
*/
/*
template<class T>
 UfNode<T>* UnionFind<T>::find(const int group_id)
{
    UfNode<T>* iter_node = (this->uf_group_arr)[group_id];
    UfNode<T>* origin_node = (this->uf_group_arr)[group_id];
    while(iter_node->getParent() != nullptr)
    {
        iter_node = iter_node->getParent();
    }
    shrinkPath(iter_node, origin_node);
    return iter_node;
}*/

template<class T>
void UnionFind<T>::unionGroups(const int group_id1, const int group_id2)
{
    UfNode<T>* group1 = (this->uf_group_arr)[group_id1];
    UfNode<T>* group2 = (this->uf_group_arr)[group_id2];
    UfNode<T>* root_group1 = getRoot(group1);
    UfNode<T>* root_group2 = getRoot(group2);
    if(*(root_group1->getData()) == *(root_group2->getData())) return; //the groups already toghter //but not sure if i need it!
    if(root_group1->getUfSize() < root_group2->getUfSize())
    {
        root_group1->setParent(root_group2);
        root_group2->setUfSize(root_group2->getUfSize() + root_group1->getUfSize());

    }
    else if(root_group1->getUfSize() == root_group2->getUfSize())
    {
        if(*(root_group1->getData()) > *(root_group2->getData()))
        {
            root_group2->setParent(root_group1);
            root_group1->setUfSize(root_group2->getUfSize() + root_group1->getUfSize());

        }
        else
        {
            root_group1->setParent(root_group2);
            root_group2->setUfSize(root_group2->getUfSize() + root_group1->getUfSize());
        }
    }
    else
    {
        root_group2->setParent(root_group1);
        root_group1->setUfSize(root_group2->getUfSize() + root_group1->getUfSize());
    }
}
template<class T>
UfNode<T>* UnionFind<T>::getRoot(UfNode<T>* data)
{
    UfNode<T>* iter_node = data;
    while(iter_node->getParent() != nullptr)
    {
        iter_node = iter_node->getParent();
    }
    return iter_node;
}

template<class T>
T* UnionFind<T>::getGroup(const int group_id)
{
    return (this->uf_group_arr)[group_id]->getData();
}


template<class T>
T* UnionFind<T>::find(const int group_id)
{
    UfNode<T>* iter_node = (this->uf_group_arr)[group_id];
    UfNode<T>* origin_node = (this->uf_group_arr)[group_id];
    while(iter_node->getParent() != nullptr)
    {
        iter_node = iter_node->getParent();
    }
    shrinkPath(iter_node, origin_node);
    return iter_node->getData();
}

template<class T>
void UnionFind<T>::shrinkPath(UfNode<T>* root, UfNode<T>* origin)
{
    UfNode<T>* iter_node = origin;
    UfNode<T>* temp_node = origin;
    while(temp_node->getParent() != nullptr)
    {
        temp_node = temp_node->getParent();
        iter_node->setParent(root);
        iter_node = temp_node;
    }
}



#endif 