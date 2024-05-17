/*
clang++ -std=c++11 get_quote.cpp -o a.out
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

vector<vector<string> > backtest(int window_size) {
    vector<string> tickers = get_tickers();
    vector<vector<string> > final_list(3 * window_size + 1);
    for(auto ticker: tickers) {
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

        // Get the final quote on which stokcs to invest in at the end of the day.
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
        
        // Checking for values' correctness.
        // cout << "For ticker " << ticker << " the values are:" << endl;
        // for(auto x: values)
        //     cout << x << " ";
        // cout << endl;

        labels = get_labels(values);
        for(int index = labels.size() - window_size; index < labels.size(); index++)
            current.push_back(labels[index]);

        // Checking for labels' correctness.
        // cout << "For ticker " << ticker << " the labels are:" << endl;
        // for(auto x: current)
        //     cout << x << " ";
        // cout << endl;

        if(mapping.find(current) != mapping.end()) {
            int sum = 0, minimum = 3, maximum = -3;
            for(auto x: mapping[current]) {
                sum += x;
                minimum = min(minimum, x);
                maximum = max(maximum, x);
            }
            // cout << "Sum for ticker " << ticker << " is: " << sum << endl;
            if(sum >= 0 and abs(minimum) <= abs(maximum)) {
                final_list[sum].push_back(ticker);
            }
        }
    }
    return final_list;
} 
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    int window_size = 5;
    vector<vector<string> > final_list= backtest(window_size);
    for(int index = 0; index < 3 * window_size; index++) {
        if(final_list[index].size() == 0)
            continue;
        cout << "The list of tickers having weight = " << index << " are:" << endl;
        for(auto ticker: final_list[index])
            cout << ticker << endl;
        cout << endl;
    }

    return 0;
}