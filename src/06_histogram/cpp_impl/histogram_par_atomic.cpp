#include "../histogram.h"

#include <atomic>
#include <cstdint>

#include <algorithm>
#include <iostream>
#include <thread>
#include <vector>

void histogram_par_atomic_cpp(const size_t num_bins, uint64_t* bins_out, const size_t num_indices, const int64_t* is) {
    if (!std::atomic<uint64_t>::is_always_lock_free) {
        throw std::runtime_error{"uint64 atomic support is not present as desired"};
    }

    const size_t num_cores = std::max(1u, std::thread::hardware_concurrency());

    std::vector<std::thread> threads(num_cores);
    std::vector<std::atomic<uint64_t>> bins(num_bins);

    {
        const size_t chunk_size = (num_bins + num_cores - 1) / num_cores;

        auto const f = [chunk_size, num_bins, &bins](const uint64_t id) {
            const size_t begin = id * chunk_size;
            const size_t end = std::min(begin + chunk_size, num_bins);

            for (size_t i = begin; i < end; i++) {
                bins[i].store(0);
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
        const size_t chunk_size = (num_indices + num_cores - 1) / num_cores;

        auto const f = [chunk_size, num_indices, num_bins, &bins, &is](const size_t id) {
            const size_t begin = id * chunk_size;
            const size_t end = std::min(begin + chunk_size, num_indices);

            for (size_t i = begin; i < end; i++) {
                const int64_t index = is[i]; // read from read-only array

                if (0 <= index && index < (int64_t)std::min<uint64_t>(INT64_MAX, num_bins)) {
                    uint64_t x_old = bins[index].load();
                    uint64_t x_new;
                    do {
                        x_new = x_old + 1;
                        // compare_exchange():
                        // if (bins[index] = x_old) {
                        //      bins[index] = x_new
                        //      return true;
                        // } else {
                        //      x_old = bins[index]
                        //      return false;
                        // }
                    } while (!bins[index].compare_exchange_weak(x_old, x_new));
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
    {
        const size_t chunk_size = (num_bins + num_cores - 1) / num_cores;

        auto const f = [chunk_size, num_bins, &bins, &bins_out](const uint64_t id) {
            const size_t begin = id * chunk_size;
            const size_t end = std::min(begin + chunk_size, num_bins);

            for (size_t i = begin; i < end; i++) {
                bins_out[i] = bins[i].load();
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

extern "C" void histogram_par_atomic(const size_t k, uint64_t* bins, const size_t n, const int64_t* is) {
    try {
        histogram_par_atomic_cpp(k, bins, n, is);
    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        std::abort();
    } catch (...) {
        std::cerr << "error occurred.\n";
        std::abort();
    }
}
