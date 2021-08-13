#pragma once
#include "List.h"
//Ë«ÏòÁ´±í

template <class Type>
class CDoublyList:public CList<Type>,public CList<Type>::iterator,public CList<Type>::Node
{	
public:
	~CDoublyList() { Release(); }
public:
	bool InsertList(Type* dst)
	{
		typename CList<Type>::Node* node = CreateNode(dst);
		if (!CList<Type>::startNode)
		{
			CList<Type>::startNode = node;
			CList<Type>::endNode = node;
		}
		else
		{
			CList<Type>::endNode->nextNode = node;
			node->preNode = CList<Type>::endNode;
			CList<Type>::endNode = node;
		}
		return false;
	}

	bool DeleteList(Type* dst)
	{
		typename CList<Type>::Node* node = this->FindNode(dst);
		if (!node) return false;
		if (node == CList<Type>::startNode)
		{
			node->nextNode->preNode = nullptr;
			CList<Type>::startNode = node->nextNode;
			delete node;
		}
		else if (node == CList<Type>::endNode)
		{
			node->preNode->nextNode = nullptr;
			CList<Type>::endNode = node;
			delete node;
		}
		else
		{
			node->preNode->nextNode = node->nextNode;
			node->nextNode->preNode = node->preNode;
			delete node;
		}
		return true;
	}
	bool ModifyList(Type* src, Type* dst)
	{
		typename CList<Type>::Node* node = this->FindNode(src);
		if (node) node->pdata = dst;
		else return false;
		return true;
	}
	CList<Type>::Node* SelectList(Type* dst)
	{
		return FindNode(dst);
	}
	size_t Statistics()
	{
		return 0;
	}
private:
	CList<Type>::Node* CreateNode(Type* src) {
		typename CList<Type>::Node* node = new typename CList<Type>::Node();
		node->nextNode = nullptr;
		node->preNode = nullptr;
		node->pdata = src;
		return node;
	}
	CList<Type>::Node* FindNode(Type* src)
	{
		if (!CList<Type>::startNode || !CList<Type>::endNode || !src)
		{
			return nullptr;
		}
		if (CList<Type>::startNode->pdata == src) return CList<Type>::startNode;
		else if (CList<Type>::endNode->pdata == src) return CList<Type>::endNode;
		else {
			typename CList<Type>::Node* tmp = CList<Type>::startNode->nextNode;
			typename CList<Type>::Node* tmp1 = CList<Type>::endNode->preNode;
			do
			{
				if (tmp->pdata == src) return tmp;
				if (tmp1->pdata == src) return tmp1;
				tmp = tmp->nextNode;
				tmp1 = tmp1->preNode;
			} while (tmp1->nextNode != tmp && tmp1->nextNode != tmp->preNode);
		}
		return nullptr;
	}
	void Release()
	{
		typename CList<Type>::Node* node = CList<Type>::startNode;
		while (node != nullptr)
		{
			CList<Type>::startNode = node->nextNode;
			delete node;
			node = CList<Type>::startNode;
		}
	}
};