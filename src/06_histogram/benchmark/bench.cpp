#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "./../histogram.h"

int main(void) {
    std::random_device rd;
    std::mt19937 gen(rd());

    constexpr int RUNS = 20;

    static auto benchmark = [](auto&& func, int runs) {
        std::vector<double> times;
        times.reserve(runs);

        for (int i = 0; i < runs; i++) {
            auto start = std::chrono::high_resolution_clock::now();

            func();

            auto end = std::chrono::high_resolution_clock::now();

            double ms = std::chrono::duration<double, std::milli>(end - start).count();

            times.push_back(ms);
        }

        double min = *std::min_element(times.begin(), times.end());
        double max = *std::max_element(times.begin(), times.end());
        double avg = std::accumulate(times.begin(), times.end(), 0.0) / times.size();

        return std::tuple{min, avg, max};
    };

    auto test = [&rd, &gen](const std::size_t N, std::size_t M) {
        std::vector<std::int64_t> inds;
        inds.reserve(N);

        std::uniform_int_distribution<std::int64_t> distrib(-1, M - 1);
        for (std::size_t i = 0; i < N; i++) {
            inds.emplace_back(distrib(gen));
        }

        std::vector<std::size_t> seq_bins(M);
        std::vector<std::size_t> par_mutex_bins(M);
        std::vector<std::size_t> par_atomic_bins(M);
        std::vector<std::size_t> par_atomic_bins2(M);

        auto [smin, savg, smax] = benchmark(
            [&] {
                histogram_seq(seq_bins.size(), seq_bins.data(), inds.size(), inds.data());
            },
            RUNS);

        auto [mmin, mavg, mmax] = benchmark(
            [&] {
                histogram_par_mutex(par_mutex_bins.size(), par_mutex_bins.data(), inds.size(), inds.data());
            },
            RUNS);

        auto [amin, aavg, amax] = benchmark(
            [&] {
                histogram_par_atomic(par_atomic_bins.size(), par_atomic_bins.data(), inds.size(), inds.data());
            },
            RUNS);

        auto [a2min, a2avg, a2max] = benchmark(
            [&] {
                histogram_par_atomic_optimal(par_atomic_bins2.size(), par_atomic_bins2.data(), inds.size(), inds.data());
            },
            RUNS);

        std::cout << "num_indices=" << N << " num_bins=" << M << "\n\n";

        std::cout << "seq:\n"
                  << "  min: " << smin << " ms\n"
                  << "  avg: " << savg << " ms\n"
                  << "  max: " << smax << " ms\n\n";

        std::cout << "mutex:\n"
                  << "  min: " << mmin << " ms\n"
                  << "  avg: " << mavg << " ms\n"
                  << "  max: " << mmax << " ms\n\n";

        std::cout << "atomic:\n"
                  << "  min: " << amin << " ms\n"
                  << "  avg: " << aavg << " ms\n"
                  << "  max: " << amax << " ms\n\n";

        std::cout << "atomic optimal:\n"
                  << "  min: " << a2min << " ms\n"
                  << "  avg: " << a2avg << " ms\n"
                  << "  max: " << a2max << " ms\n\n";
    };

    test(10000000, 100);
    test(10000000, 10000);
    test(10000000, 10000000);
}
