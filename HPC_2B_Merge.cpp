#include <iostream>
#include <omp.h> // Library for OpenMP functions and timing [cite: 367]

using namespace std;

// The merge function combines two sorted halves [cite: 295, 301]
void merge(int a[], int l, int m, int r) {
    int temp[100], i = l, j = m + 1, k = 0;

    // Compare and pick the smaller element from each half [cite: 302]
    while (i <= m && j <= r) {
        if (a[i] < a[j]) {
            temp[k++] = a[i++];
        } else {
            temp[k++] = a[j++];
        }
    }

    // Copy remaining elements if any [cite: 303]
    while (i <= m) temp[k++] = a[i++];
    while (j <= r) temp[k++] = a[j++];

    // Copy the sorted temp array back into the original array [cite: 300]
    for (i = l, k = 0; i <= r; i++, k++) {
        a[i] = temp[k];
    }
}

// Sequential Merge Sort: Uses standard recursion [cite: 294, 296]
void seqMergeSort(int a[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        seqMergeSort(a, l, m);
        seqMergeSort(a, m + 1, r);
        merge(a, l, m, r);
    }
}

// Parallel Merge Sort: Uses parallel sections to sort halves simultaneously [cite: 375, 376]
void parMergeSort(int a[], int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;

        // Parallel sections allow different threads to sort the left and right halves at the same time [cite: 381, 385]
        #pragma omp parallel sections
        {
            #pragma omp section
            parMergeSort(a, l, m); // Left half sorted by one thread [cite: 382]

            #pragma omp section
            parMergeSort(a, m + 1, r); // Right half sorted by another thread [cite: 382]
        }

        merge(a, l, m, r); // Merge the results after both sections finish [cite: 383, 384]
    }
}

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    int a[100], b[100];
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        b[i] = a[i]; // Keeping data same for fair comparison [cite: 254]
    }

    // Measure Sequential Time [cite: 256, 392]
    double start = omp_get_wtime();
    seqMergeSort(a, 0, n - 1);
    double end = omp_get_wtime();
    cout << "Sequential Time: " << end - start << " seconds" << endl;

    // Measure Parallel Time [cite: 256, 393]
    start = omp_get_wtime();
    parMergeSort(b, 0, n - 1);
    end = omp_get_wtime();
    cout << "Parallel Time: " << end - start << " seconds" << endl;

    cout << "Sorted Array: ";
    for (int i = 0; i < n; i++)
        cout << b[i] << " ";
    cout << endl;

    return 0;
}

// Compile command: g++ -fopenmp program.cpp -o program.  
//Run command: ./program
