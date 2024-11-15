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


// Moves an index up the heap to maintain max-heap property
void upheap(int k) {
    int v = a[k];    // Store the value at index `k`
    a[0] = INT_MAX; // Set a[0] to INT_MAX to simplify comparison

    // Move upwards while the parent value is smaller than the current value
     while(a[k / 2] <= v) {       // Compare current value with its parent
        a[k] = a[k / 2];          // Move the parent's value down
        k = k / 2;                // Update `k` to parent's index
    }


    a[k] = v; // Place the original value `v` in its correct position
    
// Inserts a new item into the heap and restores heap property
void insert(int v) {
    a[++N] = v;   // Add the new element at the end of the heap array
    upheap(N);    // Restore the heap property by moving the new element upwards
}


// Moves an index down the heap to restore the heap property
void downheap(int a[], int N, int k) {
    int j, v = a[k];  // Store the value at index `k` and set it as `v`

    // Move down the heap until the heap property is restored
    while (k <= N / 2) {  // Continue while there are children to compare
        j = 2 * k;         // Calculate the index of the left child
        if (j < N && a[j] < a[j + 1]) j++;  // Select the larger child
        if (v >= a[j]) break;  // If the current value is larger than both children, stop
        a[k] = a[j];          // Move the larger child up to index `k`
        k = j;                // Update `k` to continue with the larger child
    }

    a[k] = v;  // Place the original value `v` in its correct position
}


// Removes and returns the maximum (root) value and restores heap property
int remove_max() {
    int v = a[1];           // Store the current root value (maximum) to return it later
    a[1] = a[N--];          // Swap root with the last element and reduce heap size (N--)
    downheap(a, N, 1);      // Restore the heap property starting from the root
    return v;               // Return the original maximum value
}


// Replaces root with a new value and restores heap property
int replace(int v) {
    a[0] = v;               // Replace the root value with the new value `v`
    downheap(a, N, 1);       // Restore the heap property by moving the new root down the heap
    return a[0];             // Return the replaced value (new root value)
}


// Sorts the array using the heap sort algorithm
void heapsort(int a[], int N) {
    int k, t;

    // Build the heap from non-leaf nodes down to the root
    // This ensures the heap property is maintained for the entire array
    for (k = N / 2; k >= 1; k--) downheap(a, N, k);

    // Repeatedly extract the max (root) and restore heap property
    while (N > 1) {
        t = a[1];            // Store the current root value (maximum value)
        a[1] = a[N];         // Swap root with the last element
        a[N] = t;            // Place the original root value at the end
        downheap(a, --N, 1); // Restore the heap property after the swap
    }
}


// Constructs a priority queue (max-heap) from an unordered array
void pqconstruct(int a[], int N) {
    int k;

    // Initialize p[] and q[] to the original indices
    // p[] and q[] store the indices of the elements in the priority queue
    for (k = 1; k <= N; k++) {
        p[k] = k;  // p[k] stores the index of element k in the array
        q[k] = k;  // q[k] also stores the same index, for possible other uses
    }

    // Build the heap using pqdownheap
    // We start from the last non-leaf node and move up to the root
    for (k = N / 2; k >= 1; k--) {
        pqdownheap(a, N, k);  // Ensure the max-heap property by calling pqdownheap
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

