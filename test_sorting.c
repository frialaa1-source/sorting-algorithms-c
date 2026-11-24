#include "../src/sorting.h"
#include "../src/utils.h"
#include <stdio.h>

typedef void (*SortFunction)(int[], int);

int main(void)
{
    SortFunction sorts[] = {
        bubble_sort, selection_sort, insertion_sort, merge_sort, quick_sort
    };
    const char *names[] = {
        "Bubble", "Selection", "Insertion", "Merge", "Quick"
    };
    int test_data[] = {5, -2, 8, 1, 3, 3, 0, -9};
    int expected[] = {-9, -2, 0, 1, 3, 3, 5, 8};
    int i, j, a[8], ok;

    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++)
            a[j] = test_data[j];

        sorts[i](a, 8);
        ok = 1;
        for (j = 0; j < 8; j++) {
            if (a[j] != expected[j]) {
                ok = 0;
                break;
            }
        }
        printf("%s Sort: %s\n", names[i], ok ? "PASS" : "FAIL");
        if (!ok)
            return 1;
    }
    return 0;
}
