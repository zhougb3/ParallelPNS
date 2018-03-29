#ifndef PROCESS_H
#define PROCESS_H
#include <sys/types.h>

class Process {
public:
	Process() :m_pid(-1){ }
	void setId(pid_t id) {
		m_pid = id;
	}
	int * getFds_sub_read() {
		return fds_sub_read;	
	}
	int * getFds_sub_write() {
		return fds_sub_write;
	}
private:
	pid_t m_pid; // 进程Id
	int fds_sub_read[2];  // 管道文件描述符，子进程读，父进程写。
	int fds_sub_write[2]; // 管道文件描述符，子进程写，父进程读。
};
#endif // !PROCESS_H

