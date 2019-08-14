#pragma once
#include "ringbuf.h"
#include <memory>
#include <list>
#include <mutex>
using namespace std;
class BufHandle {
public:
	BufHandle(ringbuf* ring);
	~BufHandle();
	char *GetBufferPtr();
private:
	ringbuf* ring_;
	char *bufptr_;
};

using BufPtr = std::shared_ptr<BufHandle>;

class BufPool {
public:
BufPool(int unit_size, int unit_num);
~BufPool();
BufPool& operator =(const BufPool &) = delete;
int Init();
BufPtr GetBuffer();

private:
	list<BufPtr> buf_list_;
	mutex mutex_;
	ringbuf* ring_;
};

