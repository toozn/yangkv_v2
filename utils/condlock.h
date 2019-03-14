#include <pthread.h>
#include <unistd.h>
#pragma once
namespace leveldb{

class CondLock {
public:
	CondLock() {
		mutex_ = PTHREAD_MUTEX_INITIALIZER;
		cond_ = PTHREAD_COND_INITIALIZER;
	}
	~CondLock() {
		pthread_cond_destroy(&cond_);
		pthread_mutex_destroy(&mutex_);
	}
	void Lock() {
		pthread_mutex_lock(&mutex_);
	}
	void Unlock() {
		pthread_mutex_unlock(&mutex_);
	}
	void Wait() {
		pthread_cond_wait(&cond_, &mutex_);
	}
	void Signal() {
		pthread_cond_signal(&cond_);
	}
	void Boardcast() {
		pthread_cond_broadcast(&cond_);
	}
private:
	pthread_cond_t cond_;
	pthread_mutex_t mutex_;
};

}