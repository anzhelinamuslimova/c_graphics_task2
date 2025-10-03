#include "edu_sort.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define UNIMPLEMENTED \
    do { \
        fprintf(stderr, "[ERROR] function %s is unimplemented\n", __func__); \
        abort(); \
    } while (0)

#define UNUSED(x) (void)(x)

static void edu_swap(void *a, void *b, size_t size) {
    char tmp[size];
    memcpy(tmp, a, size);
    memcpy(a, b, size);
    memcpy(b, tmp, size);
}

// ---------- Bubble Sort ----------
void edu_bubble_sort(void *ptr, size_t count, size_t size, edu_cmp cmp) {
    int swapped = 1;
    while (swapped) {
        swapped = 0;
        for (size_t i = 0; i < count - 1; ++i) {
            void *val1 = (char *) ptr + i * size;
            void *val2 = (char *) ptr + (i + 1) * size;
            if (cmp(val1, val2) > 0) {
                edu_swap(val1, val2, size);
                swapped = 1;
            }
        }
        --count;
    }
}

// ---------- Selection Sort ----------
void edu_selection_sort(void *ptr, size_t count, size_t size, edu_cmp cmp) {
    char *base = (char *)ptr;
    for (size_t i = 0; i < count - 1; i++) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < count; j++) {
            if (cmp(base + j * size, base + min_idx * size) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            edu_swap(base + i * size, base + min_idx * size, size);
        }
    }
}

// ---------- Insertion Sort ----------
void edu_insertion_sort(void *ptr, size_t count, size_t size, edu_cmp cmp) {
    char *base = (char *)ptr;
    for (size_t i = 1; i < count; i++) {
        char key[size];
        memcpy(key, base + i * size, size);
        size_t j = i;
        while (j > 0 && cmp(base + (j - 1) * size, key) > 0) {
            memcpy(base + j * size, base + (j - 1) * size, size);
            j--;
        }
        memcpy(base + j * size, key, size);
    }
}

// ---------- Shell Sort ----------
void edu_shell_sort(void *ptr, size_t count, size_t size, edu_cmp cmp) {
    char *base = (char *)ptr;
    for (size_t gap = count / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < count; i++) {
            char temp[size];
            memcpy(temp, base + i * size, size);
            size_t j = i;
            while (j >= gap && cmp(base + (j - gap) * size, temp) > 0) {
                memcpy(base + j * size, base + (j - gap) * size, size);
                j -= gap;
            }
            memcpy(base + j * size, temp, size);
        }
    }
}

// ---------- Quick Sort ----------
static void quick_sort_rec(char *base, size_t left, size_t right, size_t size, edu_cmp cmp) {
    if (left >= right) return;

    size_t mid = left + (right - left) / 2;
    char *pivot = base + mid * size;

    size_t i = left, j = right;
    while (i <= j) {
        while (cmp(base + i * size, pivot) < 0) i++;
        while (cmp(base + j * size, pivot) > 0) j--;
        if (i <= j) {
            edu_swap(base + i * size, base + j * size, size);
            i++; j--;
        }
    }
    if (left < j) quick_sort_rec(base, left, j, size, cmp);
    if (i < right) quick_sort_rec(base, i, right, size, cmp);
}

void edu_quick_sort(void *ptr, size_t count, size_t size, edu_cmp cmp) {
    if (count > 0)
        quick_sort_rec((char *)ptr, 0, count - 1, size, cmp);
}

// ---------- Heap Sort ----------
static void heapify(char *base, size_t n, size_t i, size_t size, edu_cmp cmp) {
    size_t largest = i;
    size_t l = 2 * i + 1;
    size_t r = 2 * i + 2;

    if (l < n && cmp(base + l * size, base + largest * size) > 0)
        largest = l;
    if (r < n && cmp(base + r * size, base + largest * size) > 0)
        largest = r;

    if (largest != i) {
        edu_swap(base + i * size, base + largest * size, size);
        heapify(base, n, largest, size, cmp);
    }
}

void edu_heap_sort(void *ptr, size_t count, size_t size, edu_cmp cmp) {
    char *base = (char *)ptr;

    for (ssize_t i = (ssize_t)count / 2 - 1; i >= 0; i--) {
        heapify(base, count, i, size, cmp);
    }

    for (ssize_t i = (ssize_t)count - 1; i > 0; i--) {
        edu_swap(base, base + i * size, size);
        heapify(base, i, 0, size, cmp);
    }
}

// ---------- Counting Sort (для int) ----------
void edu_counting_sort(void *ptr, size_t count, size_t size, edu_counting_sort_order_e order) {
    if (size != sizeof(int)) {
        fprintf(stderr, "[ERROR] counting sort supports only int\n");
        abort();
    }

    int *arr = (int *)ptr;
    int min = arr[0], max = arr[0];
    for (size_t i = 1; i < count; i++) {
        if (arr[i] < min) min = arr[i];
        if (arr[i] > max) max = arr[i];
    }

    size_t range = (size_t)(max - min + 1);
    int *count_arr = calloc(range, sizeof(int));
    int *output = malloc(count * sizeof(int));

    for (size_t i = 0; i < count; i++) {
        count_arr[arr[i] - min]++;
    }

    if (order == EDU_COUNTING_SORT_ASC) {
        for (size_t i = 1; i < range; i++) {
            count_arr[i] += count_arr[i - 1];
        }
        for (ssize_t i = (ssize_t)count - 1; i >= 0; i--) {
            output[--count_arr[arr[i] - min]] = arr[i];
        }
    } else {
        for (size_t i = 1; i < range; i++) {
            count_arr[i] += count_arr[i - 1];
        }
        for (size_t i = 0; i < count; i++) {
            output[--count_arr[arr[i] - min]] = arr[i];
        }
        // reverse
        for (size_t i = 0; i < count / 2; i++) {
            int tmp = output[i];
            output[i] = output[count - 1 - i];
            output[count - 1 - i] = tmp;
        }
    }

    memcpy(arr, output, count * sizeof(int));
    free(count_arr);
    free(output);
}
