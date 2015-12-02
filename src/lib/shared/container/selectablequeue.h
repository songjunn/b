#ifndef _SELECTABLEQUEUE_H_
#define _SELECTABLEQUEUE_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <queue>
#include <vector>
#include "Lock.h"

// Selectable queue, multi writers, single reader
template <class T>
class SelectableQueue{
private:
	int fds[2];
public:
	Mutex mutex;
	std::queue<T> items;

	SelectableQueue();
	~SelectableQueue();
	int fd(){
		return fds[0];
	}
	int size();
	// multi writer
	int push(const T item);
	// single reader
	int pop(T *data);
};


template <class T>
SelectableQueue<T>::SelectableQueue(){
	if(pipe(fds) == -1){
		exit(0);
	}
}

template <class T>
SelectableQueue<T>::~SelectableQueue(){
	close(fds[0]);
	close(fds[1]);
}

template <class T>
int SelectableQueue<T>::size(){
	Locking l(&mutex);
	return items.size();
}

template <class T>
int SelectableQueue<T>::push(const T item){
	Locking l(&mutex);
	items.push(item);
	if(::write(fds[1], "1", 1) == -1){
		exit(0);
	}
	return 1;
}

template <class T>
int SelectableQueue<T>::pop(T *data){
	int n, ret = 1;
	char buf[1];

	while(1){
		n = ::read(fds[0], buf, 1);
		if(n < 0){
			if(errno == EINTR){
				continue;
			}else{
				return -1;
			}
		}else if(n == 0){
			ret = -1;
		}else{
			Locking l(&mutex);
			if(items.empty()){
				fprintf(stderr, "%s %d error!\n", __FILE__, __LINE__);
				return -1;
			}
			*data = items.front();
			items.pop();
		}
		break;
	}
	return ret;
}

#endif //_SELECTABLEQUEUE_H_

