#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

struct Item {
    int weight;
    int profit;
};

bool compareByWeight(const Item &a, const Item &b) {
    return a.weight < b.weight;
}

bool compareByProfit(const Item &a, const Item &b) {
    return a.profit > b.profit;
}

bool compareByRatio(const Item &a, const Item &b) {
    double ratioA = static_cast<double>(a.profit) / a.weight;
    double ratioB = static_cast<double>(b.profit) / b.weight;
    return ratioA > ratioB;
}

int knapsack(vector<Item> &items, int capacity, bool (*compareFunction)(const Item&, const Item&)) {
    sort(items.begin(), items.end(), compareFunction);

    int n = items.size();
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= capacity; ++w) {
            if (items[i - 1].weight <= w) {
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - items[i - 1].weight] + items[i - 1].profit);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][capacity];
}

int main() {
    // Generate random inputs
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> weightDist(1, 10);
    uniform_int_distribution<int> profitDist(1, 20);

    // Define input sizes
    vector<int> inputSizes = {10, 50, 100, 200, 500};

    cout << setw(15) << "Input Size" << setw(25) << "Time (ns) Weight" << setw(25) << "Time (ns) Profit" << setw(25) << "Time (ns) Ratio" << endl;

    for (int size : inputSizes) {
        // Generate random items for each input size
        vector<Item> items(size);
        for (int i = 0; i < size; ++i) {
            items[i].weight = weightDist(gen);
            items[i].profit = profitDist(gen);
        }

        auto startWeight = high_resolution_clock::now();
        knapsack(items, size / 2, compareByWeight);
        auto stopWeight = high_resolution_clock::now();
        auto durationWeight = duration_cast<nanoseconds>(stopWeight - startWeight);

        auto startProfit = high_resolution_clock::now();
        knapsack(items, size / 2, compareByProfit);
        auto stopProfit = high_resolution_clock::now();
        auto durationProfit = duration_cast<nanoseconds>(stopProfit - startProfit);

        auto startRatio = high_resolution_clock::now();
        knapsack(items, size / 2, compareByRatio);
        auto stopRatio = high_resolution_clock::now();
        auto durationRatio = duration_cast<nanoseconds>(stopRatio - startRatio);

        cout << setw(15) << size
             << setw(25) << durationWeight.count()
             << setw(25) << durationProfit.count()
             << setw(25) << durationRatio.count() << endl;
    }

    return 0;
}
