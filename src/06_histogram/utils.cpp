#include "utils.h"

#include <algorithm>
#include <thread>

// C version:
//
// #ifdef _WIN32
// #include "windows.h"
// #else
// #include "unistd.h"
// #endif
//
// unsigned int get_num_cores(void) {
// #ifdef _WIN32
//     SYSTEM_INFO info;
//     GetSystemInfo(&info);
//     return info.dwNumberOfProcessors;
// #else
//     long n = sysconf(_SC_NPROCESSORS_ONLN);
//     return (n > 0) ? (unsigned int)n : 1;
// #endif
// }

extern "C" unsigned int get_num_cores(void) {
    unsigned int const n = std::thread::hardware_concurrency();
    return std::max(1u, n);
}
