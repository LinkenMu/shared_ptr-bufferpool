#pragma once
#include <list>
#include <vector>
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
	int raw_capacity_;
	int enlarge_counter_;
	std::vector<char*> buf_stack_;
	std:: mutex mutex_;

	
};

