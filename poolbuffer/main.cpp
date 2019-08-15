#include "poolbuffer.h"
#include "observer.h"
#include <thread>
#include <Windows.h>
#include <iostream>

using namespace std;

int main()
{
	BufPool * pool = new BufPool(1024, 20);
	pool->Init();

	Subject * subjectA = new Subject();
	Observer * observerA = new SyncObserver("observerA");
	Observer * observerB = new AsyncObserver("observerB");
	Observer * observerC = new AsyncObserver("observerC");

	subjectA->addObserver(observerA);
	subjectA->addObserver(observerB);
	subjectA->addObserver(observerC);

	thread productor([subjectA, pool] {while (1) {
		BufPtr share_buf = pool->GetBuffer();
		cout << "productor get 0x" << (int)(share_buf->GetBufferPtr()) << " count " << share_buf.use_count() << endl;
		subjectA->notifyObservers(share_buf);
		Sleep(500);
		}
	});
	Sleep(1000);
	thread comsumor2([observerB] {while (1) {
		AsyncObserver* b = (AsyncObserver*)observerB;
		b->ProcWithBlock();
	}
	});

	thread comsumor3([observerC] {while (1) {
		AsyncObserver* c = (AsyncObserver*)observerC;
		Sleep(2000);
		c->ProcWithBlock();
	}
	});

	system("pause");
	return 0;
}

