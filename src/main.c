#include <stdio.h>


typedef struct {
    int Id;
    char title[100];
    double salary; //
    char adress[100]; // navn på adresse
    double timeFromAAU; // estimeret tid givet af de studerende
    double distanceFromAAU; // km
} Job;

int read_csv(Job jobs[]);
void printJobs(Job jobs[], int n);

int main() {

    Job jobs[13];

    read_csv(jobs);
    printJobs(jobs, 13);

    return 0;
}

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

    return 0;
}

void printJobs(Job jobs[],int n) {
    printf("\n records: %d", n);
    printf("\n array: %d", n);

    for (int i = 0; i < n; i++) 
        printf("\n%s %s Løn: %.2lf tid fra aau: %.2lf, distance: %lf", jobs[i].title, jobs[i].adress, jobs[i].salary, jobs[i].timeFromAAU, jobs[i].distanceFromAAU);
}