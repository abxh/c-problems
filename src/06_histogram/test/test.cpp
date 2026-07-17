#include <cassert>
#include <iostream>
#include <random>

#include "./../histogram.h"

int main(void) {
    std::random_device rd;
    std::mt19937 gen(rd());

    auto test = [&rd, &gen](const std::size_t N, std::size_t M) {
        std::vector<std::int64_t> inds;
        inds.reserve(N);

        std::uniform_int_distribution<std::int64_t> distrib(-1, M);
        for (std::size_t i = 0; i < N; i++) {
            inds.emplace_back(distrib(gen));
        }

        std::vector<std::size_t> seq_bins(M);
        histogram_seq(seq_bins.size(), seq_bins.data(), inds.size(), inds.data());

        std::vector<std::size_t> par_mutex_bins(M);
        histogram_par_mutex(par_mutex_bins.size(), par_mutex_bins.data(), inds.size(), inds.data());

        std::vector<std::size_t> par_atomic_bins(M);
        histogram_par_atomic(par_atomic_bins.size(), par_atomic_bins.data(), inds.size(), inds.data());

        std::vector<std::size_t> par_atomic_bins2(M);
        histogram_par_atomic_optimal(par_atomic_bins2.size(), par_atomic_bins2.data(), inds.size(), inds.data());

        for (std::size_t i = 0; i < M; i++) {
            assert(seq_bins[i] == par_mutex_bins[i]);
            assert(seq_bins[i] == par_atomic_bins[i]);
            assert(seq_bins[i] == par_atomic_bins2[i]);
        }
    };

    test(1000000, 1);
    test(1000000, 2);
    test(1000000, 500);
    test(1000000, 1000000);

    test(1, 1);
    test(1, 2);
    test(1, 500);
    test(1, 1000000);

    std::cout << "implementations give consistent result\n";
}
