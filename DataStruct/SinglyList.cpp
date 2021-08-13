#include "SinglyList.h"

template<typename Type>
CSinglyList<Type>::CSinglyList() {}

template<typename Type>
CSinglyList<Type>::~CSinglyList()
{
	//ÊÍ·Å×ÊÔ´
	if (StatisticsNode() > 0)
	{
		DeleteAllNode();
	}
}

template<typename Type>
bool CSinglyList<Type>::InsertListNode(Type* dst)
{
	if (dst)
	{
		Node* node = CreateNode(dst);
		if (!startNode)
		{
			startNode = node;
			endNode = node;
		}
		else {
			endNode->nextPtr = node;
			endNode = node;
		}
		return true;
	}
	
	return false;
}

template<typename Type>
bool CSinglyList<Type>::DeleteListNode(Type* dst)
{
	if (dst)
	{
		Node* preNode = FindNode(dst);
		Node* node = preNode->nextPtr;
		if (preNode)
		{
			if (endNode == node)
			{
				endNode = preNode;
				endNode->nextPtr = nullptr;
				//delete node->dataPtr;
				delete node;
				return true;
			}
			else
			{
				preNode->nextPtr = node->nextPtr;
				//delete node->dataPtr;
				delete node;
				return true;
			}
		}
	}
	return false;
}

template <typename Type>
bool CSinglyList<Type>::ModifyListNode(Type* src, Type* dst)
{

	return true;
}

template<typename Type>
typename CSinglyList<Type>::Node* CSinglyList<Type>::SelectListNode(Type* dst)
{
	return FindNode(dst);
}

template<typename Type>
size_t CSinglyList<Type>::StatisticsNode()
{
	size_t size = 0;
	for (Node* node = startNode;node != nullptr;)
	{
		size++;
		node = node->nextPtr;
	}
	return size;
}

template<typename Type>
typename CSinglyList<Type>::Node* CSinglyList<Type>::CreateNode(Type* dst)
{
	if (dst)
	{
		Node* node = new Node();
		node->dataPtr = dst;
		//node->startPtr = nullptr;
		node->nextPtr = nullptr;
		return node;
	}
	return nullptr;
}

template<typename Type>
typename CSinglyList<Type>::Node* CSinglyList<Type>::FindNode(Type* src)
{
	if (!startNode || !endNode || !src)
	{
		return nullptr;
	}
	for (Node* node = startNode;node != nullptr;)
	{
		if (node->dataPtr == src)
		{
			return node;
		}
		else {
			node = node->nextPtr;
		}
	}
	return nullptr;
}

template<typename Type>
void CSinglyList<Type>::DeleteAllNode()
{
	if (StatisticsNode() > 0)
	{
		for (Node* tmp = startNode;tmp != nullptr;)
		{
			startNode = tmp->nextPtr;
			delete tmp;
			tmp = startNode;
		}
	}
}

template<typename Type>
typename CSinglyList<Type>::iterator CSinglyList<Type>::begin()
{
	return CSinglyList<Type>::iterator(startNode);
}

template<typename Type>
typename CSinglyList<Type>::iterator CSinglyList<Type>::end()
{
	return CSinglyList<Type>::iterator(endNode->nextPtr);
}

template<typename Type>
typename CSinglyList<Type>::iterator& CSinglyList<Type>::iterator::operator++()
{
	ptr = ptr->nextPtr;
	return *this;
}

template<typename Type>
typename CSinglyList<Type>::iterator CSinglyList<Type>::iterator::operator++(int)
{
	iterator tmp = *this;
	++*this;
	return tmp;
}

template<typename Type>
bool CSinglyList<Type>::iterator::operator==(const iterator& itor)
{
	return this->ptr == itor.ptr;
}

template<typename Type>
bool CSinglyList<Type>::iterator::operator!=(const iterator& itor)
{
	return !(*this == itor);
}

template<typename Type>
typename CSinglyList<Type>::Node& CSinglyList<Type>::iterator::operator*()
{
	return *ptr;
}

template<typename Type>
typename CSinglyList<Type>::Node* CSinglyList<Type>::iterator::operator->()
{
	return ptr;
}