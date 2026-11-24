#include "sorting.h"
#include "benchmark.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REPETITIONS 5
#define SEED 2026

typedef struct {
    const char *name;
    SortFunction function;
} Algorithm;

static Algorithm algorithms[] = {
    {"Bubble Sort", bubble_sort},
    {"Selection Sort", selection_sort},
    {"Insertion Sort", insertion_sort},
    {"Merge Sort", merge_sort},
    {"Quick Sort", quick_sort}
};

static int read_integer(const char *prompt, int min_value, int max_value)
{
    char line[128], extra;
    int value;

    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL)
            return min_value - 1;

        if (sscanf(line, " %d %c", &value, &extra) == 1 &&
            value >= min_value && value <= max_value)
            return value;

        printf("Invalid input. Enter a number between %d and %d.\n",
               min_value, max_value);
    }
}

static void show_result(BenchmarkResult result, int n)
{
    printf("\nAlgorithm: %s\n", result.name);
    printf("Array size: %d\n", n);
    printf("Average execution time: %.3f ms (%d runs)\n",
           result.average_ms, REPETITIONS);
    if (result.peak_memory_mb >= 0)
        printf("Process peak memory: %.3f MB\n", result.peak_memory_mb);
    else
        printf("Peak memory: unavailable on this system.\n");
    printf("Correctly sorted: %s\n", result.correct ? "YES" : "NO");
}

static void run_one_algorithm(int index)
{
    int n, *original;
    char prompt[100];
    BenchmarkResult result;

    snprintf(prompt, sizeof(prompt), "Enter array size (1-%d): ",
             index < 3 ? 10000 : 1000000);
    n = read_integer(prompt, 1, index < 3 ? 10000 : 1000000);
    if (n < 1)
        return;

    original = generate_array(n, SEED);
    if (original == NULL) {
        printf("Could not allocate the input array.\n");
        return;
    }

    printf("Running %s...\n", algorithms[index].name);
    result = benchmark_sort(algorithms[index].name,
                            algorithms[index].function,
                            original, n, REPETITIONS);
    show_result(result, n);
    free(original);
}

static void compare_all(void)
{
    const int sizes[] = {1000, 5000, 10000, 50000, 100000};
    const int size_count = (int)(sizeof(sizes) / sizeof(sizes[0]));
    FILE *csv;
    int s, a, n, *original;
    BenchmarkResult result;

    csv = fopen("results/benchmark_results.csv", "w");
    if (csv == NULL) {
        printf("Could not open results/benchmark_results.csv for writing.\n");
        printf("Make sure the results directory exists.\n");
        return;
    }

    fprintf(csv, "algorithm,array_size,average_time_ms,process_peak_memory_mb,correct\n");
    printf("\nComparison (average of %d runs)\n", REPETITIONS);
    printf("%-16s %10s %15s %18s %10s\n",
           "Algorithm", "Size", "Time (ms)", "Peak memory (MB)", "Correct");

    for (s = 0; s < size_count; s++) {
        n = sizes[s];
        original = generate_array(n, SEED);
        if (original == NULL) {
            printf("Could not allocate %d elements.\n", n);
            continue;
        }

        for (a = 0; a < 5; a++) {
            if (a < 3 && n > 10000)
                continue;

            result = benchmark_sort(algorithms[a].name,
                                    algorithms[a].function,
                                    original, n, REPETITIONS);

            printf("%-16s %10d %15.3f %18.3f %10s\n",
                   result.name, n, result.average_ms,
                   result.peak_memory_mb,
                   result.correct ? "YES" : "NO");

            fprintf(csv, "\"%s\",%d,%.6f,%.6f,%s\n",
                    result.name, n, result.average_ms,
                    result.peak_memory_mb,
                    result.correct ? "yes" : "no");
        }
        free(original);
    }

    fclose(csv);
    printf("\nCSV results saved to results/benchmark_results.csv\n");
}

int main(void)
{
    int choice;

    while (1) {
        printf("\n========== SORTING ALGORITHMS ==========\n");
        printf("1. Bubble Sort\n");
        printf("2. Selection Sort\n");
        printf("3. Insertion Sort\n");
        printf("4. Merge Sort\n");
        printf("5. Quick Sort\n");
        printf("6. Compare all algorithms\n");
        printf("0. Exit\n");

        choice = read_integer("Your choice: ", 0, 6);

        if (choice == 0) {
            printf("Goodbye!\n");
            break;
        } else if (choice == 6) {
            compare_all();
        } else {
            run_one_algorithm(choice - 1);
        }
    }

    return 0;
}
