#include "../include/PrimeNumber.h"

std::vector<bool>  PrimeNumber::first_process_get_prime(int first_range) {
    std::vector<bool> nums(first_range + 1, true);
    for (int i = 2; i < nums.size(); ++i) {
        int loc = i;
        if (nums[loc]) {
            for (int j = 0; j < my_pool->getProcessNumber(); ++j) {
                write(my_pool->getSubProcess()[j].getFds_sub_read()[1], &loc,4);
            }
            loc += i;
            while(loc <= first_range) {
                nums[loc] = false;
                loc +=i;
            }
        }		
    }
    for (int j = 0; j < my_pool->getProcessNumber(); ++j) {
        close(my_pool->getSubProcess()[j].getFds_sub_read()[1]);
    }
    return nums;
}

std::vector<bool>  PrimeNumber::sub_process_get_prime(int low_bound, int sub_range) {
    int * target = new int();
    std::vector<bool> nums(sub_range,true);
    while(1) {
        ssize_t s = read(my_pool->getSubProcess()[my_pool->getIndex()].getFds_sub_read()[0],target,4);
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
    delete [] target;
    close(my_pool->getSubProcess()[my_pool->getIndex()].getFds_sub_read()[0]);
    return nums;
}

void PrimeNumber::first_process_read_prime(std::vector<bool> & nums) {
    for (int i = 2; i < nums.size(); ++i) {
        if (nums[i])
            prime_set.push_back(i);
    }
    int * target = new int();
    for (int j = 0; j < my_pool->getProcessNumber(); ++j) {
        while(1) {
            ssize_t s = read(my_pool->getSubProcess()[j].getFds_sub_write()[0],target,4);
            if (s > 0) {
                prime_set.push_back(*target);
            } else {
                break;
            }
        }
        close(my_pool->getSubProcess()[j].getFds_sub_write()[0]);						
    }
    delete [] target;
}
void PrimeNumber::sub_process_write_prime(std::vector<bool> & nums, int low_bound) {
     for (int i = 0;i < nums.size(); ++i) {
        if (nums[i]) {
            int index = i + low_bound;
            write(my_pool->getSubProcess()[my_pool->getIndex()].getFds_sub_write()[1], &index,4);				
        }
    }
    close(my_pool->getSubProcess()[my_pool->getIndex()].getFds_sub_write()[1]);	   
} 
std::vector<int> & PrimeNumber::getPrime() {
    int range = pow(2, m_size) * 1000;
	int sub_process_number = sqrt(range) / 2;
	my_pool = ProcessPool::creatPool(sub_process_number);
    //父进程执行的操作
	if (my_pool->getIndex() == -1) {
		int first_range = range / (sub_process_number + 1) + range % (sub_process_number + 1);
        std::vector<bool>  nums = first_process_get_prime(first_range); 
        first_process_read_prime(nums);
	}
    //子进程执行的操作 
    else {
		int low_bound = (range / (sub_process_number + 1)) * (my_pool->getIndex() + 1) + range % (sub_process_number + 1) + 1;
		int sub_range = range / (sub_process_number + 1);
        std::vector<bool>  nums = sub_process_get_prime(low_bound, sub_range);
        sub_process_write_prime(nums,low_bound);
	}
    return prime_set;   
}