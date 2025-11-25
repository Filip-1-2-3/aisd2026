#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm> // do znalezienia zakresu w modyfikacji

using std::cout;
using std::endl;
using std::vector;

struct Node {
    float val;
    Node* next;
    
    Node(float v) : val(v), next(nullptr) {}
};

struct LinkedList {
    Node* head;

    LinkedList() : head(nullptr) {}

    void sortedInsert(float newVal) 
    {
        Node* newNode = new Node(newVal);

        if (head == nullptr || head->val >= newVal) {
            newNode->next = head;
            head = newNode;
        }
        else {
            Node* current = head;
            while (current->next != nullptr && current->next->val < newVal) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }//czyszczenie pamieci
    }
};



void bucketsort(vector<float>& A) {
    if (A.empty()) return;
    int n = A.size();

    vector<LinkedList> buckets(n); 

    for (float val : A) {
        int bucketIndex = static_cast<int>(std::ceil(static_cast<float>(n) * val)) - 1;
        
        if (bucketIndex < 0) bucketIndex = 0;
        if (bucketIndex >= n) bucketIndex = n - 1; 

        buckets[bucketIndex].sortedInsert(val);
    }

    int index = 0;
    for (int i = 0; i < n; i++) {
        Node* current = buckets[i].head; 
        
        while (current != nullptr) {
            A[index++] = current->val;
            
            current = current->next;
        }
    }
}

void mod_bucketsort(std::vector<float>& A) {
    if (A.empty()) return;
    int n = A.size();

    // zakres z uzyciem biblioteki algorithm
    auto minMax = std::minmax_element(A.begin(), A.end());
    float minVal = *minMax.first;
    float maxVal = *minMax.second;

    if (minVal == maxVal) return; 

    double range = maxVal - minVal;
    std::vector<LinkedList> buckets(n); 

    for (float val : A) {

        double normalized_val = (val - minVal) / range;
    
        int bucketIndex = static_cast<int>(std::floor(static_cast<double>(n) * normalized_val));
        
        if (bucketIndex >= n) 
            bucketIndex = n - 1; 
        
        buckets[bucketIndex].sortedInsert(val);
    }
    int index = 0;
    for (int i = 0; i < n; i++) {
        Node* current = buckets[i].head;
        while (current != nullptr) {
            A[index++] = current->val;
            current = current->next;
        }
    }
}
