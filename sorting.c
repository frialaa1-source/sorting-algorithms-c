#include "sorting.h"
#include <stdlib.h>

void bubble_sort(int a[], int n)
{
    int i, j, temp, swapped;

    for (i = 0; i < n - 1; i++) {
        swapped = 0;
        for (j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped)
            break;
    }
}

void selection_sort(int a[], int n)
{
    int i, j, min_index, temp;

    for (i = 0; i < n - 1; i++) {
        min_index = i;
        for (j = i + 1; j < n; j++) {
            if (a[j] < a[min_index])
                min_index = j;
        }
        if (min_index != i) {
            temp = a[i];
            a[i] = a[min_index];
            a[min_index] = temp;
        }
    }
}

void insertion_sort(int a[], int n)
{
    int i, j, key;

    for (i = 1; i < n; i++) {
        key = a[i];
        j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

/* Merge the sorted ranges [left, mid] and [mid + 1, right]. */
static void merge(int a[], int temp[], int left, int mid, int right)
{
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        if (a[i] <= a[j])
            temp[k++] = a[i++];
        else
            temp[k++] = a[j++];
    }
    while (i <= mid)
        temp[k++] = a[i++];
    while (j <= right)
        temp[k++] = a[j++];

    for (i = left; i <= right; i++)
        a[i] = temp[i];
}

static void merge_sort_recursive(int a[], int temp[], int left, int right)
{
    int mid;
    if (left >= right)
        return;

    mid = left + (right - left) / 2;
    merge_sort_recursive(a, temp, left, mid);
    merge_sort_recursive(a, temp, mid + 1, right);
    merge(a, temp, left, mid, right);
}

void merge_sort(int a[], int n)
{
    int *temp;
    if (n <= 1)
        return;

    temp = (int *)malloc((size_t)n * sizeof(int));
    if (temp == NULL) {
        /* Allocation failure: leave the input unchanged. */
        return;
    }
    merge_sort_recursive(a, temp, 0, n - 1);
    free(temp);
}

/* Lomuto partition, using the last element as the pivot. */
static int partition(int a[], int low, int high)
{
    int pivot = a[high];
    int i = low - 1;
    int j, temp;

    for (j = low; j < high; j++) {
        if (a[j] <= pivot) {
            i++;
            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }
    temp = a[i + 1];
    a[i + 1] = a[high];
    a[high] = temp;
    return i + 1;
}

static void quick_sort_recursive(int a[], int low, int high)
{
    int pivot_index;
    if (low < high) {
        pivot_index = partition(a, low, high);
        quick_sort_recursive(a, low, pivot_index - 1);
        quick_sort_recursive(a, pivot_index + 1, high);
    }
}

void quick_sort(int a[], int n)
{
    if (n > 1)
        quick_sort_recursive(a, 0, n - 1);
}
