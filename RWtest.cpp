// RWtest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <omp.h>
#include <vector> //for std::vector
#include <numeric> //for std::iota
#include <algorithm> //for std::remove
#include <unordered_set>
#include "Time.h"

//bool isPrime(int number) {
//	int divider = 2;
//
//	int upperBound = int(sqrt(number));
//
//	while (divider <= upperBound) {
//		if (number % divider == 0) {
//			return false;
//		}
//		divider++;
//	}
//
//	return true;
//}
//
//std::vector<int> primesDivide(int a, int b, bool output = true) {
//	Time time;
//
//	std::vector<int> results;
//	results.reserve(b - a + 1);
//
//	for (int i = a; i <= b; i++) {
//		if (isPrime(i)) {
//			results.push_back(i);
//		}
//	}
//
//	time.stop();
//	if (output) std::cout << "primesDivide: " << time.get() << std::endl;
//
//	return results;
//}
//
//std::vector<int> primesSieve(int a, int b) {
//	//Time time;
//	int upperBound = int(sqrt(b));
//	std::vector<int> firstPrimes = primesDivide(2, upperBound, false);
//	std::vector<bool> checked(b + 1, false);
//
//	Time time;
//	for (int i = 0; i < firstPrimes.size(); i++) {
//		//int multiplicity = firstPrimes[i] * 2;
//		//int multiplicity = a - a%firstPrimes[i];
//		int multiplicity = firstPrimes[i];
//
//
//		while (multiplicity <= b) {
//			checked[multiplicity] = true;
//
//			multiplicity += firstPrimes[i];
//		}
//	}
//	time.stop();
//	std::vector<int> primes;
//	primes.reserve(b - a + 1);
//
//	for (int i = a; i <= b; i++) {
//		if (!checked[i]) {
//			primes.push_back(i);
//		}
//	}
//	//time.stop();
//	std::cout << "primesSieve: " << time.get() << std::endl;
//
//	return primes;
//}



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


int main()
{
    std::cout << "Hello World!\n"; 
	//std::vector<int> result = first_sequential(10, 49);
	//print_out_vector(result);

	//int maxThreadsCount = omp_get_max_threads();
	//int threadsCount = omp_get_num_threads();
	//std::cout << "maxThreadsCount " << maxThreadsCount << " \n";
	//std::cout << "threadsCount " << maxThreadsCount << " \n";
	//primesSieve(10, 49);

	second_sequential(10, 49);
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
