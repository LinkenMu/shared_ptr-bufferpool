#pragma once
#include <list>
#include <mutex>
struct Header {
	short flag;
	char reverse;
	char in_use;
};

class ringbuf
{
public:
	ringbuf(int unit_size, int unit_num);
	~ringbuf();
	void Init();
	char *GetOneBuf();
	void RecycOneBuf(char * addr);
	void StatusPrint();
private:
	char * buffer_addr_;
	int unit_size_;
	int capacity_;
	std::list<char*> product_list_;
	std::list<char*> customer_list_;
	std:: mutex mutex_;

	
};

