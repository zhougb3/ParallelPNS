#ifndef PROCESSPOOL_H
#define PROCESSPOOL_H
#include "Process.h"
#include <unistd.h>

class ProcessPool {
public:
	static ProcessPool * creatPool(int number);
	~ProcessPool();
	int getIndex() {
		return index;	
	}
	Process * getSubProcess() {
		return sub_process;
	}

private:
	ProcessPool(int number);
	Process * sub_process;
	int process_number;
	static ProcessPool  * instance;
	int index;
};
#endif // !PROCESSPOOL_H

