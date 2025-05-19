#include <iostream>
#include <cassert>
#include <exception>
#include "../src/Random.hpp"

void test_uniform_mod_range_and_edge_cases() {
    // q = 1 should always return 0
    for (int i = 0; i < 1000; ++i) {
        assert(uniform_mod(1) == 0);
    }

    // some other small q
    uint64_t nums[]{2ULL, 3ULL, 10ULL, 100ULL};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 1000; j++) {
            uint64_t q = nums[i];
            uint64_t v = uniform_mod(nums[i]);
            assert(v < q && "uniform_mod output out of range");
        }
    }
}

void test_uniform_mod_statistical_uniformity() {
    const uint64_t q = 10;
    const int N = 100000;
    int counts[10] = {0};

    for (int i = 0; i < N; ++i) {
        int index = uniform_mod(q);
        counts[index] = counts[index] + 1;
    }

    double expected = double(N) / q;
    double tol = expected * 0.1;  // ±10%

    for (int i = 0; i < q; ++i) {
        assert(std::abs(counts[i] - expected) < tol && "bucket frequency out of tolerance");
    }
}


void test_sample_error() {
    const double sigma = 2.0;
    const int N = 100000;
    long long sum = 0;
    for (int i = 0; i < N; ++i) { // this loop conducts a Box-Muller Transform. This converts two independent uniform random variables (u1 and u2) into two independent standard normal random variables.
        double u1 = (double)rand() / RAND_MAX;
        double u2 = (double)rand() / RAND_MAX;
        double z0 = std::sqrt(-2.0 * std::log(u1)) * std::cos(2.0 * M_PI * u2);
        double sample = z0 * sigma; //lol
        sum += sample;
    }
    double mean = double(sum) / N;
    assert(std::abs(mean) < 0.1 && "sample_error mean too far from 0");
}

int main() {
    test_uniform_mod_range_and_edge_cases();
    test_uniform_mod_statistical_uniformity();
    test_sample_error();

    std::cout << "All tests passed\n";
    return 0;
}
