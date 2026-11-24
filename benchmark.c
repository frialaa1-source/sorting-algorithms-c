#include "benchmark.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif

double get_peak_memory_mb(void)
{
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS info;
    info.cb = sizeof(info);
    if (GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info)))
        return (double)info.PeakWorkingSetSize / (1024.0 * 1024.0);
    return -1.0;
#else
    struct rusage usage;
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
#ifdef __APPLE__
        return (double)usage.ru_maxrss / (1024.0 * 1024.0);
#else
        return (double)usage.ru_maxrss / 1024.0;
#endif
    }
    return -1.0;
#endif
}

BenchmarkResult benchmark_sort(const char *name, SortFunction sort_fn,
                               const int original[], int n, int repetitions)
{
    BenchmarkResult result;
    double total_ms = 0.0;
    double peak_mb = -1.0;
    int run, *working;
    clock_t start, end;

    result.name = name;
    result.average_ms = 0.0;
    result.peak_memory_mb = -1.0;
    result.correct = 1;

    for (run = 0; run < repetitions; run++) {
        working = copy_array(original, n);
        if (working == NULL) {
            fprintf(stderr, "Memory allocation failed for %s.\n", name);
            result.correct = 0;
            return result;
        }

        start = clock();
        sort_fn(working, n);
        end = clock();

        if (start == (clock_t)-1 || end == (clock_t)-1) {
            fprintf(stderr, "Could not read the clock for %s.\n", name);
            free(working);
            result.correct = 0;
            return result;
        }

        total_ms += 1000.0 * (double)(end - start) / CLOCKS_PER_SEC;
        if (!is_sorted(working, n))
            result.correct = 0;

        free(working);

        {
            double current_mb = get_peak_memory_mb();
            if (current_mb > peak_mb)
                peak_mb = current_mb;
        }
    }

    result.average_ms = total_ms / repetitions;
    result.peak_memory_mb = peak_mb;
    return result;
}
