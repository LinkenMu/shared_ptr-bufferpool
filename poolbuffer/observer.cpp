#include "observer.h"
#include <iostream>
void Subject::addObserver(Observer *observer)
{
	m_observers.push_back(observer);
}

void Subject::deleteObserver(Observer * observer)
{
	for (vector<Observer*>::iterator iter = m_observers.begin();
		iter != m_observers.end(); iter++)
	{
		if (*iter == observer)
		{
			m_observers.erase(iter);
			return;
		}
	}
}

void Subject::notifyObservers(BufPtr ptr) {
	for (vector<Observer*>::iterator iter = m_observers.begin();
		iter != m_observers.end();
		iter++)
	{
		(*iter)->update(ptr);
	}
}


void SyncObserver::update(BufPtr ptr)
{
	cout << "update observer " << m_observerName << "  0x"<< (int)ptr->GetBufferPtr() << " count "<< ptr.use_count() << endl;
}

void AsyncObserver::update(BufPtr ptr)
{
	lock_guard<mutex> locker(mutex_);
	ptr_queue.push(ptr);
	cond.notify_all();

}

void AsyncObserver::ProcWithBlock()
{
	lock_guard<mutex> locker(mutex_);
	cond.wait(mutex_);
	auto ptr = ptr_queue.front();
	ptr->GetBufferPtr();
	ptr_queue.pop();
	cout << "update observer " << m_observerName << "  0x" << (int)ptr->GetBufferPtr() << " count " << ptr.use_count() << endl;
}