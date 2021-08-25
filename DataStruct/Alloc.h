#pragma once
#include <windows.h>
#include <stdlib.h>
#include <vector>
/*
其实是按照sgi stl的内存管理来重新实现一遍,其目的:
1.分析stl内存管理的实现思路,明白一个内存管理实现的思路和方法
2.添加相关注释 并且增加相关异常处理
3.其他想法。。。
*/
class alloc {
public:
	/*
	 stl在这里使用union,不使用struct,这里设计的很巧妙而且也很绕脑。
	 一般情况下,使用struct会定义nextPtr、dataPtr,这样很好理解,但是这样
	 存在两个小问题:1.增加了一个指针的空间使用 2.需要处理struct的变量。
	 stl在这用union结构来保存数据,能够完全代替struct,其中指向分配未使用的起始地址用data[1]来指向,
	 而next_node是指向下一个node地址,其中next_node保存下一个node地址是使用分配未使用内存,也就是说
	 在未使用的时候分配的内存当指针使用。
	*/
	union Node
	{
		union Node* next_node;
		char data[1];
	};
public:
	static void* allocate(size_t size);
	static void  dellocate(void* &p, size_t size);
	static size_t getLefNum(size_t size);
	static void  release();
private:
	static int8_t* refill(size_t size);
	static int8_t* _alloc(size_t size,size_t& n);
	void  _free();
private:
	static size_t roundUp(size_t size)
	{
		return (size + Align - 1) & ~(Align - 1);
	}

	static size_t freeListIndex(size_t size)
	{
		return ((size + Align - 1) / Align - 1);
	}
private:
	static int8_t* _start;
	static int8_t* _end;
	static int32_t _size;
private:
	static const int Align = 8;
	static const int MaxBytes = 128;
	static const int NumberOfFreeLists = MaxBytes / Align;
	static const int NumberOfAddedNodesForEachTime = 20;
private:
	static Node* volatile free_list[NumberOfFreeLists];
	static std::vector<int8_t*> malloc_vec;
};
alloc::Node* volatile alloc::free_list[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
std::vector<int8_t*> alloc::malloc_vec;
int8_t* alloc::_start = 0;
int8_t* alloc::_end = 0;
int32_t alloc::_size = 0;

void* alloc::allocate(size_t size)
{
	if (size > MaxBytes)
	{
		return malloc(size);
	}
	size_t index = freeListIndex(size);
	void* result;
	Node* volatile *myFreeList = free_list + index;
	result = *myFreeList;
	if (result == 0)
	{
		return refill(roundUp(size));
	}
	*myFreeList = ((Node*)result)->next_node;
	return result;
}

void  alloc::dellocate(void* &p,size_t size)
{
	if (nullptr == p) return;
	if (size > MaxBytes)
	{
		free(p);
		p = nullptr;
		return;
	}
	Node* node = (Node*)p;
	Node* volatile * myFreeList = free_list + freeListIndex(size);
	node->next_node = *myFreeList;
	*myFreeList = node;
	p = nullptr;
}

void  alloc::release()
{
	if (malloc_vec.size() > 0)
	{
		for (auto iter = malloc_vec.begin();iter != malloc_vec.end();++iter)
		{
			int8_t* _mem = *iter;
			free(_mem);
			_mem = nullptr;
		}
		malloc_vec.clear();
	}
}

int8_t* alloc::refill(size_t size)
{
	size_t defnum = 20;
	int8_t* chunk = _alloc(size, defnum);
	Node* tmp;

	if (defnum == 1) return chunk;
	Node* result = (Node*)chunk;
	Node* volatile * my_free_list = free_list + freeListIndex(size);
	*my_free_list= tmp = (Node*)(chunk + size);
	
	for (int i = 2; i < defnum ;i++)
	{
		tmp->next_node = (Node*)(chunk + i * size);
		tmp = tmp->next_node;
	}
	tmp->next_node = 0;
	return (int8_t*)result;
}

int8_t* alloc::_alloc(size_t size, size_t& n)
{
	size_t leftSize = _end - _start;
	size_t needSize = size * n;
	int8_t* result;
	if (leftSize >= needSize)
	{
		result = _start;
		_start = _start + needSize;
		return result;
	}else if (leftSize >= size)
	{
		n = needSize / size;
		result = _start;
		_start += size * n;
		return result;
	}
	else
	{
		size_t bytes_to_get = 2 * needSize + roundUp(_size >> 4);
		if (leftSize > 0)
		{
			Node* volatile * myFreeList = free_list + freeListIndex(leftSize);
			((Node*)_start)->next_node = *myFreeList;
			*myFreeList = (Node*)_start;
		}
		_start = (int8_t*)malloc(bytes_to_get);
		if (_start == 0)
		{
			for (int i = size;i < MaxBytes;i+=Align)
			{
				Node* volatile * myFreeList = free_list + freeListIndex(i);
				Node* headerNode = *myFreeList;
				if (headerNode != 0)
				{
					*myFreeList = headerNode->next_node;
					_start = (int8_t*)headerNode;
					_end = _start + i;
					return _alloc(i, n);
				}
			}
			//如果找不到怎么办？直接返回null
			_end = 0;
			return NULL;
		}
		//如果分配成功
		memset(_start, 0, bytes_to_get);
		_size += bytes_to_get;//内存池大小增加
		_end = _start + bytes_to_get;//修改内存池可用空间的结束位置
		malloc_vec.push_back(_start);
		return(_alloc(size, n));//递归调用自己，为了修正nobjs
	}
}

void  alloc::_free()
{
	
}

size_t alloc::getLefNum(size_t size)
{
	Node* volatile * myFreeList = free_list + freeListIndex(size);
	Node* header = *myFreeList;
	int num = 0;
	for (;header!= 0;)
	{
		num++;
		header = header->next_node;
	}
	return num;
}