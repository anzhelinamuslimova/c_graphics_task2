#include "edu_search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t edu_linear_search(const void *ptr, size_t count, size_t size, const void *key, edu_cmp cmp) {
    const char *base = (const char *)ptr;

    for (size_t i = 0; i < count; i++) {
        const void *element = base + i * size;
        if (cmp(element, key) == 0) {
            return i;  // найден элемент
        }
    }
    return (size_t)-1;  // не найден
}

size_t edu_binary_search(const void *ptr, size_t count, size_t size, const void *key, edu_cmp cmp) {
    const char *base = (const char *)ptr;
    size_t left = 0;
    size_t right = count;

    while (left < right) {
        size_t mid = left + (right - left) / 2;
        const void *element = base + mid * size;

        int res = cmp(element, key);
        if (res == 0) {
            return mid;  // найден
        } else if (res < 0) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return (size_t)-1;  // не найден
}
