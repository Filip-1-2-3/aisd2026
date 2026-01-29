#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <chrono>
#include <random>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace std::chrono;

struct Solution {
    int max_revenue;
    vector<int> s;
};

void print_reconstructed_solution(int n, const Solution& sol) {
    cout << "Maksymalny zysk: " << sol.max_revenue << "\n";
    cout << "Skladowe kawalki (dlugosci): ";
    while (n > 0) {
        cout << sol.s[n] << " ";
        n = n - sol.s[n];
    }
    cout << "\n";
}

int cut_rod_naive(const vector<int>& p, int n) {
    if (n == 0) {
        return 0;
    }
    int q = INT_MIN;
    for (int i = 1; i <= n; ++i) {
        q = max(q, p[i - 1] + cut_rod_naive(p, n - i));
    }
    return q;
}

int memoized_cut_rod(const vector<int>& p, int n, vector<int>& revenue, vector<int>& s) {
    if (revenue[n] >= 0) {
        return revenue[n];
    }
    
    int q;
    if (n == 0) {
        q = 0;
    } else {
        q = INT_MIN;
        for (int i = 1; i <= n; ++i) {
            int current_val = p[i - 1] + memoized_cut_rod(p, n - i, revenue, s);
            if (q < current_val) {
                q = current_val;
                s[n] = i;
            }
        }
    }
    revenue[n] = q;
    return q;
}

Solution cut_rod_memoized(const vector<int>& p, int n) {
    vector<int> revenue(n + 1, INT_MIN);
    vector<int> sol(n + 1, 0);
    
    int result = memoized_cut_rod(p, n, revenue, sol);
    return {result, sol};
}

Solution cut_rod_bottom_up(const vector<int>& p, int n) {
    vector<int> revenue(n + 1);
    vector<int> sol(n + 1);
    
    revenue[0] = 0;
    
    for (int j = 1; j <= n; ++j) {
        int q = INT_MIN;
        for (int i = 1; i <= j; ++i) {
            int val = p[i - 1] + revenue[j - i];
            if (q < val) {
                q = val;
                sol[j] = i;
            }
        }
        revenue[j] = q;
    }
    
    return {revenue[n], sol};
}

vector<int> generate_prices(int n) {
    vector<int> p;
    p.reserve(n);
    mt19937 gen(1234);
    uniform_int_distribution<> dis(1, 10);

    int current_price = 0;
    for(int i = 0; i < n; ++i) {
        current_price += dis(gen);
        p.push_back(current_price);
    }
    return p;
}

void run_benchmarks() {
    cout << "\nTesty:   \n";
    cout << "Jednostka czasu: mikrosekundy (us)\n";
    
    ofstream file("rod.csv");
    file << "N,Naiwny,Memoized,Iteracyjny\n";

    vector<int> test_sizes = {10, 15, 20, 25, 100, 500, 1000, 2000, 3000, 5000};
    const int NAIVE_LIMIT = 26; 

    cout << left << setw(10) << "N (rozm.)" 
         << setw(20) << "Naiwny [us]" 
         << setw(20) << "Memoized [us]" 
         << setw(20) << "Iteracyjny [us]" << endl;
    cout << string(70, '-') << endl;

    for (int n : test_sizes) {
        vector<int> prices = generate_prices(n);
        
        long long t_naive = -1;
        long long t_memo = 0;
        long long t_iter = 0;

        cout << left << setw(10) << n;

        if (n < NAIVE_LIMIT) {
            auto start = high_resolution_clock::now();
            cut_rod_naive(prices, n);
            auto end = high_resolution_clock::now();
            t_naive = duration_cast<microseconds>(end - start).count();
            cout << setw(20) << t_naive;
        } else {
            cout << setw(20) << "--- (limit)";
        }

        {
            auto start = high_resolution_clock::now();
            cut_rod_memoized(prices, n);
            auto end = high_resolution_clock::now();
            t_memo = duration_cast<microseconds>(end - start).count();
            cout << setw(20) << t_memo;
        }

        {
            auto start = high_resolution_clock::now();
            cut_rod_bottom_up(prices, n);
            auto end = high_resolution_clock::now();
            t_iter = duration_cast<microseconds>(end - start).count();
            cout << setw(20) << t_iter;
        }
        cout << endl;

        file << n << ",";
        if (t_naive != -1) file << t_naive; 
        file << "," << t_memo << "," << t_iter << "\n";
    }

    file.close();
    cout << "\nWyniki zostaly zapisane do pliku 'rod.csv'.\n";
}

int main() {
    cout << "Test:\n";
    vector<int> example_prices = {1, 5, 8, 9, 10, 17, 17, 20, 24, 30}; 
    int example_n = 9;

    cout << "Cennik testowy: ";
    for(int p : example_prices) cout << p << " ";
    cout << "\nDlugosc preta: " << example_n << "\n\n";

    cout << "[Naiwny] Wynik: " << cut_rod_naive(example_prices, example_n) << "\n";

    cout << "\n[Memoized + Odzyskiwanie]:\n";
    Solution sol_memo = cut_rod_memoized(example_prices, example_n);
    print_reconstructed_solution(example_n, sol_memo);

    cout << "\n[Iteracyjny + Odzyskiwanie]:\n";
    Solution sol_iter = cut_rod_bottom_up(example_prices, example_n);
    print_reconstructed_solution(example_n, sol_iter);

    cout << string(50, '=') << endl;

    run_benchmarks();

    return 0;
}