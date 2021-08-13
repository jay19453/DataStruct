#pragma once

template <class Type>
class CList
{
public:
	virtual ~CList() {}
public:
	class Node
	{
	public:
		Node* preNode;
		Node* nextNode;
		Type* pdata;
	};
public:
	class iterator {
	public:
		iterator(Node* Ptr) :ptr(Ptr) {}
		iterator() {}
		~iterator() {}
	private:
		Node* ptr;
	public:

		iterator& operator++() {
			ptr = ptr->nextNode;
			return *this;
		}
		iterator operator++(int) {
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		Node& operator*() { return *ptr; }
		Node* operator->() { return ptr; }

		bool operator!=(const iterator& itor) { return !(*this == itor); }
		bool operator==(const iterator& itor) { return ptr == itor.ptr; }
	};
public:
	virtual bool InsertList(Type* dst) = 0;
	virtual bool DeleteList(Type* dst) = 0;
	virtual bool ModifyList(Type* src, Type* dst) = 0;
	virtual Node* SelectList(Type* dst) = 0;
	virtual size_t Statistics() = 0;

	iterator begin() { return iterator(startNode); }
	iterator end() { return iterator(endNode->nextNode); }
public:
	Node* startNode = nullptr;
	Node* endNode = nullptr;
};