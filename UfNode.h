#ifndef UFNODE
#define UFNODE

#include "Group.h"



const int UF_INITIAL_SIZE = 1;
template<class T> 
class UfNode{
	T*              data;       // a pointer to the data, in our case - group
	int             uf_size;    // number of groups within the union
	UfNode<T>*      parent;     // a pointer to the parent union find node
	public:
        UfNode(int key, int scale, int uf_size=UF_INITIAL_SIZE, UfNode<T>* parent = nullptr);
		~UfNode();
        int getUfSize() const;
        T* getData();
        UfNode<T>* getParent();

        void setParent(UfNode<T>* parent);
        void setUfSize(int size);


        bool operator==(const T& data) const{
            if(this->data == data) return true;
            return false;
        }

};

template<class T> 
UfNode<T>::UfNode(int key, int scale, int uf_size, UfNode<T>* parent) :uf_size(uf_size), parent(parent)
{
    this->data = new T(key, scale);
}

template<class T> 
UfNode<T>::~UfNode()
{
    delete data;
    this->data = nullptr;
    this->parent = nullptr;
}

/**
 * @brief get the size of all the uNIongroups with the same representive.
 * 
 * @tparam T 
 * @return the size of the union groups
 */
template<class T> 
int UfNode<T>::getUfSize() const
{
    return this->uf_size;
}

/**
 * @brief setting the union groups size
 * 
 * @tparam T 
 * @param size 
 */
template<class T> 
void UfNode<T>::setUfSize(int size)
{
    this->uf_size = size;
}

/**
 * @brief get the data this node is restoring(in our case is group)
 * 
 * @tparam T 
 * @return T* 
 */
template<class T> 
T* UfNode<T>::getData() 
{
    return (this->data);
}

/**
 * @brief set pointer to the parent UFgroup
 * 
 * @tparam T 
 * @param parent 
 */
template<class T> 
void UfNode<T>::setParent(UfNode<T>* parent) 
{
    this->parent = parent;
}

template<class T> 
UfNode<T>* UfNode<T>::getParent() 
{
    return this->parent;
}

#endif 