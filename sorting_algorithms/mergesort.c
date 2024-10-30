#include <stdio.h>
#include <stdlib.h>

void Merge(int L[], int start, int end, int mid);
void MergeSort(int L[], int start, int end);
void printList(int L[], int size);

int main(void){
    int L[] = { 5, 3, 8, 1, 6, 10, 7, 2, 4, 9 };
    int L_size = sizeof(L) / sizeof(L[0]);

    printf("Given list \n");
    printList(L, L_size);

    MergeSort(L, 0, L_size - 1);

    printf("The sorted list \n");
    printList(L, L_size);
    return 0;
}

void Merge(int L[], int start, int end, int mid){
    int i = 0, j = 0, k = 0;

    // Definerer arrays L1 og L2 med korrekt størrelse (halvering af L)
    int lengthL1 = mid - start + 1, lengthL2 = end - mid;
    int L1[lengthL1], L2[lengthL2];

    // Fordeler første halvdel af L til L1
    for(i = 0; i < lengthL1; i++){
        L1[i] = L[start+i];
    }
    // Fordeler anden halvdel af L til L2
    for(i = 0; i < lengthL2; i++){
        L2[i] = L[mid+1+i];
    }
    // Resetter i til 0 efter brugen af 'i' i for loops
    i = 0;

    // Loop så længe at hhv index i og j ikke overskrider længden af elementer i L1 og L2
    while(i < lengthL1 && j < lengthL2){
        /* Hvis L1 har det mindste tal (eller L1 og L2 har samme tal) i sammenligningen, 
        så tilføjes L1's element til L */
        if (L1[i] <= L2[j]){
            L[start + i + j] = L1[i];
            i++;
        }
        // Ellers tilføjes L2's element til L
        else{
            L[start + i + j] = L2[j];
            j++;
        }
    }
    
    /* Herunder if/else statement til at tage højde for elementer "til overs" efter 
    merging. Dvs hvis enten L1 eller L2 er længere, fordi L havde et ulige antal elementer. */

    // Hvis index fra L1 er nået til enden
    if(i == mid - start + 1){
        // Så skal den tilføje resten af L2's elementer til L
        for(k = j; k <= (end - mid - 1); k++){
            L[start + i + k] = L2[k];
        }
    }
    /* Ellers tilføj resten af L1 (fordi hvis ovenstående ikke er opfyldt må L1
    alt andet lige have flere elementer tilbage efter gennemførelse af merge) */
    else{
        for(k = i; k <= (mid - start); k++){
            L[start + j + k] = L1[k];

    }
    }
}

void MergeSort(int L[], int start, int end){
    int mid = 0;

    if(start < end){
        mid = (end + start) / 2;

        MergeSort(L, start, mid);
        MergeSort(L, (mid + 1), end);
        Merge(L, start, end, mid);
    }
}

void printList(int L[], int size){
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", L[i]);
    printf("\n");
}
