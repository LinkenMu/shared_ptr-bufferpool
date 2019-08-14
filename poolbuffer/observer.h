#pragma once
#include "poolbuffer.h"
#include <string>
#include <vector>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
class Observer
{
public:
	Observer() {};
	virtual ~Observer() {};
	// 定义纯虚函数，规范接口
	virtual void update(BufPtr ptr) = 0;
};

class Subject
{
public:
	Subject() {};
	virtual ~Subject() {};
	void addObserver(Observer *observer);
	void deleteObserver(Observer *observer);
	void notifyObservers(BufPtr ptr);
private:
	vector<Observer*> m_observers;
};


class SyncObserver : public Observer
{

public:
	SyncObserver(string name) :m_observerName(name){};
	~SyncObserver() {};
	virtual void update(BufPtr ptr);

private:
	string m_observerName;
	Subject *m_subject;
};

class AsyncObserver : public Observer
{

public:
	AsyncObserver(string name) :m_observerName(name) {};
	~AsyncObserver() {};
	virtual void update(BufPtr ptr);
	void ProcWithBlock();

private:
	queue<BufPtr> ptr_queue;
	string m_observerName;
	mutex mutex_;
	condition_variable_any cond;
};