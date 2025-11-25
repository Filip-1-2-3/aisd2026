#include "testy.h"
#include "algo.h"

#include <iostream>
#include <random>//modern standard
//#include <cstdlib> old C-style randomness
//#include <ctime>  
#include <chrono> 
#include <vector>
#include <algorithm> //sprawdzenie poprawnosci
#include <fstream> //csv output


using std::cout;
using std::endl;
using std::vector;
using std::ofstream;

void random_array_sizeN(vector<int> &tablica) 
{
    // inny losowy seed dla kazdej kompilacji
    static std::random_device rd;
    static std::mt19937 gen(rd());
    //zakres -10^6;10^6
    static std::uniform_int_distribution<> distrib(-1000000, 1000000);

    for (int &elem : tablica)
        elem = distrib(gen); 
}

void run_tests()
{
    // DANE 
    const vector<int> Ns = {100, 200, 500, 1000, 2000, 5000, 10000, 200000,500000,100000}; 

    ofstream out("wyniki1.csv", std::ios::trunc);

    out << "N,Radix,Radix1024,Quick,QuickMod,BucketMod" << endl;

    cout << "--- START TESTOW (Srednia z 10 roznych prob losowych) ---\n";

    for (int N : Ns) 
    { 
        cout << "Testowanie N = " << N << " ... ";
    
        auto suma_radix = std::chrono::microseconds::zero();
        auto suma_radix_1024 = std::chrono::microseconds::zero();
        auto suma_quick = std::chrono::microseconds::zero();
        auto suma_quick_mod = std::chrono::microseconds::zero();
        auto suma_bucket = std::chrono::microseconds::zero();

        for (int k = 0; k < 10; ++k)
        {
            vector<int> dane_bazowe(N);
            random_array_sizeN(dane_bazowe);
            vector<int> kopia;

            // RADIX
            kopia = dane_bazowe;
            auto start = std::chrono::high_resolution_clock::now();
            radix(kopia, 7, 10);
            auto stop = std::chrono::high_resolution_clock::now();
            suma_radix += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            if (!std::is_sorted(kopia.begin(), kopia.end())) cout << " [BLAD!]";
            cout << endl;

            // RADIX_1024
            const int max_val = 1000000;
            const int podstawa =1024;
            kopia = dane_bazowe;
            start = std::chrono::high_resolution_clock::now();
            radix(kopia, std::ceil(std::log(max_val) / std::log(podstawa)) + 1, 1024);
            stop = std::chrono::high_resolution_clock::now();
            suma_radix_1024 += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            if (!std::is_sorted(kopia.begin(), kopia.end())) cout << " [BLAD!]";
            cout << endl;
            
            // QUICK 
            kopia = dane_bazowe;
            start = std::chrono::high_resolution_clock::now();
            quicksort(kopia, 0, N-1);
            stop = std::chrono::high_resolution_clock::now();
            suma_quick += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            if (!std::is_sorted(kopia.begin(), kopia.end())) cout << " [BLAD!]";
            cout << endl;

            // QUICK MOD 
            kopia = dane_bazowe;
            start = std::chrono::high_resolution_clock::now();
            mod_quicksort(kopia, 0, N-1);
            stop = std::chrono::high_resolution_clock::now();
            suma_quick_mod += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            if (!std::is_sorted(kopia.begin(), kopia.end())) cout << " [BLAD!]";
            cout << endl;

            // BUCKET MOD 
            vector<float> kopia_float(dane_bazowe.begin(), dane_bazowe.end());
            start = std::chrono::high_resolution_clock::now();
            mod_bucketsort(kopia_float);
            stop = std::chrono::high_resolution_clock::now();
            suma_bucket += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            if (!std::is_sorted(kopia.begin(), kopia.end())) cout << " [BLAD!]";
            cout << endl;
        }

        cout << "Zakonczono." << endl;

        out << N << "," 
            << (suma_radix.count() / 10) << ","
            << (suma_radix_1024.count() / 10) << ","
            << (suma_quick.count() / 10) << ","
            << (suma_quick_mod.count() / 10) << ","
            << (suma_bucket.count() / 10) 
            << endl;    
    }
    out.close();

    cout << "\n--- START TESTOW PODSTAW RADIX ---\n";
    
    const int N_radix = 100000; 
    cout << "Testowanie dla stalego N = " << N_radix << endl;

    ofstream out_radix("wyniki2.csv", std::ios::trunc);
    out_radix << "Podstawa,Czas" << endl;

    const vector<int> podstawy = {2, 4, 8, 10, 16, 32, 64, 128, 256, 512, 1024,2048,4096,8192};

    for (int podstawa : podstawy)
    {
        const int max_val = 1000000; 
        int ilosc_cyfr = std::ceil(std::log(max_val) / std::log(podstawa)) + 1;

        auto suma_czasow = std::chrono::microseconds::zero();

        for(int k=0; k<10; ++k)
        {
            vector<int> dane(N_radix);
            random_array_sizeN(dane);
            
            auto start = std::chrono::high_resolution_clock::now();
            radix(dane, ilosc_cyfr, podstawa);
            auto stop = std::chrono::high_resolution_clock::now();
            
            suma_czasow += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
            
            if (!std::is_sorted(dane.begin(), dane.end())) cout << " [BLAD!]";
        }

        long sredni_czas = suma_czasow.count() / 10;
        
        cout << "Podstawa: " << podstawa << " | Sredni czas: " << sredni_czas << " us" << endl;
        out_radix << podstawa << "," << sredni_czas << endl;
    }

    out_radix.close();
}
