#pragma once

#include <vector>
#include <iterator>
#include <algorithm> // For std::iter_swap

template<typename RandomIt>
RandomIt partition(RandomIt first, RandomIt last)
{
    auto pivot = std::prev(last, 1);
    auto i = first;

    for (auto j = first; j != pivot; ++j)
    {
        if (*j < *pivot)
        {
            std::iter_swap(i++, j);
        }
    }
    std::iter_swap(i, pivot);
    return i;
}

template<typename RandomIt>
void quickSort(RandomIt first, RandomIt last)
{
    if (std::distance(first, last) > 1)
    {
        RandomIt p = partition(first, last);
        quickSort(first, p);
        quickSort(std::next(p), last);
    }
}