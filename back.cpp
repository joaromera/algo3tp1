#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>
#include <vector>

#include "knapsack.h"

int solveBacktracking(int solution, int index, int max_weight, const Knapsack &knapsack)
{
    if (index == knapsack.size() || max_weight == 0)
    {
        return solution;
    }
    else if (max_weight < knapsack[index].first)
    {
        ++index;
        return solveBacktracking(solution, index, max_weight, knapsack);
    }
    else
    { 
        int next_item = index + 1;
        return std::max(
            solveBacktracking(solution + knapsack[index].second, next_item, max_weight - knapsack[index].first, knapsack),
            solveBacktracking(solution, next_item, max_weight, knapsack)
        );
    }
}

int backtracking(const int &max_weight, const Knapsack &knapsack)
{
    return solveBacktracking(0, 0, max_weight, knapsack);
}

int main(int argc, char *argv[])
{
    if (argc < 2) return 1;

    const std::string file = argv[1];

    Knapsack items = getKnapsackFromFile(file);
    const int max_weight = items[0].second;
    items.erase(items.begin());
    items.erase(items.end() - 1);

    unsigned solution = backtracking(max_weight, items);
    auto averageTime = benchmarkKnapsackAlgorithm(backtracking, items, max_weight, 25);
    // cout << "Time used by Backtracking " << chrono::duration <double, milli> (diffBacktracking).count() << " ms." << " Profit: " << test_backtracking << endl;
    printResultsToFile("backtracking", items.size(), max_weight, solution, 25, averageTime);
    return 0;
}
