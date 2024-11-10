#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "read_csv.h"



int read_csv(Job jobs[]) {

    FILE *fp; //*fp is file pointer;
    char row[1000];
    char *token;
    
    fp = fopen("survey.txt", "r"); 
    if (fp == NULL) {
        printf("Error opening file");
        return 1; 
    }
    
    int read = 0, records = 0;

    do {
        read = fscanf(fp, "%99[^,],%99[^,],%lf,%lf,%lf\n", 
                    jobs[records].title, jobs[records].adress,
                    &jobs[records].salary, &jobs[records].timeFromAAU, &jobs[records].distanceFromAAU);

        if (read == 5) records++;

        if (read != 5 && !feof(fp)) {
            printf("File format incorrect\n");
            return 1;
        }
    } while (!feof(fp));
    
    if (ferror(fp)) {
        printf("Error reading file \n");
        return 1;
        
    }

    fclose(fp);

    printf("\n records: %d", records);
    printf("\n array: %d", records);

    for (int i = 0; i < records; i++) 
        printf("\n%s %s Løn: %.2lf tid fra aau: %.2lf, distance: %lf", jobs[i].title, jobs[i].adress, jobs[i].salary, jobs[i].timeFromAAU, jobs[i].distanceFromAAU);

    return 0;
}