#include <iostream>
#include <vector>
#include <tuple>
using std::cout;
using std::endl;
using std::vector;
using std::pair;

static void swap(vector<int>& A, int a, int b)
{
    int buffer=A[a];
    A[a]=A[b];
    A[b]=buffer;
}

static int split(vector<int>& A, int start, int end)
{
    int pivot=A[end];
    int i=start-1;
    for (int j=start; j<=end-1; j++)
    {
        if(A[j]<pivot)
        {
            i++;
            swap(A,i,j);
        }
    }
    swap(A,i+1,end);
    return i+1;
}
void quicksort(vector<int>& A, int start, int end)
{
    if (start<end)
    {
        int new_pivot=split(A, start, end);
        quicksort(A, start, new_pivot - 1);
        quicksort(A, new_pivot + 1, end);
    }
}
//------------------------

static pair<int,int> split_into_3(vector<int>& A, int start, int end)
{
    // ewentualna zamiana poczatkowa
    if(A[start]>A[end])
        swap(A,start,end);

    int l_pivot=A[start];
    int r_pivot=A[end];
    int R=end;
    int L=start;
    int k=start+1;


    while (k<R)
    {
        if (A[k]<l_pivot)
        {
            L++;
            swap(A,L,k);
            k++;
        }
        else if (A[k]>r_pivot)
        {
            R--;
            swap(A,R,k);
        }
        else k++;
    }
    swap(A,start,L);
    swap(A,end,R);
    return {L,R};
}
void mod_quicksort(vector<int>& A, int start, int end)
{
    if(start<end)
    { 
        pair<int,int> new_pivots=split_into_3(A, start, end);
        mod_quicksort(A,start,new_pivots.first-1);
        mod_quicksort(A,new_pivots.first+1, new_pivots.second-1);
        mod_quicksort(A,new_pivots.second+1,end); 
    }
}
