#pragma once

#include <vector>
#include <iterator>
#include <algorithm>

template<typename RandomIt>
void mergeSort(RandomIt first, RandomIt last);

template<typename RandomIt>
void merge(RandomIt first, RandomIt middle, RandomIt last)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;
    std::vector<T> left_half(first, middle);
    std::vector<T> right_half(middle, last);

    auto left_it = left_half.begin();
    auto right_it = right_half.begin();
    auto result_it = first;

    // merge the two halves back into the original range
    while (left_it != left_half.end() && right_it != right_half.end()) {
        if (*left_it < *right_it) {
            *result_it = *left_it;
            ++left_it;
        } else {
            *result_it = *right_it;
            ++right_it;
        }
        ++result_it;
    }

    // copy any remaining elements from the left half
    std::copy(left_it, left_half.end(), result_it);

    // copy remaining elements from right
    std::copy(right_it, right_half.end(), result_it);
}

template<typename RandomIt>
void mergeSort(RandomIt first, RandomIt last)
{
    auto size = std::distance(first, last);
    if (size > 1) {
        RandomIt middle = std::next(first, size / 2);

        mergeSort(first, middle);
        mergeSort(middle, last);

        merge(first, middle, last);
    }
}