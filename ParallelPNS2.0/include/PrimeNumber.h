#ifndef PRIMENUMBER_H
#define PRIMENUMBER_H
#include "../include/ProcessPool.h"
#include <vector>
#include <cmath>
class PrimeNumber {
public:
    PrimeNumber(int size = 0):m_size(size) {}
    ~PrimeNumber(){}
    std::vector<int> & getPrime();
private:
    int m_size; //要计算的素数范围
    std::vector<int> prime_set; // 存放计算的素数集合
    ProcessPool * my_pool; //用来计算素数的进程池
    std::vector<bool>  first_process_get_prime(int first_range); // 父进程计算得到素数
    std::vector<bool>  sub_process_get_prime(int low_bound, int sub_range); // 子进程计算得到素数
    void first_process_read_prime(std::vector<bool> & nums); // 父进程通过管道读取所有子进程素数
    void sub_process_write_prime(std::vector<bool> & nums,int low_bound); //子进程将素数写回管道
};
#endif // !PRIMENUMBER_H

