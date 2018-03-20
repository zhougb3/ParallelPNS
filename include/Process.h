#ifndef PROCESS_H
#define PROCESS_H
#include <sys/types.h>

class Process {
public:
	Process() :m_pid(-1){ }
	void setId(pid_t id) {
		m_pid = id;
	}
	int * getfds() {
		return fds;	
	}

private:
	pid_t m_pid;
	int fds[2];
};
#endif // !PROCESS_H

