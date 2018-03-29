#include "../include/ProcessPool.h"
#include <iostream>
ProcessPool * ProcessPool::instance = nullptr;
ProcessPool::ProcessPool(int number) :process_number(number),index(-1) {
	sub_process = new Process[process_number];
	for (int i = 0; i < process_number; ++i) {
		if(pipe(sub_process[i].getFds_sub_read()) < 0 || pipe(sub_process[i].getFds_sub_write()) < 0) {
			std::cerr << "pipe error";  
		}
		pid_t sub = fork();
    	if( sub < 0 ){  
			std::cerr << "fork error";  
    	}  
		else if (sub > 0) {
			sub_process[i].setId(sub);
			close(sub_process[i].getFds_sub_read()[0]);
			close(sub_process[i].getFds_sub_write()[1]);			
		}
		else {
			close(sub_process[i].getFds_sub_read()[1]);
			close(sub_process[i].getFds_sub_write()[0]);
			index = i;
			break;		
		}
	}
}
ProcessPool::~ProcessPool() {
	delete[] sub_process;
}
ProcessPool * ProcessPool::creatPool(int number) {
	if (instance == nullptr) {
		instance = new ProcessPool(number);
	}
	return instance;
}
