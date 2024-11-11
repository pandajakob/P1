#include <stdio.h>
#include <stdlib.h>

void insertion(int Array[], int Array_Size);
void print_list(int Array[], int Array_size);

int main(void) {
    int Array[] = {5, 8, 1, 9, 2, 3, 7, 4, 6, 10};
    int Array_Size = sizeof(Array) / sizeof(Array[0]);

    insertion(Array, Array_Size);

    printf("The sorted list \n");
    print_list(Array, Array_Size);

    return 0;
}

void insertion(int Array[], int Array_Size) {
    int i, j, Value;

    for (i = 1; i < Array_Size; i++) {
        Value = Array[i];
        j = i;

        while (j > 0 && Array[j - 1] > Value) {
            Array[j] = Array[j - 1];
            j--;
        }

        Array[j] = Value;
    }
}

void print_list(int Array[], int Array_size) {// Tak malthe med k
    int i;
    
    for (i = 0; i < Array_size; i++){
        printf("%d ", Array[i]);
    }
}