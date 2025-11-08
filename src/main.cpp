#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <string>
#include <algorithm>

#include "User.h"
#include "MergeSort.h"
#include "QuickSort.h"

// Function to generate a vector of users with random ranks
std::vector<User> generateDataset(int size) {
    std::vector<User> users;
    users.reserve(size);

    // Modern C++ random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 1000);

    for (int i = 0; i < size; ++i) {
        users.push_back({"User " + std::to_string(i + 1), distrib(gen)});
    }
    return users;
}

// Function to validate if the data is sorted
bool validateSort(const std::vector<User>& users) {
    return std::is_sorted(users.begin(), users.end());
}

int main() {
    const int DATASET_SIZE = 100000;

    // 1. Generate the dataset
    std::cout << "Generating a dataset with " << DATASET_SIZE << " users...\n\n";
    auto originalDataset = generateDataset(DATASET_SIZE);

    // 2. Create copies for each sort
    auto dataForMergeSort = originalDataset;
    auto dataForQuickSort = originalDataset;

    // 3. Test and time Merge Sort
    std::cout << "--- Running Merge Sort ---\n";
    auto startTimeMerge = std::chrono::high_resolution_clock::now();
    mergeSort(dataForMergeSort.begin(), dataForMergeSort.end());
    auto endTimeMerge = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> durationMerge = endTimeMerge - startTimeMerge;

    if (validateSort(dataForMergeSort)) {
        std::cout << "Merge Sort validation successful: Data is sorted correctly.\n";
    } else {
        std::cout << "Merge Sort validation FAILED: Data is not sorted correctly.\n";
    }
    std::cout << "Merge Sort took " << durationMerge.count() << " microseconds.\n\n";

    // 4. Test and time Quick Sort
    std::cout << "--- Running Quick Sort ---\n";
    auto startTimeQuick = std::chrono::high_resolution_clock::now();
    quickSort(dataForQuickSort.begin(), dataForQuickSort.end());
    auto endTimeQuick = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::micro> durationQuick = endTimeQuick - startTimeQuick;

    if (validateSort(dataForQuickSort)) {
        std::cout << "Quick Sort validation successful: Data is sorted correctly.\n";
    } else {
        std::cout << "Quick Sort validation FAILED: Data is not sorted correctly.\n";
    }
    std::cout << "Quick Sort took " << durationQuick.count() << " microseconds.\n\n";

    return 0;
}