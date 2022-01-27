#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mutex_id;
condition_variable cond_id;
vector<int> mem;
int cap = 10;

void* producer() {
	while (1) {
		int data = rand() % 1000;
		unique_lock<mutex> locker(mutex_id);
		cond.wait(locker, [](){return mem.size() < cap;});
		mem.push_back(data);
		locker.unlock();
		cond.notify_one();
	}
}

void* consumer() {
	while (1) {
		unique_lock<mutex> locker(mutex_id);
		cond.wait(locker, [](){return mem.size() > 0;});
		cout<<mem.back()<<endl;
		mem.pop_back();
		locker.unlock();
		cond.notify_one();
	}
}

int main() {
	thread thread_id1(producer);
	thread thread_id2(consumer);

	thread_id1.join();
	thread_id2.join();

	return 0;
}