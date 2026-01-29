#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace std::chrono;

struct Activity {
    int id;
    int start;
    int finish;
};

void recursive_activity_selector_aux(const vector<Activity>& A, int k, int n, vector<Activity>& result) {
    int m = k + 1;
    int k_finish = (k == -1) ? 0 : A[k].finish;

    while (m < n && A[m].start < k_finish) {
        m++;
    }

    if (m < n) {
        result.push_back(A[m]);
        recursive_activity_selector_aux(A, m, n, result);
    }
}

void solve_greedy_recursive(vector<Activity> acts) {
    sort(acts.begin(), acts.end(), [](const Activity& a, const Activity& b) {
        return a.finish < b.finish;
    });

    vector<Activity> result;
    recursive_activity_selector_aux(acts, -1, acts.size(), result);
}

void solve_greedy_iterative(vector<Activity> acts) {
    sort(acts.begin(), acts.end(), [](const Activity& a, const Activity& b) {
        return a.finish < b.finish;
    });

    vector<Activity> result;
    if (acts.empty()) return;

    result.push_back(acts[0]); 
    int last_finish_index = 0;

    for (int i = 1; i < acts.size(); ++i) {
        if (acts[i].start >= acts[last_finish_index].finish) {
            result.push_back(acts[i]);
            last_finish_index = i;
        }
    }
}

void solve_greedy_start_sorted(vector<Activity> acts) {
    sort(acts.begin(), acts.end(), [](const Activity& a, const Activity& b) {
        return a.start < b.start;
    });

    vector<Activity> result;
    if (acts.empty()) return;

    int n = acts.size();
    result.push_back(acts[n - 1]);
    int last_added_index = n - 1;

    for (int i = n - 2; i >= 0; --i) {
        if (acts[i].finish <= acts[last_added_index].start) {
            result.push_back(acts[i]);
            last_added_index = i;
        }
    }
}

void solve_dp(vector<Activity> acts) {
    sort(acts.begin(), acts.end(), [](const Activity& a, const Activity& b) {
        return a.start < b.start;
    });

    int n = acts.size();
    if (n == 0) return;

    vector<int> dp(n, 1);

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            if (acts[j].finish <= acts[i].start) {
                if (dp[j] + 1 > dp[i]) {
                    dp[i] = dp[j] + 1;
                }
            }
        }
    }
}

vector<Activity> generate_activities(int n) {
    vector<Activity> acts;
    acts.reserve(n);
    static mt19937 gen(1234); 
    uniform_int_distribution<> start_dist(0, n * 2); 
    uniform_int_distribution<> duration_dist(1, 20); 

    for (int i = 0; i < n; ++i) {
        int s = start_dist(gen);
        int f = s + duration_dist(gen);
        acts.push_back({i, s, f});
    }
    return acts;
}

void run_benchmarks() {
    vector<int> test_sizes = {100, 1000, 2000, 5000, 10000}; 
    
    ofstream file("activity.csv");
    file << "N,Recursive,Iterative,Start_Sorted,DP\n";

    cout << "\n=== TESTY WYDAJNOSCI ACTIVITY SELECTOR ===\n";
    cout << "Jednostka czasu: milisekundy (ms)\n\n";

    cout << left << setw(10) << "N" 
         << setw(15) << "Rec [ms]" 
         << setw(15) << "Iter [ms]" 
         << setw(15) << "Start [ms]" 
         << setw(15) << "DP [ms]" << endl;
    cout << string(70, '-') << endl;

    for (int n : test_sizes) {
        vector<Activity> data = generate_activities(n);
        
        double t_rec = 0, t_iter = 0, t_start = 0, t_dp = 0;
        bool dp_run = false;

        cout << left << setw(10) << n;

        {
            auto start = high_resolution_clock::now();
            solve_greedy_recursive(data);
            auto end = high_resolution_clock::now();
            t_rec = duration<double, milli>(end - start).count();
            cout << setw(15) << fixed << setprecision(3) << t_rec;
        }

        {
            auto start = high_resolution_clock::now();
            solve_greedy_iterative(data);
            auto end = high_resolution_clock::now();
            t_iter = duration<double, milli>(end - start).count();
            cout << setw(15) << fixed << setprecision(3) << t_iter;
        }

        {
            auto start = high_resolution_clock::now();
            solve_greedy_start_sorted(data);
            auto end = high_resolution_clock::now();
            t_start = duration<double, milli>(end - start).count();
            cout << setw(15) << fixed << setprecision(3) << t_start;
        }

        if (n <= 15000) { 
            auto start = high_resolution_clock::now();
            solve_dp(data);
            auto end = high_resolution_clock::now();
            t_dp = duration<double, milli>(end - start).count();
            dp_run = true;
            cout << setw(15) << fixed << setprecision(3) << t_dp;
        } else {
            cout << setw(15) << "---";
        }

        cout << endl;

        file << n << "," << t_rec << "," << t_iter << "," << t_start << ",";
        if (dp_run) file << t_dp;
        file << "\n";
    }

    file.close();
    cout << "\nWyniki zapisane do 'activity.csv'.\n";
}

int main() {
    run_benchmarks();
    return 0;
}