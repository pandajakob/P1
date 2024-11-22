#include <stdio.h>
#include <stdlib.h>


typedef struct Job {
    int Id;
    char title[100];
    char adress[100]; 
    double salary; // kr/time
    double distanceFromAAU; // km
    double timeWalk;
    double timeBike;
    double timePublic;
    double timeCar;
} Job;

Job *readJobs(int *n);
void printJobs(Job jobs[], int n);

int main() {
    int n = 0;

    Job *jobs = readJobs(&n);

    printJobs(jobs, n);

    return 0;
}

Job *readJobs(int *n) {
    FILE *fp; //*fp is file pointer;
    char row[1000];
    char *token;

    int capacity = 2; // initial capacity of 2 jobs.
    
    Job *jobs = malloc(sizeof(Job)*capacity);
    if (jobs == NULL) {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    fp = fopen("./src/survey.txt", "r"); 

    if (fp == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    
    int read = 0, records = 0; // read holds the variables read in each record. Records is each new line

    fgets(row, sizeof(row), fp); // makes sure we skip the first header line

    do {
        //fscanf scans a row in survey.txt, into a given target. Our target is the jobs structure
        read = fscanf(fp, "%99[^,],%99[^,],%lf,%lf,%lf,%lf,%lf,%lf\n", 
                    jobs[records].title, jobs[records].adress,
                    &jobs[records].salary, &jobs[records].distanceFromAAU,
                    &jobs[records].timeWalk, &jobs[records].timeBike, 
                    &jobs[records].timePublic, &jobs[records].timeCar);
                    
        jobs[records].Id = records;

        if (read == 8) { 
            records++; 
            (*n)++;
        }
        
        if (read != 8 && !feof(fp)) {
            printf("Error: file format incorrect\n");
            exit(EXIT_FAILURE);
        }

        // if we run out of capacity, it will give jobs 10 more space.
        if (*n >= capacity) {
            capacity *= 2;
            jobs = realloc(jobs, sizeof(Job)*capacity);
            if (jobs == NULL) {
                printf("Failed to allocate memory");
                exit(EXIT_FAILURE);
            }
        }

    } while (!feof(fp));
    
    if (ferror(fp)) {
        printf("Error reading file\n");
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    return jobs;
}

void printJobs(Job jobs[],int n) {
    printf("\n records: %d", n);
    printf("\n array: %d", n);

    for (int i = 0; i < n; i++) 
        printf("\n Id: %d, %s %s Løn: %.1lf distance: %.1lf km, Gå: %.1lfmin, Cykel: %.1lfmin, tog/bus: %.1lfmin, bil: %.1lfmin \n", jobs[i].Id, jobs[i].title, jobs[i].adress, jobs[i].salary, jobs[i].distanceFromAAU, jobs[i].timeWalk, jobs[i].timeBike, jobs[i].timePublic, jobs[i].timeCar);
}