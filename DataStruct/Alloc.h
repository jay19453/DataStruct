#pragma once
#include <windows.h>
#include <stdlib.h>
#include <vector>
/*
��ʵ�ǰ���sgi stl���ڴ����������ʵ��һ��,��Ŀ��:
1.����stl�ڴ�����ʵ��˼·,����һ���ڴ����ʵ�ֵ�˼·�ͷ���
2.������ע�� ������������쳣����
3.�����뷨������
*/
class alloc {
public:
	/*
	 stl������ʹ��union,��ʹ��struct,������Ƶĺ��������Ҳ�����ԡ�
	 һ�������,ʹ��struct�ᶨ��nextPtr��dataPtr,�����ܺ����,��������
	 ��������С����:1.������һ��ָ��Ŀռ�ʹ�� 2.��Ҫ����struct�ı�����
	 stl������union�ṹ����������,�ܹ���ȫ����struct,����ָ�����δʹ�õ���ʼ��ַ��data[1]��ָ��,
	 ��next_node��ָ����һ��node��ַ,����next_node������һ��node��ַ��ʹ�÷���δʹ���ڴ�,Ҳ����˵
	 ��δʹ�õ�ʱ�������ڴ浱ָ��ʹ�á�
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
			//����Ҳ�����ô�죿ֱ�ӷ���null
			_end = 0;
			return NULL;
		}
		//�������ɹ�
		memset(_start, 0, bytes_to_get);
		_size += bytes_to_get;//�ڴ�ش�С����
		_end = _start + bytes_to_get;//�޸��ڴ�ؿ��ÿռ�Ľ���λ��
		malloc_vec.push_back(_start);
		return(_alloc(size, n));//�ݹ�����Լ���Ϊ������nobjs
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