#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int a[100];
int N;
int p[100], q[100];
void upheap(int k);
void insert(int v);
void downheap(int a[], int N, int k );
void pqconstruct (int a[], int N);
void heapsort (int a[], int N);
void pqdownheap(int a[], int N, int k);

int main(void) {
    int arr[] = {0, 20, 15, 10, 30, 25, 5, -1000000};  
    N = 7;

    printf("Original array:\n");
    for (int i = 1; i <= N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    heapsort(arr, N);

    printf("Sorted array:\n");
    for (int i = 1; i <= N; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}


// Moves an index up the heap to maintain min-heap property
void upheap(int k) {
    int v = a[k];
    a[0] = INT_MAX; // Set a[0] to INT_MAX to simplify comparison

    // Move up while parent is less than current value
    while(a[k/2] <= v) {
        a[k] = a[k/2];
        k = k / 2;
    }

    a[k] = v; // Place the value at the correct position
}
    
// Inserts a new item and restores heap property
void insert(int v) {
    a[++N] = v; // Add element at the end
    upheap(N);   // Restore heap by moving up
}

// Moves an index down the heap to restore heap property
void downheap(int a[], int N, int k) {
    int j, v = a[k];

    // Move down the heap until heap property is restored
    while (k <= N / 2) {
        j = 2 * k; // Left child index
        if (j < N && a[j] < a[j + 1]) j++; // Select the larger child
        if (v >= a[j]) break; // If the current value is larger than both children, stop
        a[k] = a[j]; // Move child up
        k = j; // Continue with the selected child
    }

    a[k] = v; // Place the value at the correct position
}

// Removes and returns the maximum (root) value and restores heap property
int remove_max() {
    int v = a[1];
    a[1] = a[N--]; // Swap root with last element and reduce heap size
    downheap(a, N, 1); // Restore heap property starting from the root
    return v;
}

// Replaces root with a new value and restores heap property
int replace(int v) {
    a[0] = v; // Replace root with new value
    downheap(a, N, 1); // Restore heap property
    return a[0]; // Return replaced value
}

// Sorts the array using heap sort algorithm
void heapsort(int a[], int N) {
    int k, t;

    // Build the heap from non-leaf nodes down to the root
    for (k = N / 2; k >= 1; k--) downheap(a, N, k);

    // Repeatedly extract the max (root) and restore heap property
    while (N > 1) {
        t = a[1]; // Store root
        a[1] = a[N]; // Swap root with last element
        a[N] = t;
        downheap(a, --N, 1); // Restore heap property
    }
}

// Constructs a priority queue (max-heap) from an unordered array
void pqconstruct(int a[], int N) {
    int k;
    for (k = 1; k <= N; k++) {
        p[k] = k; q[k] = k; // Initialize p[] and q[] to original indices
    }

    // Build the heap using pqdownheap
    for (k = N / 2; k >= 1; k--) {
        pqdownheap(a, N, k);
    }
}

// Priority queue-specific downheap to maintain the heap with p[] and q[] arrays
void pqdownheap(int a[], int N, int k) {
    int j, v = p[k];

    // Move down in the priority queue to restore the heap property
    while (k <= N / 2) {
        j = 2 * k; // Left child index
        if (j < N && a[p[j]] < a[p[j + 1]]) j++; // Choose the larger child
        if (a[v] >= a[p[j]]) break; // Stop if the current value is greater than the child
        p[k] = p[j]; q[p[j]] = k; // Move child up
        k = j; // Continue with the selected child
    }
    p[k] = v; q[v] = k; // Place the value at the correct position
}

