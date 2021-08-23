#pragma once
#include <windows.h>
#include <stdlib.h>
/*
��ʵ�ǰ���sgi stl���ڴ����������ʵ��һ��,��Ŀ��:
1.����stl�ڴ�����ʵ��˼·,����һ���ڴ����ʵ�ֵ�˼·�ͷ���
2.������ע�� ������������쳣����
3.�����뷨������
*/
class CMemoryPool {
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
	void* allocate(size_t size);
	void  dellocate(void* p, size_t size);
	size_t getLefNum(size_t size);
private:
	int8_t* refill(size_t size);
	int8_t* _alloc(size_t size,size_t& n);
	void  _free();
private:
	size_t roundUp(size_t size)
	{
		return (size + Align - 1) & ~(Align - 1);
	}

	size_t freeListIndex(size_t size)
	{
		return ((size + Align - 1) / Align - 1);
	}
private:
	int8_t* _start = 0;
	int8_t* _end = 0;
	int32_t _size = 0;
private:
	static const int Align = 8;
	static const int MaxBytes = 128;
	static const int NumberOfFreeLists = MaxBytes / Align;
	static const int NumberOfAddedNodesForEachTime = 20;
private:
	/*volatile*/ Node* free_list[NumberOfFreeLists] = { 0 };
};
//node* CMemoryPool::free_list[NumberOfFreeLists] = {0};

void* CMemoryPool::allocate(size_t size)
{
	if (size > MaxBytes)
	{
		return malloc(size);
	}
	size_t index = freeListIndex(size);
	Node* pFreeList = free_list[index];
	if (pFreeList == 0)
	{
		return refill(roundUp(size));
	}
	free_list[index] = pFreeList->next_node;
	return pFreeList;
}

void  CMemoryPool::dellocate(void* p,size_t size)
{
	Node* node = (Node*)p;
	if (size > MaxBytes)
	{
		free(p);
		return;
	}
	Node** myFreeList = free_list + freeListIndex(size);
	node->next_node = (*myFreeList)->next_node;
	*myFreeList = node;
}

int8_t* CMemoryPool::refill(size_t size)
{
	size_t defnum = 20;
	int8_t* chunk = _alloc(size, defnum);

	if (defnum == 1) return chunk;
	Node* result = (Node*)chunk;
	Node** my_free_list = free_list + freeListIndex(size);
	Node* tmp = *my_free_list = (Node*)(chunk + size);
	
	for (int i = 2; i < defnum ;i++)
	{
		tmp->next_node = (Node*)(chunk + i * size);
		tmp = tmp->next_node;
	}
	tmp->next_node = 0;
	return (int8_t*)result;
}

int8_t* CMemoryPool::_alloc(size_t size, size_t& n)
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
			Node** myFreeList = free_list + freeListIndex(leftSize);
			((Node*)_start)->next_node = *myFreeList;
			*myFreeList = (Node*)_start;
		}
		_start = (int8_t*)malloc(bytes_to_get);
		if (_start == 0)
		{
			for (int i = size;i < MaxBytes;i+=Align)
			{
				Node** myFreeList = free_list + freeListIndex(i);
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
		return(_alloc(size, n));//�ݹ�����Լ���Ϊ������nobjs
	}
}

void  CMemoryPool::_free()
{
	
}

size_t CMemoryPool::getLefNum(size_t size)
{
	Node** myFreeList = free_list + freeListIndex(size);
	Node* header = *myFreeList;
	int num = 0;
	for (;header!= 0;)
	{
		num++;
		header = header->next_node;
	}
	return num;
}