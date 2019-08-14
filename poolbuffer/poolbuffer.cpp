// poolbuffer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "poolbuffer.h"
#define DEBUG

//#ifdef DEBUG
//#define PRINT_DEBUG(fmt,args...) printf(fmt,##args)
//#else
//#define PRINT_DEBUG(fmt,args...)
//#endif // DEBUG


using namespace std;

BufPool::BufPool(int unit_size, int unit_num)
{
	ring_ = new ringbuf(unit_size, unit_num);
}
int BufPool::Init()
{
	 ring_->Init();
	 return 0;
}

BufPtr BufPool::GetBuffer()
{	
	return std::make_shared<BufHandle>(ring_);

}

BufPool::~BufPool()
{
	delete ring_;
}


BufHandle::BufHandle(ringbuf *ring) : bufptr_(nullptr)
{
	ring_ = ring;
}

BufHandle::~BufHandle()
{
	ring_->RecycOneBuf(bufptr_);
}

char* BufHandle::GetBufferPtr()
{
	if (bufptr_ == nullptr)
	{
		bufptr_ = ring_->GetOneBuf();
	}
	return bufptr_;
}



