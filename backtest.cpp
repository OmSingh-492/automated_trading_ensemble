/*
clang++ -std=c++11 backtest.cpp -o a.out
./a.out
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <cmath>
#include <cstring>
#include <climits>
#include <cstdio>
#include <utility>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <numeric>
#include <limits>
#include <iomanip>
#include <tuple>
#include <functional>
 
using namespace std;

vector<string> get_tickers() {
    vector<string> tickers;
    ifstream file("./tickers.txt");
    if(file.is_open()) {
        string line;
        while(getline(file, line)) {
            if(!line.empty())
                tickers.push_back(line);
        }
        file.close();
    }
    return tickers;
}

vector<int> get_labels(vector<double> &values) {
    vector<int> labels;
    for(auto x: values) {
        if(x < -1)
            labels.push_back(-2);
        else if(x < 0)
            labels.push_back(-1);
        else if(x < 1)
            labels.push_back(1);
        else
            labels.push_back(2);
    }
    return labels;
}

void backtest(int window_size) {
    vector<string> tickers = get_tickers();
    // vector<string> tickers;
    // tickers.push_back("HDFCBANK.NS");
    double overall = 0, invested = 0;
    for(auto ticker: tickers) {
        cout << "Ticker under consideration: " << ticker << endl;
        string filename = "./outputs/" + ticker + ".txt";
        vector<double> values;
        ifstream file_test(filename);
        if(file_test.is_open()) {
            double val;
            while(file_test >> val)
                values.push_back(val);
            file_test.close();
        }
        vector<int> labels = get_labels(values);
        map<vector<int>, vector<int> > mapping;
        vector<int> current;
        for(int index = 0; index < window_size; index++) 
            current.push_back(labels[index]);
        mapping[current].push_back(labels[window_size]);
        for(int curr_idx = window_size + 1; curr_idx < labels.size(); curr_idx++) {
            current.push_back(labels[curr_idx - 1]);
            current.erase(current.begin());
            mapping[current].push_back(labels[curr_idx]);
        }

        // Bactesting.
        int losing = 0, winning = 0;
        filename = "./backtest_files/" + ticker + ".txt";
        values.clear();
        current.clear();
        ifstream file_backtest(filename);
        if(file_backtest.is_open()) {
            double val;
            while(file_backtest >> val)
                values.push_back(val);
            file_backtest.close();
        }
        labels = get_labels(values);
        double total = 100;
        for(int index = 0; index < window_size; index++) 
            current.push_back(labels[index]);
        if(mapping.find(current) != mapping.end()) {
            int sum = 0, minimum = 3, maximum = -3;
            for(auto x: mapping[current]) {
                // cout << x << " ";
                sum += x;
                minimum = min(minimum, x);
                maximum = max(maximum, x);
            }
            // cout << endl;
            if(sum >= 0 and abs(minimum) <= abs(maximum)) {
                // cout << "MARKED: " << count_three << " " << mapping[current].size() << endl;
                total *= (1 + values[window_size] / 100.0);
                if(values[window_size] >= 0)
                    winning += 1;
                else
                    losing += 1;
            }
        }
        for(int curr_idx = window_size + 1; curr_idx < labels.size(); curr_idx++) {
            current.push_back(labels[curr_idx - 1]);
            current.erase(current.begin());
            if(mapping.find(current) != mapping.end()) {
                int sum = 0, minimum = 3, maximum = -3;
                for(auto x: mapping[current]) {
                    // cout << x << " ";
                    sum += x;
                    minimum = min(minimum, x);
                    maximum = max(maximum, x);
                }
                // cout << endl;
                if(sum >= 0 and abs(minimum) <= abs(maximum)) {
                    // cout << count_three << " " << mapping[current].size() << endl;
                    total *= (1 + values[curr_idx] / 100.0);
                    if(values[curr_idx] >= 0)
                        winning += 1;
                    else
                        losing += 1;
                }
            }
        }
        overall += total - 100;
        invested += 100;
        cout << "Overall Profit Percentage: " << total - 100.0 << endl;
        cout << "Winning Trades: " << winning << " and Losing Trades: " << losing << endl << endl;
    }
    cout << "Overall gains: " << overall << endl;
    cout << "Net Invested Amount: " << invested << endl;
    cout << "Percentage Gains: " << overall * 100.0 / invested << endl;
} 
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    // Using a window size of 10.
    backtest(5);

    return 0;
}