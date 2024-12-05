#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    double ttrWalk;
    double ttrBike;
    double ttrPublic;
    double ttrCar;
} Job;


Job *readJobs(int *n);
void printJobs(Job jobs[], int n);
Job *filterJobs(int *n, Job *jobs, int minSalary, int timeOnStudies, int maxWorkload, int *k);
void Merge(Job L[], int start, int end, int mid, int (*compare)(const Job*, const Job*));
void MergeSort(Job L[], int start, int end, int (*compare)(const Job*, const Job*));
int compareByTTRBike(const Job* a, const Job* b);
int compareByTTRWalk(const Job* a, const Job* b);
int compareByTTRPublic(const Job* a, const Job* b);
int compareByTTRCar(const Job* a, const Job* b);
void category(int commuteMode, Job *jobsFiltered, int *k);
void calcTTR(Job *jobsFiltered, int *k);
void printWalk(Job *jobsFiltered, int *k);
void printBike(Job *jobsFiltered, int *k);
void printPublic(Job *jobsFiltered, int *k);
void printCar(Job *jobsFiltered, int *k);

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

    printf("Indtast den gennemsnitlige antal timer du bruger paa dit studie ugentligt: \n");
    scanf("%d", &timeOnStudies);

    printf("Hvilken commute mode foretraekker du? \n 1 - Aktiv \n 2 - Groen \n 3 - Ingen praeference \n");
    scanf("%d", &commuteMode);
    

    printf("Minimum Salary: %d\nTime to AAU: %d\nAvg. Time on studies: %d\nChoosen comuute mode: %d\n", minSalary, homeToAAU, timeOnStudies, commuteMode);


    Job *jobs = readJobs(&n);
    Job *jobsFiltered = filterJobs(&n, jobs, minSalary, timeOnStudies, maxWorkload, &k);
    calcTTR(jobsFiltered, &k);
    category(commuteMode, jobsFiltered, &k);
    return 0;
}


void printWalk(Job *jobsFiltered, int *k){
    printf("By Walk:\n");
    printf("%-30s %-30s %-15s %-15s %-15s %-15s %-15s \n", "Title", "Adress", "Salary", "Distance", "Hours", "Walk-time", "TTR");
    MergeSort(jobsFiltered, 0, *k - 1, compareByTTRWalk);
    for (int i = 0; i < *k; i++){
        printf("%-30s %-30s %-15.1lf %-15.1lf %-15.1lf %-15.1lf %-15.5lf \n",
        jobsFiltered[i].title, 
        jobsFiltered[i].adress, 
        jobsFiltered[i].salary, 
        jobsFiltered[i].distanceFromAAU, 
        jobsFiltered[i].workingHours, 
        jobsFiltered[i].timeWalk,
        jobsFiltered[i].ttrWalk);
    }
}
void printBike(Job *jobsFiltered, int *k){
    printf("\nBy Bike:\n");
    printf("%-30s %-30s %-15s %-15s %-15s %-15s %-15s \n", "Title", "Adress", "Salary", "Distance", "Hours", "Bike-time", "TTR");
    MergeSort(jobsFiltered, 0, *k - 1, compareByTTRBike);
    for (int i = 0; i < *k; i++){
        printf("%-30s %-30s %-15.1lf %-15.1lf %-15.1lf %-15.1lf %-15.5lf\n",
        jobsFiltered[i].title, 
        jobsFiltered[i].adress, 
        jobsFiltered[i].salary, 
        jobsFiltered[i].distanceFromAAU, 
        jobsFiltered[i].workingHours,  
        jobsFiltered[i].timeBike,
        jobsFiltered[i].ttrBike);
    }
}

void printPublic(Job *jobsFiltered, int *k){
    printf("\nBy Public:\n");
    printf("%-30s %-30s %-15s %-15s %-15s %-15s %-15s \n", "Title", "Adress", "Salary", "Distance", "Hours", "Public-time", "TTR");
    MergeSort(jobsFiltered, 0, *k - 1, compareByTTRPublic);
    for (int i = 0; i < *k; i++){
        printf("%-30s %-30s %-15.1lf %-15.1lf %-15.1lf %-15.1lf %-15.5lf \n",
        jobsFiltered[i].title, 
        jobsFiltered[i].adress, 
        jobsFiltered[i].salary, 
        jobsFiltered[i].distanceFromAAU, 
        jobsFiltered[i].workingHours,  
        jobsFiltered[i].timePublic,
        jobsFiltered[i].ttrPublic);
    }
}
void printCar(Job *jobsFiltered, int *k){
    printf("\nBy Car:\n");
    printf("%-30s %-30s %-15s %-15s %-15s %-15s %-15s \n", "Title", "Adress", "Salary", "Distance", "Hours", "Car-time", "TTR");
    MergeSort(jobsFiltered, 0, *k - 1, compareByTTRCar);
    for (int i = 0; i < *k; i++){
        printf("%-30s %-30s %-15.1lf %-15.1lf %-15.1lf %-15.1lf %-15.5lf \n",
        jobsFiltered[i].title, 
        jobsFiltered[i].adress, 
        jobsFiltered[i].salary, 
        jobsFiltered[i].distanceFromAAU, 
        jobsFiltered[i].workingHours,  
        jobsFiltered[i].timeCar,
        jobsFiltered[i].ttrCar);
    }
}

void category (int commuteMode, Job *jobsFiltered, int *k) {
    if (*k > 10){
        *k = 10;
    }

    // Active mode
    if (commuteMode == 1) {
        printf("TOP 10 Active modes \n");
        printWalk(jobsFiltered, k);
        printBike(jobsFiltered, k);
    }

    // Green mode
    else if (commuteMode == 2) {
        printf("TOP 10 Green modes \n");
        printWalk(jobsFiltered, k);
        printBike(jobsFiltered, k);
        printPublic(jobsFiltered, k);
    }

    // No pref
    else if (commuteMode == 3) {
        printf("TOP 10 of all modes \n");
        printWalk(jobsFiltered, k);
        printBike(jobsFiltered, k);
        printPublic(jobsFiltered, k);
        printCar(jobsFiltered, k);
    }
}

Job *filterJobs(int *n, Job *jobs, int minSalary, int timeOnStudies, int maxWorkload, int *k) {
    Job *jobsFiltered = malloc(*n * sizeof(Job));

    *k = 0; 

    for (int i = 0; i < *n; i++) {
        double tempWorkingHours = jobs[i].workingHours;
        double tempSalary = jobs[i].salary * tempWorkingHours * 4.33;
        double tempWorkload = tempWorkingHours + timeOnStudies;

        if (tempSalary >= minSalary && tempWorkload <= maxWorkload) {
            jobsFiltered[*k] = jobs[i]; 
            (*k)++;
        }
    }
    //printf("Filtered jobs (%d):\n", *k);
    /*
    for (int i = 0; i < *k; i++) {
        printf("Job %s: Salary = %.0lf Hours = %.0lf\n", jobsFiltered[i].title, jobsFiltered[i].salary, jobsFiltered[i].workingHours);
    }
    */
    return jobsFiltered; // Return the filtered jobs
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

        if (read == 9) { 
            records++; 
            (*n)++;
        }
        
        if (read != 9 && !feof(fp)) {
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

void Merge(Job jobsFiltered[], int start, int end, int mid, int (*compare)(const Job*, const Job*)){
    int i = 0, j = 0, k = 0;

    // Definerer arrays L1 og L2 med korrekt størrelse (halvering af L)
    int lengthL1 = mid - start + 1, lengthL2 = end - mid;
    Job L1[lengthL1], L2[lengthL2];

    // Fordeler første halvdel af L til L1
    for(i = 0; i < lengthL1; i++){
        L1[i] = jobsFiltered[start+i];
    }
    // Fordeler anden halvdel af L til L2
    for(i = 0; i < lengthL2; i++){
        L2[i] = jobsFiltered[mid+1+i];
    }
    // Resetter i til 0 efter brugen af 'i' i for loops
    i = 0;

    // Loop så længe at hhv index i og j ikke overskrider længden af elementer i L1 og L2
    while(i < lengthL1 && j < lengthL2){
        /* Hvis L1 har det mindste tal (eller L1 og L2 har samme tal) i sammenligningen, 
        så tilføjes L1's element til L */
        if (compare(&L1[i], &L2[j]) <= 0){
            jobsFiltered[start + i + j] = L1[i];
            i++;
        }
        // Ellers tilføjes L2's element til L
        else{
            jobsFiltered[start + i + j] = L2[j];
            j++;
        }
    }
    
    /* Herunder if/else statement til at tage højde for elementer "til overs" efter 
    merging. Dvs hvis enten L1 eller L2 er længere, fordi L havde et ulige antal elementer. */

    // Hvis index fra L1 er nået til enden
    if(i == mid - start + 1){
        // Så skal den tilføje resten af L2's elementer til L
        for(k = j; k <= (end - mid - 1); k++){
            jobsFiltered[start + i + k] = L2[k];
        }
    }
    /* Ellers tilføj resten af L1 (fordi hvis ovenstående ikke er opfyldt må L1
    alt andet lige have flere elementer tilbage efter gennemførelse af merge) */
    else{
        for(k = i; k <= (mid - start); k++){
            jobsFiltered[start + j + k] = L1[k];

    }
    }
}

void MergeSort(Job jobsFiltered[], int start, int end, int (*compare)(const Job*, const Job*)){
    int mid = 0;

    if(start < end){
        mid = (end + start) / 2;

        MergeSort(jobsFiltered, start, mid, compare);
        MergeSort(jobsFiltered, (mid + 1), end, compare);
        Merge(jobsFiltered, start, end, mid, compare);
    }
}

int compareByTTRBike(const Job* a, const Job* b) {
    return (a->ttrBike > b->ttrBike) - (a->ttrBike < b->ttrBike);
}

int compareByTTRWalk(const Job* a, const Job* b) {
    return (a->ttrWalk > b->ttrWalk) - (a->ttrWalk < b->ttrWalk);
}

int compareByTTRPublic(const Job* a, const Job* b) {
    return (a->ttrPublic > b->ttrPublic) - (a->ttrPublic < b->ttrPublic);
}

int compareByTTRCar(const Job* a, const Job* b) {
    return (a->ttrCar > b->ttrCar) - (a->ttrCar < b->ttrCar);
}

void calcTTR(Job *jobsFiltered, int *k) {
    for (int i = 0; i < *k; i++){
        double tempWorkTime = jobsFiltered[i].workingHours * 60;
        double tempWalkTime = jobsFiltered[i].timeWalk * 2;
        double tempBikeTime = jobsFiltered[i].timeBike * 2;
        double tempPublicTime = jobsFiltered[i].timePublic * 2;
        double tempCarTime = jobsFiltered[i].timeCar * 2;

        jobsFiltered[i].ttrWalk = tempWalkTime / (tempWalkTime + tempWorkTime);
        jobsFiltered[i].ttrBike = tempBikeTime / (tempBikeTime + tempWorkTime);
        jobsFiltered[i].ttrPublic = tempPublicTime / (tempPublicTime + tempWorkTime);
        jobsFiltered[i].ttrCar = tempCarTime / (tempCarTime + tempWorkTime);
    }
} 
