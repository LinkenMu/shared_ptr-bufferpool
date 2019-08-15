#include "stdafx.h"
#include "ringbuf.h"
#include <algorithm>
#include <iostream>
#include <assert.h>


const short FLAG = 0xa5a5;
using namespace std;

ringbuf::ringbuf(int size, int num): raw_capacity_(num), unit_size_(size)
{
	int bufSize = (sizeof(Header) + size)*num;
	buffer_addr_ = (char*)malloc(bufSize);
	memset(buffer_addr_, 0, bufSize);
	buf_stack_.clear();
	enlarge_counter_ = 0;

}

void ringbuf::Init()
{
	lock_guard<mutex> locker(mutex_);
	
	for (int i = 0; i < raw_capacity_; i++)
	{
		
		Header* head = (Header*)(buffer_addr_ + i * (sizeof(Header) + unit_size_));
	
		head->flag = FLAG;
		head->in_use = 0;
		buf_stack_.push_back((char*)head);
	}
}

ringbuf::~ringbuf()
{
	free(buffer_addr_);
}


char *ringbuf::GetOneBuf()
{
	lock_guard<mutex> locker(mutex_);

	if (buf_stack_.empty())
	{
		buffer_addr_ = (char*)realloc(buffer_addr_, raw_capacity_*(sizeof(Header) + unit_size_)*(enlarge_counter_ + 1));
		if (nullptr != buffer_addr_)
		{
			for (int i = 0; i < raw_capacity_; i++)
			{
				Header* head = (Header*)(buffer_addr_ + (raw_capacity_*enlarge_counter_ + i) * (sizeof(Header) + unit_size_));
				head->flag = FLAG;
				head->in_use = 0;
				buf_stack_.push_back((char*)head);
			}
		}
		else
		{
			return nullptr;
		}

		enlarge_counter_++;
	}

	char *tmp = buf_stack_.back();
	
	buf_stack_.pop_back();
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
	tmp->in_use = 0;
	buf_stack_.push_back((char*)tmp);
	cout << "RecycOneBuf 0x" << (unsigned int)addr << endl;
	return;
	
}