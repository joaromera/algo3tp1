#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

#include "knapsack.h"

#define Matrix std::vector<std::vector<int> >

int solveDynamic(int index, int max_weight, Matrix &matrix, Knapsack &knapsack) {
    if (matrix[index][max_weight] != -1) return matrix[index][max_weight];

    std::pair<int, int> item = knapsack[index];

    if (index == 0) { // If last item
        if (item.first <= max_weight) return item.second;
        return 0;
    }

    if (max_weight == 0) return 0;

    if (item.first > max_weight) return solveDynamic(index-1, max_weight, matrix, knapsack);

    matrix[index][max_weight] = std::max(solveDynamic(index-1, max_weight, matrix, knapsack),
                                    solveDynamic(index-1, max_weight - item.first, matrix, knapsack) + item.second);  

    return matrix[index][max_weight];
}

int dynamic(int max_weight, Knapsack &knapsack) {
    Matrix matrix(knapsack.size(), std::vector<int>(max_weight + 1, -1));
    int result = solveDynamic(knapsack.size() - 1, max_weight, matrix, knapsack);
    return result;
}

int main(int argc, char *argv[]) {
    std::string file;
    if (argc > 1) {
        file = argv[1];
    } else {
        return 1;
    }

    Knapsack items = getKnapsackFromFile(file);
    int max_weight = items[0].second;
    items.erase(items.begin());
    items.erase(items.end() - 1);

    auto startDynamic = std::chrono::steady_clock::now();
    unsigned int testDynamic = dynamic(max_weight, items);
    auto endDynamic = std::chrono::steady_clock::now();
    auto diffDynamic = endDynamic - startDynamic;
    auto averageTime = diffDynamic;
    for (int i = 1; i <= 50 - 1; i++) {
        startDynamic = std::chrono::steady_clock::now();
        testDynamic = dynamic(max_weight, items);
        endDynamic = std::chrono::steady_clock::now();
        diffDynamic = endDynamic - startDynamic;
        averageTime += diffDynamic;
    }
    // cout << "Times used by Dynamic " << chrono::duration <double, milli> (diffDynamic).count() << " ms." << " Profit: " << testDynamic << endl;
    printResultsToFile("dynamic", items.size(), max_weight, testDynamic, 50, averageTime);
    return 0;
}