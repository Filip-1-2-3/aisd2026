#include <iostream>
#include <vector>
#include <cmath> 
#include "testy.h"

using std::cout;
using std::endl;
using std::vector;

static void counting_sort(vector<int>&A, int ile_narny, int pozycja,bool znak)
{
    int n = A.size();
    vector<int> C (ile_narny,0);
    vector<int> B (n);

    //zliczanie wystapien
    for (int x:A)
        C[(abs(x)/(int)pow(ile_narny,pozycja))%ile_narny]++;

    for (int j = 1; j <= ile_narny-1; ++j) 
        C[j] += C[j - 1]; //tablica ostatnich pojawien

    
    for (int i = n - 1; i >= 0; --i) {
        int wartosc_elementu = A[i];
        int cyfra=(abs(wartosc_elementu)/(int)pow(ile_narny,pozycja))%ile_narny;
        // C[wartosc_elementu] zawiera pozycję ostatniego wystąpienia tej wartości
        int ostateczny_indeks = C[cyfra] - 1; 

        B[ostateczny_indeks] = wartosc_elementu;

        C[cyfra]--;
    }
    A=B;
}

void radix(vector<int> &A, int ilosc_cyfr, int ile_narny) //w finalnej wersji tylko modyfikacja do testow
{
    int n=A.size();
    vector<int> A_positive;
    vector<int> A_negative;

    for (int elem:A)
    {
        if (elem>=0) A_positive.push_back(elem);
        else A_negative.push_back(elem);
    }

    
    for (int i=0; i<ilosc_cyfr;i++)
    {
        if (!A_negative.empty()) counting_sort(A_negative, ile_narny, i, 0);
        if (!A_positive.empty()) counting_sort(A_positive, ile_narny, i, 1);
    }


    int neg_size=A_negative.size(); 
    int pos_size = A_positive.size();   
    for (int i=0; i<neg_size/2; ++i)
    {
        int buffer=A_negative[i];
        A_negative[i]=A_negative[neg_size-i-1];
        A_negative[neg_size-i-1]=buffer;
    }

    //scalenie dodatnich i ujemnych do A
    for(int i = 0; i < neg_size; i++) A[i] = A_negative[i];
    for(int i = 0; i < pos_size; i++) A[neg_size + i] = A_positive[i];    
}



