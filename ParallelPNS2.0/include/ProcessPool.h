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
	int getProcessNumber() {
		return process_number;
	}
private:
	ProcessPool(int number);
	Process * sub_process;
	int process_number; // 子进程数量
	static ProcessPool  * instance; //单实例
	int index;
};
#endif // !PROCESSPOOL_H

