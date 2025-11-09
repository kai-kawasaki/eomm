#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <chrono>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <thread>
#include <iomanip>
#include <sstream>

#include "User.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "ResultsWindow.h"

// generates a dataset of users
std::vector<User> generateDataset(int size) {
    if (size <= 0) return {};
    std::vector<User> users;
    users.reserve(size);

    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine random number generator that i found online as the recommended way to generate such a large dataset
    std::uniform_int_distribution<> distrib(1, 1000);

    for (int i = 0; i < size; ++i) {
        users.push_back({ "User " + std::to_string(i + 1), distrib(gen) });
    }
    return users;
}

// validates if the vector is sorted
bool validateSort(const std::vector<User>& users) {
    return std::is_sorted(users.begin(), users.end());
}

// represents a clickable button
struct Button {
    sf::RectangleShape shape;
    sf::Text text;

    Button(const std::string& label, const sf::Font& font, sf::Vector2f position, sf::Vector2f size)
        : text(font, label, 24) // initialize text in the member initializer list
    {
        shape.setPosition(position);
        shape.setSize(size);
        shape.setFillColor(sf::Color(80, 80, 80));
        shape.setOutlineColor(sf::Color::White);
        shape.setOutlineThickness(2);

        text.setFillColor(sf::Color::White);

        // center text
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin(textBounds.position + textBounds.size / 2.f);
        text.setPosition(position + size / 2.f);
    }

    bool isMouseOver(const sf::RenderWindow& window) const {
        return shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }
};

// updates the results text on the main menu
void updateResultsText(const std::vector<BenchmarkResult>& results, std::vector<sf::Text>& resultsText, const sf::Font& font) {
    resultsText.clear();
    if (results.empty()) return;

    float startY = 150.0f;
    float lineSpacing = 40.0f;
    float startX = 50.0f;

    // create header text
    std::stringstream header_ss;
    header_ss << std::left
              << std::setw(20) << "Dataset Size"
              << std::setw(28) << "Merge Sort Time (ms)"
              << "Quick Sort Time (ms)";

    sf::Text header(font, header_ss.str(), 26);
    header.setFillColor(sf::Color::White);
    header.setPosition({startX, startY});
    resultsText.push_back(header);
    
    // create text for each result line
    for (size_t i = 0; i < results.size(); ++i) {
        const auto& result = results[i];
        std::stringstream line_ss;
        // set floating point precision
        line_ss << std::fixed << std::setprecision(4);

        line_ss << std::left << std::setw(20) << result.datasetSize
                << std::left << std::setw(28) << result.mergeSortTime
                << std::left << result.quickSortTime;

        sf::Text textLine(font, line_ss.str(), 26);
        textLine.setFillColor(sf::Color::White);
        textLine.setPosition({startX, startY + lineSpacing * (i + 1)});
        resultsText.push_back(textLine);
    }
}

// runs all sorting benchmarks
void runAllBenchmarks(std::vector<BenchmarkResult>& results) {
    results.clear(); // clear previous results
    std::cout << "\n--- Running Multiple Speed Comparisons ---\n";
    const std::vector<int> datasetSizes = {1000, 5000, 10000, 50000, 100000, 500000, 1000000, 2000000};

    for (int size : datasetSizes) {
        std::cout << "\n--- Testing with dataset size: " << size << " ---\n";

        // 1 generate the dataset
        auto originalDataset = generateDataset(size);

        // 2 create copies for each sort
        auto dataForMergeSort = originalDataset;
        auto dataForQuickSort = originalDataset;

        // 3 test and time merge sort
        auto startTimeMerge = std::chrono::high_resolution_clock::now();
        mergeSort(dataForMergeSort.begin(), dataForMergeSort.end());
        auto endTimeMerge = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> durationMerge = endTimeMerge - startTimeMerge;
        if (!validateSort(dataForMergeSort)) { std::cout << "Merge Sort validation FAILED.\n"; }
        std::cout << "Merge Sort took " << durationMerge.count() << " milliseconds.\n";

        // 4 test and time quick sort
        auto startTimeQuick = std::chrono::high_resolution_clock::now();
        quickSort(dataForQuickSort.begin(), dataForQuickSort.end());
        auto endTimeQuick = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> durationQuick = endTimeQuick - startTimeQuick;
        if (!validateSort(dataForQuickSort)) { std::cout << "Quick Sort validation FAILED.\n"; }
        std::cout << "Quick Sort took " << durationQuick.count() << " milliseconds.\n";

        results.push_back({size, durationMerge.count(), durationQuick.count()});
    }
}

int main() {
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;

    sf::RenderWindow window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Sorting Algorithm Main Menu");
    window.setFramerateLimit(60);

    // load font
    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        std::cerr << "Error: Could not load font. Make sure font.ttf is in the execution directory.\n";
        return 1;
    }

    // create the two menu buttons
    const sf::Vector2f buttonSize = {350, 80};
    const float margin = 20.0f;
    float startY = margin;

    Button runBenchmarksButton("Run Benchmarks", font, {margin, startY}, buttonSize);
    Button showGraphButton("Show Performance Graph", font, {WINDOW_WIDTH - buttonSize.x - margin, startY}, buttonSize);

    // holds benchmark results
    std::vector<BenchmarkResult> results;

    // holds drawable text for results
    std::vector<sf::Text> resultsText;

    // main application loop
    while (window.isOpen()) {
        // event handling
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left && runBenchmarksButton.isMouseOver(window)) {
                    runAllBenchmarks(results);
                    updateResultsText(results, resultsText, font); // update the display text
                }

                if (mouseButtonPressed->button == sf::Mouse::Button::Left && showGraphButton.isMouseOver(window)) {
                    // run benchmarks if results are empty
                    if (results.empty()) {
                        std::cout << "\n--- No benchmark data found. Running benchmarks first. ---\n";
                        runAllBenchmarks(results);
                        updateResultsText(results, resultsText, font); // update the display text
                    }

                    // show graph window
                    if (!results.empty()) {
                        std::thread resultsThread([font, results]() {
                            ResultsWindow resultsWindow(800, 600, "Benchmark Results", std::move(results), font);
                            resultsWindow.run();
                        });
                        resultsThread.detach(); // detach thread to run window independently
                    }
                }
            }
        }

        // drawing
        window.clear(sf::Color(50, 50, 50)); // dark grey background
        runBenchmarksButton.draw(window);
        showGraphButton.draw(window);
        for (const auto& textLine : resultsText) {
            window.draw(textLine);
        }
        window.display();
    }

    return 0;
}