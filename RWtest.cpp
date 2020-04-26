#include <iostream>
#include <omp.h>
#include <vector> //for std::vector
#include <numeric> //for std::iota
#include <algorithm> //for std::remove
#include <unordered_set>
#include <cmath>
#include "Time.h"


void print_out_vector(std::vector<int> vec) {
	for (std::vector<int>::const_iterator i = vec.begin(); i != vec.end(); ++i)
		std::cout << *i << ' ';
}

std::vector<int> first_sequential(int min, int max) {

	std::vector<int> result;

	if (min == 2) {
		result.push_back(2);
	}

	for (int i = min; i <= max; i++) {
		for (int j = 2; j <= ceil(sqrt(i)); j++) {
			if (i % j == 0) {
				break;
			}
			else if (j == ceil(sqrt(i))) {
				result.push_back(i);
			}
		}
	}

	return result;
}

template<typename T>
void remove_intersection(std::vector<T>& a, std::vector<T>& b) {
	std::unordered_multiset<T> st;
	st.insert(a.begin(), a.end());
	st.insert(b.begin(), b.end());
	auto predicate = [&st](const T & k) { return st.count(k) > 1; };
	a.erase(std::remove_if(a.begin(), a.end(), predicate), a.end());
	b.erase(std::remove_if(b.begin(), b.end(), predicate), b.end());
}

std::vector<int> second_sequential(int min, int max) {

	std::vector<int> result(max-min+1);
	std::iota(result.begin(), result.end(), min);
	std::vector<int> primary_numbers = first_sequential(2, ceil(sqrt(max)));
	std::vector<int> numbers_to_remove;

	print_out_vector(primary_numbers);

	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < primary_numbers.size(); j++) {
			if (result[i] % primary_numbers[j] == 0) {
				numbers_to_remove.push_back(result[i]);
				break;
			}
		}
	}

	remove_intersection(result, numbers_to_remove);
	print_out_vector(result);

	return result;
}


int main() {
    //std::vector<int> result = first_sequential(10, 49);
    //print_out_vector(result);
//
//	int maxThreadsCount = omp_get_max_threads();
//	int threadsCount = omp_get_num_threads();
//	std::cout << "maxThreadsCount " << maxThreadsCount << " \n";
//	std::cout << "threadsCount " << maxThreadsCount << " \n";
    //primesSieve(10, 49);

//	second_sequential(10, 49);
}

