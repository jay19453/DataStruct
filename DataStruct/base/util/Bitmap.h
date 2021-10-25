#pragma once
#include <vector>
//�Ƕ�bit���в�������insert��find������
//����find��linux�ں�find_next_bit�������
//���������ϸ�ɲ鿴https://www.cnblogs.com/zpcdbky/p/5825849.html
//��Ҫ����ʱ��������¼ÿ�����ӵĲ��� ������ʱ��������1000��1ms��� 
namespace wnet {
	class Bitmap {
	public:
		Bitmap();
		~Bitmap();
	public:
		bool Init(uint32_t size);
		bool Insert(uint32_t index);
		bool Remove(uint32_t index);
		int32_t Find(uint32_t index);
		bool Empty();
		void Clear();
	private:
		uint32_t _vec_index;
		std::vector<uint64_t> _bitmap;
	};
}