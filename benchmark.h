#ifndef BENCHMARK_H
#define BENCHMARK_H

typedef void (*SortFunction)(int[], int);

typedef struct {
    const char *name;
    double average_ms;
    double peak_memory_mb;
    int correct;
} BenchmarkResult;

BenchmarkResult benchmark_sort(const char *name, SortFunction sort_fn,
                               const int original[], int n, int repetitions);
double get_peak_memory_mb(void);

#endif
