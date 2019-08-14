#include "stdafx.h"
#include "ringbuf.h"
#include <algorithm>
#include <iostream>
#include <assert.h>


const short FLAG = 0xa5a5;
using namespace std;

ringbuf::ringbuf(int size, int num): capacity_(num), unit_size_(size)
{
	int bufSize = (sizeof(Header) + size)*num;
	buffer_addr_ = new char[bufSize];
	memset(buffer_addr_, 0, bufSize);
	product_list_.clear();
	customer_list_.clear();
}

void ringbuf::Init()
{
	lock_guard<mutex> locker(mutex_);
	
	for (int i = 0; i < capacity_; i++)
	{
		
		Header* head = (Header*)(buffer_addr_ + i * (sizeof(Header) + unit_size_));
	
		head->flag = FLAG;
		head->in_use = 0;
		product_list_.push_back((char*)head);
	}
}

ringbuf::~ringbuf()
{
	delete buffer_addr_;
}


char *ringbuf::GetOneBuf()
{
	lock_guard<mutex> locker(mutex_);

	if (product_list_.empty())
		return nullptr;

	char *tmp = product_list_.front();
	customer_list_.push_back(tmp);
	product_list_.pop_front();
	cout << "GetOneBuf 0x" << (int)(tmp+ sizeof(Header)) << endl;
	return tmp + sizeof(Header);
}

void ringbuf::RecycOneBuf(char *addr)
{
	Header* tmp = (Header*)(addr - sizeof(Header));
	if (tmp->flag != FLAG)
		assert(0);
	lock_guard<mutex> locker(mutex_);
	//customer_list_.remove([&tmp](char* addr) { return addr == (char*)tmp; });
	auto it = find(customer_list_.begin(), customer_list_.end(), (char*)tmp);
	if (it != customer_list_.end())
	{
		customer_list_.erase(it);
		tmp->in_use = 0;
		product_list_.push_back((char*)tmp);
		cout << "RecycOneBuf 0x" << (int)addr << endl;
		return;
	}
	
		
	//for (auto it = customer_list_.begin(); it != customer_list_.end(); ++it)
	//{
	//	if ((char*)tmp == *it)
	//	{
	//		tmp->in_use = 0;
	//		customer_list_.erase(it);
	//		product_list_.push_back((char*)tmp);
	//		return;
	//	}
	//}

	{
		//throw out
		cout << "get error ring buf ptr " << tmp << endl;
	}
	return;
}