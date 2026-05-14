#include <iostream>
#include <omp.h> // Required for OpenMP API and timing functions [cite: 94, 256]

using namespace std;

// Sequential Bubble Sort: Standard implementation using nested loops [cite: 212, 254]
void sequentialBubble(int a[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

// Parallel Bubble Sort: Uses Odd-Even Transposition to allow concurrent swaps 
void parallelBubble(int a[], int n) {
    for (int i = 0; i < n; i++) {
        // Alternates between Even Phase (starts at 0) and Odd Phase (starts at 1) 
        int first = i % 2;

        // Parallelize the internal loop; shared array 'a' and 'first' index [cite: 234, 245]
        #pragma omp parallel for shared(a, first)
        for (int j = first; j < n - 1; j += 2) {
            if (a[j] > a[j + 1]) {
                int temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    // Dynamic memory allocation based on user input n [cite: 478]
    int* a = new int[n];
    int* b = new int[n];

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        b[i] = a[i]; // Copy data so both algorithms sort identical lists [cite: 254, 255]
    }

    // 1. Measure Sequential Performance [cite: 253, 392]
    double start = omp_get_wtime();
    sequentialBubble(a, n);
    double end = omp_get_wtime();
    cout << "\nSequential Time: " << (end - start) << " seconds" << endl;

    // 2. Measure Parallel Performance [cite: 253, 393]
    start = omp_get_wtime();
    parallelBubble(b, n);
    end = omp_get_wtime();
    cout << "Parallel Time: " << (end - start) << " seconds" << endl;

    cout << "\nSorted array:\n";
    for (int i = 0; i < n; i++) {
        cout << b[i] << " ";
    }
    cout << endl;


    return 0;
}

// Compile command: g++ -fopenmp program.cpp -o program.  
//Run command: ./program
