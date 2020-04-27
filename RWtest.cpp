#include <iostream>
#include <omp.h>
#include <vector> //for std::vector
#include <numeric> //for std::iota
#include <algorithm> //for std::remove
#include <unordered_set>
#include <cmath>
#include <set>
#include "Time.h"

#define MAX 10000000

template<typename T>
void print_out_set(const T &set) {
    for (int i : set)
        std::cout << i << ' ';
    std::cout << std::endl;
    fflush(stdout);
}

std::set<int> first_sequential(int min, int max) {
    std::set<int> result;

    if (min == 2)
        result.insert(2);

    for (int i = min; i <= max; i++) {
        if (i % 2 == 0) continue;
        for (int j = 3; j <= ceil(sqrt(i)); j += 2) {
            if (i % j == 0) {
                break;
            } else if (ceil(sqrt(i)) - j < 2) {
                result.insert(i);
            }
        }
    }

    return result;
}


std::set<int> second_sequential(int min, int max) {
    std::set<int> primary_numbers = first_sequential(3, ceil(sqrt(max)));
    std::set<int> initial;
    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(i);
    if (min == 2) initial.insert(2);

    for (int x: primary_numbers)
        for (int y = x; x * y < max; y += 2)
            initial.erase(x * y);

    return initial;
}


int main() {
    Time t = Time();
    std::set<int> result = second_sequential(2, MAX);
    t.stop();
    std::cout << t.get() << std::endl;


//    print_out_set<std::set<int>>(result);
//
//	int maxThreadsCount = omp_get_max_threads();
//	int threadsCount = omp_get_num_threads();
//	std::cout << "maxThreadsCount " << maxThreadsCount << " \n";
//	std::cout << "threadsCount " << maxThreadsCount << " \n";
    //primesSieve(10, 49);

//	second_sequential(10, 49);
}

