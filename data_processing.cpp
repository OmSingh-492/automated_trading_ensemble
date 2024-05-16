/*
clang++ -std=c++11 data_processing.cpp -o a.out
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

vector<int> get_labels() {
    vector<string> tickers = get_tickers();
    vector<double> values;
    for(auto ticker: tickers) {
        string filename = "./outputs/" + ticker + ".txt";
        ifstream file(filename);
        if(file.is_open()) {
            double val;
            while(file >> val)
                values.push_back(val);
            file.close();
        }
    }
    
    /* 
    Labels: (inclusive-exclusive)
    < -1 -> -4
    -1 - -0.5 -> -3
    -0.5 - -0.1 -> -2
    -0.1 - 0 -> -1
    0 - 0.1 -> 0
    0.1 - 0.5 -> 1
    0.5 - 1 -> 2
    >= 1 -> 3
    */

    vector<int> labels;
    for(auto x: values) {
        if(x < -4)
            labels.push_back(-6);
        else if(x < -2.5)
            labels.push_back(-5);
        else if(x < -1.0)
            labels.push_back(-4);
        else if(x < -0.5)
            labels.push_back(-3);
        else if(x < -0.3)
            labels.push_back(-2);
        else if(x < -0.1)
            labels.push_back(-1);
        else if(x < 0.1)
            labels.push_back(0);
        else if(x < 0.3)
            labels.push_back(1);
        else if(x < 0.5)
            labels.push_back(2);
        else if(x < 1.0)
            labels.push_back(3);
        else if(x < 2.5)
            labels.push_back(4);
        else if(x < 4.0)
            labels.push_back(5);
        else
            labels.push_back(6);
    } 
    return labels;
}    

map<vector<int>, vector<int> > get_map(int window_size) {
    vector<int> labels = get_labels();
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
    return mapping;
}
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    
    map<vector<int>, vector<int> > mapping = get_map(3);
    // Checking for the level of homogenity in the entries.
    for(auto entry: mapping) {
        for(auto x: entry.second)
            cout << x << " ";
        cout << endl;
    }

    return 0;
}