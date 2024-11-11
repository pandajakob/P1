#include <stdio.h>

insertion(int Array[], int Array_Size);

int main(void){

    int Array[] = {5, 8, 1, 9, 2, 3, 7, 4, 6, 10};
    int Array_Size = sizeof(Array);

    insertion(Array, Array_Size);
    
    return 0;
}



insertion(int Array[], int Array_Size){
    int i, j, Value; //Current index, index, Value 

    for (i = 2; i <= Array_Size; i++){
        Value = Array[i];
        j = i;

        while (Array[j-1] > Value){
            Array[j] = Array[j-1]; j--;
        }

    Array[j] = Value; 
    }
    return Array;
}