#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <time.h>
#include<sys/wait.h>
#include "../include/ProcessPool.h"
#include "../include/PrimeNumber.h"
using namespace std;

void writeToFile(vector<int> & nums, int size, clock_t finish);

int main() {
	int size = 5;
	while(!(cin >> size) || size < 0 && size > 9){}
	PrimeNumber prime_number(size);
	vector<int> & result = prime_number.getPrime(); 
	if (result.size() != 0) {
		clock_t finish = clock();	
		writeToFile(result,size, finish);
	}
}

void writeToFile(vector<int> & nums, int size, clock_t finish) {
	string name = "./result/" + to_string(size) + "/prime.txt";	
	ofstream outFile;  
	outFile.open(name.c_str(),ios::trunc);	
	if (!outFile) {
		cerr << "write file " << name << " error!" <<endl;
		return;
	}
	for (auto num:nums)
		outFile << num << " " <<endl;
	double duration = (double)finish / CLOCKS_PER_SEC;
	outFile << endl << "the program's run time is " << duration <<endl;
}
