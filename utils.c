#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int *generate_array(int n, unsigned int seed)
{
    int *a, i;
    if (n <= 0)
        return NULL;

    a = (int *)malloc((size_t)n * sizeof(int));
    if (a == NULL)
        return NULL;

    srand(seed);
    for (i = 0; i < n; i++)
        a[i] = rand() % 200001 - 100000; /* [-100000, 100000] */

    return a;
}

int *copy_array(const int source[], int n)
{
    int *copy;
    if (n <= 0)
        return NULL;

    copy = (int *)malloc((size_t)n * sizeof(int));
    if (copy == NULL)
        return NULL;

    for (int i = 0; i < n; i++)
        copy[i] = source[i];

    return copy;
}

int is_sorted(const int a[], int n)
{
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i])
            return 0;
    }
    return 1;
}

void print_array(const int a[], int n)
{
    for (int i = 0; i < n; i++)
        printf("%d%s", a[i], (i == n - 1) ? "\n" : " ");
}
