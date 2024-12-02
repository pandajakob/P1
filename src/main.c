#include <stdio.h>
#include <stdlib.h>


typedef struct Job {
    int Id;
    char title[100];
    char adress[100]; 
    double salary; // kr/time
    double distanceFromAAU; // km
    double workingHours;
    double timeWalk;
    double timeBike;
    double timePublic;
    double timeCar;
} Job;


Job *readJobs(int *n);

void printJobs(Job jobs[], int n);


int main() {
    int n = 0;
    int k = 0;
    int minSalary = 0;
    int homeToAAU = 0;
    int timeOnStudies = 0;
    int commuteMode = 0;
    int maxWorkload = 0;


    printf("Indtast minimums maanedlige loen du skal bruge for at overleve: \n");
    scanf("%d", &minSalary);

    printf("Indtast din transporttid til AAU fra dit hjem i minutter: \n");
    scanf("%d", &homeToAAU);

    printf("Indtast maengden af ugentlige arbejdstimer du oensker: \n");
    scanf("%d", &maxWorkload);

    printf("Indtast den gennemsnitlige antal timer du bruger på dit studie ugentligt: \n");
    scanf("%d", &timeOnStudies);

    printf("Hvilken commute mode foretraekker du? \n 1 - Aktiv \n 2 - Groen \n 3 - Ingen praeference \n");
    scanf("%d", &commuteMode);

    printf("%d %d %d %d", minSalary, homeToAAU, timeOnStudies, commuteMode);


    Job *jobs = readJobs(&n);
    Job *filteredJobs = filterSalary()

    for (int i = 0; i < count; i++)
    {
        /* code */
    }
    
  

    return 0;
}

Job *filterJobs(int *n, Job *jobs, int minSalary, int timeOnStudies, int maxWorkload, int *k) {
    struct Job *jobsFiltered[*n];
    

    for (int i = 0; i < *n; i++){
        double tempSalary = jobs[i].salary * jobs[i].workingHours * 4.33;
        double tempWorkload = jobs[i].workingHours + timeOnStudies;


        if (tempSalary >= minSalary && tempWorkload <= maxWorkload){

            jobsFiltered[*k] = jobs[i];
            (*k)++;          
        }
    }

    return jobsFiltered;
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
        read = fscanf(fp, "%99[^,],%99[^,],%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", 
                    jobs[records].title, jobs[records].adress,
                    &jobs[records].salary, &jobs[records].distanceFromAAU, 
                    &jobs[records].workingHours, &jobs[records].timeWalk, 
                    &jobs[records].timeBike, &jobs[records].timePublic, 
                    &jobs[records].timeCar);
                    
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

/*
void printJobs(Job jobs[],int n) {
    printf("\n records: %d", n);
    printf("\n array: %d", n);

    for (int i = 0; i < n; i++) 
        printf("\n Id: %d, %s %s Løn: %.1lf distance: %.1lf km, Gå: %.1lfmin, Cykel: %.1lfmin, tog/bus: %.1lfmin, bil: %.1lfmin \n", jobs[i].Id, jobs[i].title, jobs[i].adress, jobs[i].salary, jobs[i].distanceFromAAU, jobs[i].timeWalk, jobs[i].timeBike, jobs[i].timePublic, jobs[i].timeCar);
} */