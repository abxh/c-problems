#include "histogram.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

void histogram_par_mutex_cpp(const size_t k, size_t* bins, const size_t n, const int64_t* is) {

    const auto num_cores = std::max(1u, std::thread::hardware_concurrency());

    std::vector<std::thread> threads(num_cores);

    {
        const size_t chunk_size = num_cores;

        auto const f = [chunk_size, k, &bins](const size_t id) {
            const size_t begin = id * chunk_size;
            const size_t end = std::min(begin + chunk_size, k);

            for (size_t i = begin; i < end; i++) {
                bins[i] = 0;
            }
        };

        for (size_t i = 0; i < num_cores; i++) {
            threads[i] = std::thread(f, i);
        }
        for (size_t i = 0; i < num_cores; i++) {
            threads[i].join();
        }
    }
    {
        std::vector<std::mutex> bin_locks(k);

        const size_t chunk_size = num_cores;

        auto const f = [chunk_size, n, k, &bins, &bin_locks, &is](const size_t id) {
            const size_t begin = id * chunk_size;
            const size_t end = std::min(begin + chunk_size, n);

            for (size_t i = begin; i < end; i++) {
                const int index = is[i]; // read from read-only array

                if (0 <= index && index < (int64_t)std::min<size_t>(INT64_MAX, k)) {
                    bin_locks[index].lock();
                    const size_t x_old = bins[index];
                    const size_t x_new = x_old + 1;
                    bins[index] = x_new;
                    bin_locks[index].unlock();
                }
            }
        };

        for (size_t i = 0; i < num_cores; i++) {
            threads[i] = std::thread(f, i);
        }
        for (size_t i = 0; i < num_cores; i++) {
            threads[i].join();
        }
    }
}

extern "C" void histogram_par_mutex(const size_t k, size_t* bins, const size_t n, const int64_t* is) {
    try {
        histogram_par_mutex_cpp(k, bins, n, is);
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        abort();
    } catch (...) {
        std::cerr << "error occurred.\n";
        abort();
    }
}
