#pragma once
//TODO:后期添加使用模板template;

template <typename Type>
class CSinglyList
{
public:
	class Node
	{
	public:
		//Data* startPtr;
		Node* nextPtr;
		Type* dataPtr;
	};
public:
	class iterator {
	public:
		iterator(Node* Ptr) :ptr(Ptr){}
		iterator() {}
		~iterator() {}
	private:
		Node* ptr;
	public:

		iterator& operator++();
		iterator operator++(int);
		
		Node& operator*();
		Node* operator->();

		bool operator!=(const iterator& itor);
		bool operator==(const iterator& itor);
	};
public:
	CSinglyList();
	~CSinglyList();
public:
	//define any of operations for list
	bool InsertListNode(Type* dst);
	bool DeleteListNode(Type* dst);
	bool ModifyListNode(Type* src, Type* dst);
	Node* SelectListNode(Type* dst);
	size_t StatisticsNode();

	iterator begin();
	iterator end();

private:
	Node* CreateNode(Type* content);
	Node* FindNode(Type* src);
	void  DeleteAllNode();
protected:
	Node* startNode = nullptr;
	Node* endNode = nullptr;
};

