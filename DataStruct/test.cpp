// DataStruct.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <chrono>
#include "SinglyList.h"
#include "SinglyList.cpp"
#include "DoublyList.h"

void test1()
{
	CSinglyList<int> singlyList;
	int value[] = { 1,2,3,4,5,6,9,20,12,3,34 };
	for (int i = 0; i < sizeof(value) / sizeof(int); i++)
	{
		singlyList.InsertListNode(&value[i]);
	}
	std::cout << "list size:" << singlyList.StatisticsNode() << std::endl;
	//使用迭代器遍历
	CSinglyList<int>::iterator itor = singlyList.begin();
	for (; itor != singlyList.end(); ++itor)
	{
		std::cout << "size:" << *(int*)itor->dataPtr << std::endl;
	}
}

void test2()
{
	CList<int>* doublyList = new CDoublyList<int>();
	int value[] = { 1,2,3,4,5,6,9,20,12,3,34 };
	for (int i = 0; i < sizeof(value) / sizeof(int); i++)
	{
		doublyList->InsertList(&value[i]);
	}
	CDoublyList<int>::iterator itor = doublyList->begin();
	for (int i = 0;itor != doublyList->end();++itor,++i)
	{
		std::cout << "element_" << i << ":" << *(int*)itor->pdata << std::endl;
	}
	doublyList->DeleteList(&value[9]);
	doublyList->InsertList(&value[9]);
	doublyList->ModifyList(&value[9], &value[1]);
	itor = doublyList->begin();
	for (int i = 0; itor != doublyList->end(); ++itor, ++i)
	{
		std::cout << "element_" << i << ":" << *(int*)itor->pdata << std::endl;
	}

	delete doublyList;
}

int main()
{
	
	test2();
	//Sleep(10000);
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	return 0;
}

