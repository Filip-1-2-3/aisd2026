#pragma once
#include <vector>
using std::vector;

// deklaracje z radix_sort.cpp
void radix(vector<int> &A, int ilosc_cyfr, int ile_narny);
// deklaracje z quick_sort.cpp
void quicksort(vector<int>& A, int start, int end);
void mod_quicksort(vector<int>& A, int start, int end);
// deklaracje z bucket_sort.cpp
struct Node;
struct LinkedList;
void bucketsort(vector<float>& A);
void mod_bucketsort(vector<float>& A);