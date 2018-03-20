#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <time.h>
#include<sys/wait.h>
#include "../include/ProcessPool.h"

using namespace std;

void writeFile(string name, vector<bool> & nums, int low_bound, int sub_range);
void writeAllPrime(int size, int process_number);
void getPrime(int size);

int main() {
	int size = 5;
	while(!(cin >> size) || size < 0 && size > 9){}
	getPrime(size);
	return 0;
}
void getPrime(int size) {
	int range = pow(2, size) * 1000;
	int sub_process_number = sqrt(range) / 2;
	ProcessPool * my_pool = ProcessPool::creatPool(sub_process_number);
	if (my_pool->getIndex() == -1) {
		int first_range = range / (sub_process_number + 1) + range % (sub_process_number + 1);
		vector<bool> nums(first_range + 1, true);
		cout << first_range << "daixao"<< endl;
		for (int i = 2; i < nums.size(); ++i) {
			int loc = i;
			if (nums[loc]) {
				for (int j = 0; j < sub_process_number; ++j) {
					write(my_pool->getSubProcess()[j].getfds()[1], &loc,4);
				}
				loc += i;
				while(loc <= first_range) {
					nums[loc] = false;
					loc +=i;
				}
			}		
		}
		for (int j = 0; j < sub_process_number; ++j) {
			close(my_pool->getSubProcess()[j].getfds()[1]);
		}
		string name = "./result/" + to_string(size) + "/sub_process_0.txt";
		writeFile(name, nums, 0, first_range + 1);
		pid_t wpid;
		int status;
		while ((wpid = wait(&status)) > 0);
		writeAllPrime(size, sub_process_number);		
		
	} else {
		int low_bound = (range / (sub_process_number + 1)) * (my_pool->getIndex() + 1) + range % (sub_process_number + 1) + 1;
		int sub_range = range / (sub_process_number + 1);
		int * target = new int();
		vector<bool> nums(sub_range,true);
		while(1) {
			ssize_t s = read(my_pool->getSubProcess()[my_pool->getIndex()].getfds()[0],target,4);
			if (s > 0) {
				int loc = (low_bound / *target) * (*target) == low_bound ? low_bound:(low_bound / *target) * (*target) + (*target);
				while(loc < (low_bound + sub_range)) {
					nums[loc - low_bound] = false;
					loc += *target;
				}
			} else {
				break;
			}
		}
		string name = "./result/" + to_string(size) + "/sub_process_" + to_string(my_pool->getIndex() + 1) + ".txt";
		writeFile(name, nums, low_bound, sub_range);
	}
}
void writeFile(string name, vector<bool> & nums, int low_bound, int sub_range) {
	ofstream outFile;  
	outFile.open(name.c_str(),ios::trunc);
	if (!outFile) {
		cerr << "write file " << name << " error!" <<endl;
		return;
	}
	int i = (low_bound == 0 ? 2:0);		
	for (; i < sub_range; ++i) {
		if (nums[i])
			outFile << (i + low_bound) << " ";
	}
	outFile << endl;
	clock_t finish = clock();
	double duration = (double)finish / CLOCKS_PER_SEC;
	outFile << " runTime: " << duration <<endl;
}

void writeAllPrime(int size, int process_number) {
	string max_time;
	string name = "./result/" + to_string(size) + "/allPrime.txt";
	ofstream outFile;
	outFile.open(name.c_str(),ios::trunc);
	if (!outFile) {
		cerr << "write file " << name << " error!" <<endl;
		return;
	}	
	for (int i = 0; i<= process_number; ++i) {
		string read_name = "./result/" + to_string(size) + "/sub_process_" + to_string(i) + ".txt";
		ifstream inFile;
		inFile.open(read_name.c_str());
		if (!inFile) {
			cerr << "read file " << read_name << " error!" <<endl;
			return;
		}
		string temp;
		while(!inFile.eof()) {
			inFile >> temp;
			if (temp == "runTime:")
				break;
			outFile << temp  <<"  "<< endl;
		}
		inFile >> temp;
		if (temp > max_time)
			max_time = temp;		
	}
	outFile << endl << "the program's run time is " << max_time <<endl;
}

