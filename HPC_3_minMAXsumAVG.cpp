#include <iostream>
#include <omp.h>    // For OpenMP functions and timing [cite: 94, 168]
#include <climits>  // For INT_MAX and INT_MIN [cite: 418, 424]

using namespace std;

// Finds the minimum value in an array using parallel reduction [cite: 420]
void min_reduction(int arr[], int n) {
    int min_value = INT_MAX;

    // reduction(min: variable) creates local copies for each thread 
    #pragma omp parallel for reduction(min: min_value)
    for (int i = 0; i < n; i++) {
        if (arr[i] < min_value)
            min_value = arr[i];
    }

    cout << "Minimum value: " << min_value << endl; // [cite: 423]
}

// Finds the maximum value in an array using parallel reduction [cite: 425]
void max_reduction(int arr[], int n) {
    int max_value = INT_MIN;

    // reduction(max: variable) reduces local results to a global max [cite: 426, 427]
    #pragma omp parallel for reduction(max: max_value)
    for (int i = 0; i < n; i++) {
        if (arr[i] > max_value)
            max_value = arr[i];
    }

    cout << "Maximum value: " << max_value << endl; // [cite: 428]
}

// Calculates the sum using parallel reduction [cite: 430]
void sum_reduction(int arr[], int n) {
    int sum = 0;

    // reduction(+: variable) sums local results from all threads [cite: 431, 432]
    #pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    cout << "Sum: " << sum << endl; // [cite: 433]
}

// Calculates the average using sum reduction [cite: 437]
void average_reduction(int arr[], int n) {
    int sum = 0;

    #pragma omp parallel for reduction(+: sum)
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    // Final sum divided by size to find average [cite: 440]
    cout << "Average: " << (double)sum / n << endl; // [cite: 441]
}

int main() {
    int *arr, n;

    cout << "Enter total number of elements: ";
    cin >> n;

    arr = new int[n]; // Initializing array [cite: 443]

    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    // Call functions to find values [cite: 445]
    min_reduction(arr, n);
    max_reduction(arr, n);
    sum_reduction(arr, n);
    average_reduction(arr, n);

    return 0;
}

// Compile command: g++ -fopenmp program.cpp -o program.  
//Run command: ./program
