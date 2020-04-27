#include <iostream>
#include <omp.h>
#include <vector> //for std::vector
#include <numeric> //for std::iota
#include <algorithm> //for std::remove
#include <unordered_set>
#include <cmath>
#include <set>
#include "Time.h"

#define MAX 500000000
#define THREADS_NUM 12

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

    for (int i = min % 2 ? min : min + 1; i <= max; i += 2) {
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

std::vector<int> first_sequential_v(int min, int max) {
    std::vector<int> result;

    if (min == 2)
        result.push_back(2);

    for (int i = min % 2 ? min : min + 1; i <= max; i += 2) {
        for (int j = 2; j <= ceil(sqrt(i)); j++) {
            if (i % j == 0) {
                break;
            } else if (j == ceil(sqrt(i))) {
                result.push_back(i);
            }
        }
    }

    return result;
}


std::set<int> sequential(int min, int max) {
    std::set<int> primary_numbers = first_sequential(3, ceil(sqrt(max)));
    std::set<int> initial;
    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    for (int x: primary_numbers)
        for (int y = x; x * y < max; y += 2)
            initial.erase(x * y);

    return initial;
}

std::set<int> parallel_1(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    std::set<int> to_erase[THREADS_NUM];

#pragma omp parallel for default(none) private(primary_numbers, max) shared(to_erase)
    for (int x: primary_numbers) {
        int tn = omp_get_thread_num();
        for (int y = x; y * x < max; y += 2)
            to_erase[tn].insert(x * y);
//#pragma omp critical
    }

    for (const auto &te: to_erase) for (int e: te) initial.erase(e);
    return initial;
}

std::set<int> parallel_2(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial)
    for (int x: primary_numbers) {
        std::set<int> to_erase;
        for (int y = x; y * x < max; y += 2)
            to_erase.insert(x * y);
#pragma omp critical
        for (int e: to_erase) initial.erase(e);
    }

    return initial;
}

std::set<int> parallel_3(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    std::set<int> to_erase;

#pragma omp parallel for default(none) private(primary_numbers, max, to_erase) shared(initial)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            to_erase.insert(x * y);
#pragma omp critical
        for (int e: to_erase) initial.erase(e);
    }

    return initial;
}

std::set<int> parallel_4(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);


#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            initial.erase(x * y);
    }

    return initial;
}

std::set<int> parallel_5(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);


#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial) collapse(2)
    for (int x: primary_numbers) {
        for (int y = 3; y < max / 3; y += 2)
            initial.erase(x * y);
    }

    return initial;
}
// Ponieważ wyniki wstępnej analizy do 10M na 12 wątkach są bardzo zbliożone, do dalszej analizy za podstawę wezmę p3 i p4.
//parallel_1 : 47.6234
//parallel_2 : 48.2929
//parallel_3 : 46.1292
//parallel_4 : 46.5982
//parallel_5 : 46.7392

std::set<int> parallel_6(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);


#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial) schedule(static)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            initial.erase(x * y);
    }

    return initial;
}

std::set<int> parallel_7(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);


#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial) schedule(static, 100)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            initial.erase(x * y);
    }

    return initial;
}


std::set<int> parallel_8(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);


#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial) schedule(dynamic) //==dynamic, 1
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            initial.erase(x * y);
    }
    return initial;
}

std::set<int> parallel_9(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);


#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial) schedule(dynamic, 100)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            initial.erase(x * y);
    }

    return initial;
}

std::set<int> parallel_10(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);


#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial) schedule(guided)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            initial.erase(x * y);
    }

    return initial;
}

std::set<int> parallel_11(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);


#pragma omp parallel for default(none) private(primary_numbers, max) shared(initial) schedule(guided, 10)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            initial.erase(x * y);
    }

    return initial;
}

std::set<int> parallel_12(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    std::set<int> to_erase;

#pragma omp parallel for default(none) private(primary_numbers, max, to_erase) shared(initial) schedule(static)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            to_erase.insert(x * y);
#pragma omp critical
        for (int e: to_erase) initial.erase(e);
    }

    return initial;
}

std::set<int> parallel_13(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    std::set<int> to_erase;

#pragma omp parallel for default(none) private(primary_numbers, max, to_erase) shared(initial) schedule(static, 100)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            to_erase.insert(x * y);
#pragma omp critical
        for (int e: to_erase) initial.erase(e);
    }

    return initial;
}

std::set<int> parallel_14(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    std::set<int> to_erase;

#pragma omp parallel for default(none) private(primary_numbers, max, to_erase) shared(initial) schedule(dynamic)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            to_erase.insert(x * y);
#pragma omp critical
        for (int e: to_erase) initial.erase(e);
    }

    return initial;
}

std::set<int> parallel_15(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    std::set<int> to_erase;

#pragma omp parallel for default(none) private(primary_numbers, max, to_erase) shared(initial) schedule(dynamic, 100)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            to_erase.insert(x * y);
#pragma omp critical
        for (int e: to_erase) initial.erase(e);
    }

    return initial;
}

std::set<int> parallel_16(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    std::set<int> to_erase;

#pragma omp parallel for default(none) private(primary_numbers, max, to_erase) shared(initial) schedule(guided)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            to_erase.insert(x * y);
#pragma omp critical
        for (int e: to_erase) initial.erase(e);
    }

    return initial;
}

std::set<int> parallel_17(int min, int max) {
    std::vector<int> primary_numbers = first_sequential_v(3, ceil(sqrt(max)));
    std::set<int> initial;

    for (int i = min % 2 ? min : min + 1; i < max; i += 2) initial.insert(initial.end(), i);
    if (min == 2) initial.insert(initial.end(), 2);

    std::set<int> to_erase;

#pragma omp parallel for default(none) private(primary_numbers, max, to_erase) shared(initial) schedule(guided, 10)
    for (int x: primary_numbers) {
        for (int y = x; y * x < max; y += 2)
            to_erase.insert(x * y);
#pragma omp critical
        for (int e: to_erase) initial.erase(e);
    }

    return initial;
}

void tests() {
    FILE *f = fopen("results.csv", "w");
    std::set<int> result;
    Time t = Time();

    result = sequential(2, MAX);
    t.stop();
    fprintf(f, "'sequential', 1, %f\n", t.get());
    printf("'sequential', 1, %f\n", t.get());

    omp_set_num_threads(1);
    t.start();
    result = parallel_1(2, MAX);
    t.stop();
    fprintf(f, "'parallel_1', 1, %f\n", t.get());
    printf("'parallel_1', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_1(2, MAX);
    t.stop();
    fprintf(f, "'parallel_1', 3, %f\n", t.get());
    printf("'parallel_1', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_1(2, MAX);
    t.stop();
    fprintf(f, "'parallel_1', 6, %f\n", t.get());
    printf("'parallel_1', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_1(2, MAX);
    t.stop();
    fprintf(f, "'parallel_1', 12, %f\n", t.get());
    printf("'parallel_1', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_2(2, MAX);
    t.stop();
    fprintf(f, "'parallel_2', 1, %f\n", t.get());
    printf("'parallel_2', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_2(2, MAX);
    t.stop();
    fprintf(f, "'parallel_2', 3, %f\n", t.get());
    printf("'parallel_2', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_2(2, MAX);
    t.stop();
    fprintf(f, "'parallel_2', 6, %f\n", t.get());
    printf("'parallel_2', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_2(2, MAX);
    t.stop();
    fprintf(f, "'parallel_2', 12, %f\n", t.get());
    printf("'parallel_2', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_3(2, MAX);
    t.stop();
    fprintf(f, "'parallel_3', 1, %f\n", t.get());
    printf("'parallel_3', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_3(2, MAX);
    t.stop();
    fprintf(f, "'parallel_3', 3, %f\n", t.get());
    printf("'parallel_3', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_3(2, MAX);
    t.stop();
    fprintf(f, "'parallel_3', 6, %f\n", t.get());
    printf("'parallel_3', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_3(2, MAX);
    t.stop();
    fprintf(f, "'parallel_3', 12, %f\n", t.get());
    printf("'parallel_3', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_4(2, MAX);
    t.stop();
    fprintf(f, "'parallel_4', 1, %f\n", t.get());
    printf("'parallel_4', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_4(2, MAX);
    t.stop();
    fprintf(f, "'parallel_4', 3, %f\n", t.get());
    printf("'parallel_4', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_4(2, MAX);
    t.stop();
    fprintf(f, "'parallel_4', 6, %f\n", t.get());
    printf("'parallel_4', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_4(2, MAX);
    t.stop();
    fprintf(f, "'parallel_4', 12, %f\n", t.get());
    printf("'parallel_4', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_5(2, MAX);
    t.stop();
    fprintf(f, "'parallel_5', 1, %f\n", t.get());
    printf("'parallel_5', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_5(2, MAX);
    t.stop();
    fprintf(f, "'parallel_5', 3, %f\n", t.get());
    printf("'parallel_5', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_5(2, MAX);
    t.stop();
    fprintf(f, "'parallel_5', 6, %f\n", t.get());
    printf("'parallel_5', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_5(2, MAX);
    t.stop();
    fprintf(f, "'parallel_5', 12, %f\n", t.get());
    printf("'parallel_5', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_6(2, MAX);
    t.stop();
    fprintf(f, "'parallel_6', 1, %f\n", t.get());
    printf("'parallel_6', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_6(2, MAX);
    t.stop();
    fprintf(f, "'parallel_6', 3, %f\n", t.get());
    printf("'parallel_6', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_6(2, MAX);
    t.stop();
    fprintf(f, "'parallel_6', 6, %f\n", t.get());
    printf("'parallel_6', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_6(2, MAX);
    t.stop();
    fprintf(f, "'parallel_6', 12, %f\n", t.get());
    printf("'parallel_6', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_7(2, MAX);
    t.stop();
    fprintf(f, "'parallel_7', 1, %f\n", t.get());
    printf("'parallel_7', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_7(2, MAX);
    t.stop();
    fprintf(f, "'parallel_7', 3, %f\n", t.get());
    printf("'parallel_7', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_7(2, MAX);
    t.stop();
    fprintf(f, "'parallel_7', 6, %f\n", t.get());
    printf("'parallel_7', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_7(2, MAX);
    t.stop();
    fprintf(f, "'parallel_7', 12, %f\n", t.get());
    printf("'parallel_7', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_8(2, MAX);
    t.stop();
    fprintf(f, "'parallel_8', 1, %f\n", t.get());
    printf("'parallel_8', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_8(2, MAX);
    t.stop();
    fprintf(f, "'parallel_8', 3, %f\n", t.get());
    printf("'parallel_8', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_8(2, MAX);
    t.stop();
    fprintf(f, "'parallel_8', 6, %f\n", t.get());
    printf("'parallel_8', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_8(2, MAX);
    t.stop();
    fprintf(f, "'parallel_8', 12, %f\n", t.get());
    printf("'parallel_8', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_9(2, MAX);
    t.stop();
    fprintf(f, "'parallel_9', 1, %f\n", t.get());
    printf("'parallel_9', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_9(2, MAX);
    t.stop();
    fprintf(f, "'parallel_9', 3, %f\n", t.get());
    printf("'parallel_9', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_9(2, MAX);
    t.stop();
    fprintf(f, "'parallel_9', 6, %f\n", t.get());
    printf("'parallel_9', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_9(2, MAX);
    t.stop();
    fprintf(f, "'parallel_9', 12, %f\n", t.get());
    printf("'parallel_9', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_10(2, MAX);
    t.stop();
    fprintf(f, "'parallel_10', 1, %f\n", t.get());
    printf("'parallel_10', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_10(2, MAX);
    t.stop();
    fprintf(f, "'parallel_10', 3, %f\n", t.get());
    printf("'parallel_10', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_10(2, MAX);
    t.stop();
    fprintf(f, "'parallel_10', 6, %f\n", t.get());
    printf("'parallel_10', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_10(2, MAX);
    t.stop();
    fprintf(f, "'parallel_10', 12, %f\n", t.get());
    printf("'parallel_10', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_11(2, MAX);
    t.stop();
    fprintf(f, "'parallel_11', 1, %f\n", t.get());
    printf("'parallel_11', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_11(2, MAX);
    t.stop();
    fprintf(f, "'parallel_11', 3, %f\n", t.get());
    printf("'parallel_11', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_11(2, MAX);
    t.stop();
    fprintf(f, "'parallel_11', 6, %f\n", t.get());
    printf("'parallel_11', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_11(2, MAX);
    t.stop();
    fprintf(f, "'parallel_11', 12, %f\n", t.get());
    printf("'parallel_11', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_12(2, MAX);
    t.stop();
    fprintf(f, "'parallel_12', 1, %f\n", t.get());
    printf("'parallel_12', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_12(2, MAX);
    t.stop();
    fprintf(f, "'parallel_12', 3, %f\n", t.get());
    printf("'parallel_12', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_12(2, MAX);
    t.stop();
    fprintf(f, "'parallel_12', 6, %f\n", t.get());
    printf("'parallel_12', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_12(2, MAX);
    t.stop();
    fprintf(f, "'parallel_12', 12, %f\n", t.get());
    printf("'parallel_12', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_13(2, MAX);
    t.stop();
    fprintf(f, "'parallel_13', 1, %f\n", t.get());
    printf("'parallel_13', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_13(2, MAX);
    t.stop();
    fprintf(f, "'parallel_13', 3, %f\n", t.get());
    printf("'parallel_13', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_13(2, MAX);
    t.stop();
    fprintf(f, "'parallel_13', 6, %f\n", t.get());
    printf("'parallel_13', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_13(2, MAX);
    t.stop();
    fprintf(f, "'parallel_13', 12, %f\n", t.get());
    printf("'parallel_13', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_14(2, MAX);
    t.stop();
    fprintf(f, "'parallel_14', 1, %f\n", t.get());
    printf("'parallel_14', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_14(2, MAX);
    t.stop();
    fprintf(f, "'parallel_14', 3, %f\n", t.get());
    printf("'parallel_14', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_14(2, MAX);
    t.stop();
    fprintf(f, "'parallel_14', 6, %f\n", t.get());
    printf("'parallel_14', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_14(2, MAX);
    t.stop();
    fprintf(f, "'parallel_14', 12, %f\n", t.get());
    printf("'parallel_14', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_15(2, MAX);
    t.stop();
    fprintf(f, "'parallel_15', 1, %f\n", t.get());
    printf("'parallel_15', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_15(2, MAX);
    t.stop();
    fprintf(f, "'parallel_15', 3, %f\n", t.get());
    printf("'parallel_15', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_15(2, MAX);
    t.stop();
    fprintf(f, "'parallel_15', 6, %f\n", t.get());
    printf("'parallel_15', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_15(2, MAX);
    t.stop();
    fprintf(f, "'parallel_15', 12, %f\n", t.get());
    printf("'parallel_15', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_16(2, MAX);
    t.stop();
    fprintf(f, "'parallel_16', 1, %f\n", t.get());
    printf("'parallel_16', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_16(2, MAX);
    t.stop();
    fprintf(f, "'parallel_16', 3, %f\n", t.get());
    printf("'parallel_16', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_16(2, MAX);
    t.stop();
    fprintf(f, "'parallel_16', 6, %f\n", t.get());
    printf("'parallel_16', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_16(2, MAX);
    t.stop();
    fprintf(f, "'parallel_16', 12, %f\n", t.get());
    printf("'parallel_16', 12, %f\n", t.get());


    omp_set_num_threads(1);
    t.start();
    result = parallel_17(2, MAX);
    t.stop();
    fprintf(f, "'parallel_17', 1, %f\n", t.get());
    printf("'parallel_17', 1, %f\n", t.get());


    omp_set_num_threads(3);
    t.start();
    result = parallel_17(2, MAX);
    t.stop();
    fprintf(f, "'parallel_17', 3, %f\n", t.get());
    printf("'parallel_17', 3, %f\n", t.get());


    omp_set_num_threads(6);
    t.start();
    result = parallel_17(2, MAX);
    t.stop();
    fprintf(f, "'parallel_17', 6, %f\n", t.get());
    printf("'parallel_17', 6, %f\n", t.get());


    omp_set_num_threads(12);
    t.start();
    result = parallel_17(2, MAX);
    t.stop();
    fprintf(f, "'parallel_17', 12, %f\n", t.get());
    printf("'parallel_17', 12, %f\n", t.get());

}

int main() {
    tests();
}

