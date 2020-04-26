#include <iostream>
#include <omp.h>
#include <vector> //for std::vector
#include <numeric> //for std::iota
#include <algorithm> //for std::remove
#include <unordered_set>
#include <cmath>
#include <set>
#include "Time.h"

template<typename T>
void print_out_set(const T &set) {
    for (int i : set)
        std::cout << i << ' ';
    std::cout << std::endl;
    fflush(stdout);
}

std::set<int> first_sequential(int min, int max) {
    std::set<int> result;

    if (min == 2) {
        result.insert(2);
    }

    for (int i = min; i <= max; i++) {
        for (int j = 2; j <= ceil(sqrt(i)); j++) {
            if (i % j == 0) {
                break;
            } else if (j == ceil(sqrt(i))) {
                result.insert(i);
            }
        }
    }

    return result;
}


std::set<int> second_sequential(int min, int max) {
    std::set<int> initial;
    for (int i = min; i < max; i++) initial.insert(i);
    std::set<int> primary_numbers = first_sequential(2, ceil(sqrt(max)));
    std::set<int> numbers_to_erase;

    print_out_set<std::set<int>>(primary_numbers);

    for (int i : initial) {
        for (int primary_number : primary_numbers) {
            if (i % primary_number == 0) {
                numbers_to_erase.insert(i);
                break;
            }
        }
    }

    std::set<int> result;

    std::set_difference(initial.begin(), initial.end(), numbers_to_erase.begin(), numbers_to_erase.end(),
                        inserter(result, result.end()));
    print_out_set<std::set<int>>(result);

    return result;
}


int main() {
    std::set<int> result = second_sequential(2, 1000);
    //print_out_set(result);
//
//	int maxThreadsCount = omp_get_max_threads();
//	int threadsCount = omp_get_num_threads();
//	std::cout << "maxThreadsCount " << maxThreadsCount << " \n";
//	std::cout << "threadsCount " << maxThreadsCount << " \n";
    //primesSieve(10, 49);

//	second_sequential(10, 49);
}

