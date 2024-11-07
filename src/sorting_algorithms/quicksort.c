#include <stdio.h>

void quicksort(int array[], int left, int right);

int main (void){
    int i;
    int array[] = {12,7489,4,123,45,347,44,27,98,43,65,274,934,6574};
    int arraySize = sizeof(array) / sizeof(array[0]);

    printf("\nOriginal liste: ");
    for(i = 0; i < arraySize; i++){
        printf("%d ", array[i]);
    }

    quicksort(array, 0, arraySize - 1);

    printf("\nSorteret liste: ");
    for(i = 0; i < arraySize; i++){
        printf("%d ", array[i]);
    }
}

void quicksort(int array[], int left, int right){
    int pivot, i, j, temp;
    if (right > left){
        pivot = array[right]; // Pivot sættes til sidste element i array
        i = left - 1; // Index i sættes lige før første element
        j = right; // Index j sættes lige efter første element (da right er pivot, så den er ikke med)


        for ( ; ; ){
            while (array[++i] < pivot);
            while (array[--j] > pivot);
            
            /*Hvis element ved i skulle vise sig at være større 
            end j breakes der, og ingen elementer swappes*/
            if (i >= j) break;

            // Swapper element ved i og ved j
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
        temp = array[i];
        array[i] = array[right];
        array[right] = temp;

        quicksort (array, left, i-1);
        quicksort (array, i+1, right);
    }
}