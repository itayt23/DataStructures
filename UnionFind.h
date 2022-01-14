#ifndef UNIONFIND
#define UNIONFIND

#include "exceptions.h"
#include "UfNode.h"


template<class T> 
class UnionFind {
    UfNode<T>**     uf_group_arr;       // number of groups sized array with each cell pointing to its unique group
    int             uf_group_arr_size;  // the size of the array, which is set on the startup with the number of groups

    void shrinkPath(UfNode<T>* root, UfNode<T>* origin);
	public:
		UnionFind(int k, int scale);
		~UnionFind();
        void unionGroups(const int group_id1, const int group_id2);
        T* find(const int group_id);
        UfNode<T>* getRoot(UfNode<T>* data);
        T* getGroup(const int group_id);
		class Empty {};
		class NodeAlreadyExists {};
		class DoesntExist {};
};

/**
 * @brief Construct a new Union Find< T>:: Union Find object
 * 
 * @tparam T 
 * @param k 
 * @param scale 
 */
template<class T>
UnionFind<T>::UnionFind(int k, int scale)
{
    this->uf_group_arr = new UfNode<T>*[k+1];
    this->uf_group_arr_size = k;
    for(int i = 1; i<=k; i++)
    {
        uf_group_arr[i] = new UfNode<T>(i, scale);
    }
}

/**
 * @brief Destroy the Union Find< T>:: Union Find object
 * 
 * @tparam T 
 */
template<class T>
UnionFind<T>::~UnionFind()
{
    for(int i = 1; i <= this->uf_group_arr_size; i++) 
    {
        delete uf_group_arr[i];
    }
    if(this->uf_group_arr != nullptr)
        delete[] uf_group_arr;
    this->uf_group_arr = nullptr;
}

/**
 * @brief union to sets of groups
 * 
 * @tparam T 
 * @param group_id1 
 * @param group_id2 
 */
template<class T>
void UnionFind<T>::unionGroups(const int group_id1, const int group_id2)
{
    UfNode<T>* group1 = (this->uf_group_arr)[group_id1];
    UfNode<T>* group2 = (this->uf_group_arr)[group_id2];
    UfNode<T>* root_group1 = getRoot(group1);
    UfNode<T>* root_group2 = getRoot(group2);
    if(*(root_group1->getData()) == *(root_group2->getData())) return;
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
/**
 * @brief getting the represntive groups off the set
 * 
 * @tparam T 
 * @param data 
 * @return the represntive groups off the set
 */
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

/**
 * @brief finding the represntive group of the whole set
 * 
 * @tparam T 
 * @param group_id 
 * @return the represntive group of the whole set
 */
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

/**
 * @brief shrinking the path from specific group to his represntive group.
 * 
 * @tparam T 
 * @param root 
 * @param origin 
 */
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