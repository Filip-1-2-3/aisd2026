#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace std::chrono;

class LCS_Solver {
    string X;
    string Y;
    int m, n;
    
    vector<vector<int>> dp_table;
    bool table_ready; 

    int lcs_recursive_aux(int i, int j) {
        if (i == 0 || j == 0) return 0;
        if (dp_table[i][j] != -1) return dp_table[i][j];

        if (X[i - 1] == Y[j - 1]) {
            dp_table[i][j] = 1 + lcs_recursive_aux(i - 1, j - 1);
        } else {
            dp_table[i][j] = max(lcs_recursive_aux(i - 1, j), 
                                 lcs_recursive_aux(i, j - 1));
        }
        return dp_table[i][j];
    }

public:
    LCS_Solver(string x_str, string y_str) : X(x_str), Y(y_str) {
        m = X.length();
        n = Y.length();
        table_ready = false;
    }

    int run_memoized() {
        dp_table.assign(m + 1, vector<int>(n + 1, -1));
        int result = lcs_recursive_aux(m, n);
        table_ready = true;
        return result;
    }

    int run_iterative() {
        dp_table.assign(m + 1, vector<int>(n + 1, 0));

        for (int i = 1; i <= m; ++i) {
            for (int j = 1; j <= n; ++j) {
                if (X[i - 1] == Y[j - 1]) {
                    dp_table[i][j] = 1 + dp_table[i - 1][j - 1];
                } else {
                    dp_table[i][j] = max(dp_table[i - 1][j], dp_table[i][j - 1]);
                }
            }
        }
        table_ready = true;
        return dp_table[m][n];
    }

    string get_lcs_reconstructed() {
        if (!table_ready) return "";

        string lcs_str = "";
        int i = m, j = n;
        
        while (i > 0 && j > 0) {
            if (X[i - 1] == Y[j - 1]) {
                lcs_str += X[i - 1];
                i--;
                j--;
            }
            else if (dp_table[i - 1][j] >= dp_table[i][j - 1]) {
                i--;
            } else {
                j--;
            }
        }
        reverse(lcs_str.begin(), lcs_str.end());
        return lcs_str;
    }
};

string generate_random_dna(int length) {
    const string bases = "ACGT";
    string dna = "";
    dna.reserve(length);
    static mt19937 gen(1234); 
    uniform_int_distribution<> dis(0, bases.size() - 1);

    for (int i = 0; i < length; ++i) {
        dna += bases[dis(gen)];
    }
    return dna;
}

void run_benchmarks() {
    cout << "\nporownanie\n";
    cout << "Jednostka czasu: milisekundy (ms)\n";

    ofstream file("lcs.csv");
    file << "N,Memoized,Iteracyjny\n";
    
    vector<int> test_sizes = {10, 100, 500, 1000, 2000, 3000}; 

    cout << left << setw(15) << "Dlugosc (N)" 
         << setw(20) << "Memoized [ms]" 
         << setw(20) << "Iteracyjny [ms]" << endl;
    cout << string(55, '-') << endl;

    for (int n : test_sizes) {
        string s1 = generate_random_dna(n);
        string s2 = generate_random_dna(n);
        
        LCS_Solver solver(s1, s2);
        
        double t_memo = 0.0;
        double t_iter = 0.0;

        cout << left << setw(15) << n;

        {
            auto start = high_resolution_clock::now();
            solver.run_memoized();
            auto end = high_resolution_clock::now();
            t_memo = std::chrono::duration<double, std::milli>(end - start).count();
            cout << setw(20) << fixed << setprecision(3) << t_memo;
        }

        {
            auto start = high_resolution_clock::now();
            solver.run_iterative();
            auto end = high_resolution_clock::now();
            t_iter = std::chrono::duration<double, std::milli>(end - start).count();
            cout << setw(20) << fixed << setprecision(3) << t_iter;
        }
        cout << endl;

        file << n << "," << t_memo << "," << t_iter << "\n";
    }
    file.close();
    cout << "\nWyniki zapisane do 'lcs.csv'\n";
}

int main() {
    cout << "przyklad: \n";
    string s1 = "ALGORYTM";
    string s2 = "ALGIERCZYK";
    
    LCS_Solver demo(s1, s2);
    
    cout << "S1: " << s1 << "\nS2: " << s2 << "\n";
    
    int len1 = demo.run_memoized();
    string str1 = demo.get_lcs_reconstructed();
    cout << "[Memoized]   Dlugosc: " << len1 << ", Napis: " << str1 << "\n";

    int len2 = demo.run_iterative();
    string str2 = demo.get_lcs_reconstructed();
    cout << "[Iteracyjny] Dlugosc: " << len2 << ", Napis: " << str2 << "\n";

    if (len1 == len2 && str1 == str2 && str1.length() == len1) {
        cout << "OK\n";
    } else {
        cout << "BLAD\n";
    }

    run_benchmarks();

    return 0;
}