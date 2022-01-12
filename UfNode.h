#ifndef UFNODE
#define UFNODE

#include "Group.h"



const int UF_INITIAL_SIZE = 1;
template<class T> 
class UfNode{
	T*              data;
	int             uf_size;
	UfNode<T>*      parent;
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

template<class T> 
int UfNode<T>::getUfSize() const
{
    return this->uf_size;
}

template<class T> 
void UfNode<T>::setUfSize(int size)
{
    this->uf_size = size;
}

template<class T> 
T* UfNode<T>::getData() 
{
    return (this->data);
}

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