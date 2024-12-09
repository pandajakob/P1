#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Job {
    int Id;
    char title[100];
    char adress[100]; 
    double salary; // kr/time
    double distanceFromAAUInKM; // km
    double workingHours;
    double travelTimeByWalkInMinutes;
    double travelTimeByBikeInMinutes;
    double travelTimeByPublicInMinutes;
    double travelTimeByCarInMinutes;
} Job;

typedef enum CommuteMode {WALK, BIKE, PUBLIC_TRANSPORT, CAR} CommuteMode;
typedef enum CommuteModeCategory {ACTIVE=1, GREEN=2, NO_PREFERENCE=3} CommuteModeCategory;


Job *readJobs(int *n);
Job *filterJobs(int *n, Job *jobsArray, int minSalary, int timeOnStudies, int maxWorkload, int *k);

void merge(Job jobsFilteredArray[], int start, int end, int mid, CommuteMode commuteMode);
void mergeSort(Job jobsArray[], int start, int end, CommuteMode commuteMode);

double getTTR(Job job, CommuteMode commuteMode);
void printJobs(Job *jobsArray, int numberOfJobs);

int main() {
    int numberOfJobs = 0;
    int numberOfJobsFiltered = 0;

    int inputMinimumSalary = 0;
    int inputTimeFromHomeToAAUInMinutes = 0;
    int inputMaximumWorkloadInHours = 0;
    CommuteModeCategory inputCommuteModeCategory = NO_PREFERENCE;
    int inputTimeOnStudiesInHours = 0;
    
    printf("Indtast minimums maanedlige loen du skal bruge for at overleve: \n");
    scanf("%d", &inputMinimumSalary);

    printf("Indtast din transporttid til AAU fra dit hjem i minutter: \n");
    scanf("%d", &inputTimeFromHomeToAAUInMinutes);

    printf("Indtast maengden af ugentlige arbejdstimer du oensker: \n");
    scanf("%d", &inputMaximumWorkloadInHours);

    printf("Indtast den gennemsnitlige antal timer du bruger paa dit studie ugentligt: \n");
    scanf("%d", &inputTimeOnStudiesInHours);

    printf("Hvilken commute mode foretraekker du? \n 1 - Aktiv \n 2 - Groen \n 3 - Ingen praeference \n");
    scanf("%d", &inputCommuteModeCategory);
    
    printf("Minimum Salary: %d\nTime to AAU: %d\nAvg. Time on studies: %d\nChoosen comuute mode: %d\n", 
            inputMinimumSalary, inputTimeFromHomeToAAUInMinutes, inputTimeOnStudiesInHours, inputCommuteModeCategory);

    Job *jobsArray = readJobs(&numberOfJobs);
    Job *jobsFilteredArray = filterJobs(&numberOfJobs, jobsArray, inputMinimumSalary, inputTimeOnStudiesInHours, 
                                        inputMaximumWorkloadInHours, &numberOfJobsFiltered);


    if (inputCommuteModeCategory == ACTIVE) {
        mergeSort(jobsFilteredArray, 0, numberOfJobsFiltered - 1, WALK);
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
    }
    else if (inputCommuteModeCategory == GREEN) {
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
    }
    else {
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
    }

    return 0;
}

void printJobs(Job *jobsArray, int numberOfJobs) {
    int i;
    int count;

    if (numberOfJobs > 10)
        count = 10;
    else 
        count = numberOfJobs;

    for (i = 0; i < count; i++) {
        printf("%-30s %-30s %-15.1lf %-15.1lf %-15.1lf %-15.1lf %-15.5lf \n",
        jobsArray[i].title, 
        jobsArray[i].adress, 
        jobsArray[i].salary, 
        jobsArray[i].distanceFromAAUInKM, 
        jobsArray[i].workingHours, 
        jobsArray[i].travelTimeByWalkInMinutes,
        jobsArray[i].travelTimeByCarInMinutes);
    }
}

Job *filterJobs(int *n, Job *jobsArray, int minimumSalary, int timeOnStudiesInHours, int maximumWorkloadInHours, int *k) {
    Job *jobsFilteredArray = malloc(*n * sizeof(Job));

    if (jobsFilteredArray == NULL) {
      printf("Memory not allocated.\n");
      exit(EXIT_FAILURE);
    }

    *k = 0; 

    for (int i = 0; i < *n; i++) {
        double tempWorkingHours = jobsArray[i].workingHours;
        double tempMonthlySalary = jobsArray[i].salary * tempWorkingHours * 4.33;
        double tempWorkload = tempWorkingHours + timeOnStudiesInHours;

        if (tempMonthlySalary >= minimumSalary && tempWorkload <= maximumWorkloadInHours) {
            jobsFilteredArray[*k] = jobsArray[i]; 
            (*k)++;
        }
    }

    return jobsFilteredArray;
}

Job *readJobs(int *n) {
    FILE *fp;
    char row[1000];
    char *token;

    int capacity = 2;
    
    Job *jobsArray = malloc(sizeof(Job)*capacity);

    if (jobsArray == NULL) {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    
    fp = fopen("./src/survey.txt", "r"); 

    if (fp == NULL) {
        printf("Error opening file\n");
        exit(EXIT_FAILURE);
    }
    
    int read = 0; // read holds the variables read in each line.

    fgets(row, sizeof(row), fp); // makes sure we skip the first header line

    do {
        //fscanf scans a row in survey.txt, into a given target. Our target is the jobs structure
        read = fscanf(fp, "%99[^,],%99[^,],%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", 
                    jobsArray[*n].title, jobsArray[*n].adress,
                    &jobsArray[*n].salary, &jobsArray[*n].distanceFromAAUInKM, 
                    &jobsArray[*n].workingHours, &jobsArray[*n].travelTimeByWalkInMinutes, 
                    &jobsArray[*n].travelTimeByBikeInMinutes, &jobsArray[*n].travelTimeByPublicInMinutes, 
                    &jobsArray[*n].travelTimeByCarInMinutes);
                    
        jobsArray[*n].Id = *n;

        /*hver linje har 9 elementer, som skal scannes. derfor når det er opnået, 
        så incrementes records, som holder styr på linjetal*/
        if (read == 9) { 
            (*n)++;
        }
        
        if (read != 9 && !feof(fp)) {
            printf("Error: file format incorrect\n");
            exit(EXIT_FAILURE);
        }

        // if we run out of capacity, it will double the current capacity -> increasing space.
        if (*n >= capacity) {
            capacity *= 2;
            jobsArray = realloc(jobsArray, sizeof(Job)*capacity);
            if (jobsArray == NULL) {
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

    return jobsArray;
}

void merge(Job jobsFilteredArray[], int start, int end, int mid, CommuteMode commuteMode){
    int i = 0, j = 0, k = 0;

    // Definerer arrays L1 og L2 med korrekt størrelse (halvering af L)
    int lengthL1 = mid - start + 1, lengthL2 = end - mid;
    Job L1[lengthL1], L2[lengthL2];

    // Fordeler første halvdel af L til L1
    for(i = 0; i < lengthL1; i++){
        L1[i] = jobsFilteredArray[start+i];
    }
    // Fordeler anden halvdel af L til L2
    for(i = 0; i < lengthL2; i++){
        L2[i] = jobsFilteredArray[mid+1+i];
    }
    // Resetter i til 0 efter brugen af 'i' i for loops
    i = 0;

    // Loop så længe at hhv index i og j ikke overskrider længden af elementer i L1 og L2
    while(i < lengthL1 && j < lengthL2){
        /* Hvis L1 har det mindste tal (eller L1 og L2 har samme tal) i sammenligningen, 
        så tilføjes L1's element til L */
        if (getTTR(L1[i],commuteMode) <= getTTR(L2[j], commuteMode)){
            jobsFilteredArray[start + i + j] = L1[i];
            i++;
        }
        // Ellers tilføjes L2's element til L
        else{
            jobsFilteredArray[start + i + j] = L2[j];
                j++;
            }
        }
        
        /* Herunder if/else statement til at tage højde for elementer "til overs" efter 
        merging. Dvs hvis enten L1 eller L2 er længere, fordi L havde et ulige antal elementer. */

        // Hvis index fra L1 er nået til enden
        if(i == mid - start + 1){
            // Så skal den tilføje resten af L2's elementer til L
            for(k = j; k <= (end - mid - 1); k++){
                jobsFilteredArray[start + i + k] = L2[k];
            }
        }
        /* Ellers tilføj resten af L1 (fordi hvis ovenstående ikke er opfyldt må L1
        alt andet lige have flere elementer tilbage efter gennemførelse af merge) */
        else {
            for(k = i; k <= (mid - start); k++){
                jobsFilteredArray[start + j + k] = L1[k];

        }
    }
}

void mergeSort(Job jobsArray[], int start, int end, CommuteMode commuteMode) {
    int mid = 0;

    if(start < end){
        mid = (end + start) / 2;

        mergeSort(jobsArray, start, mid, commuteMode);
        mergeSort(jobsArray, (mid + 1), end, commuteMode);
        merge(jobsArray, start, end, mid, commuteMode);
    }
}


double getTTR(Job job, CommuteMode commuteMode) {
    /*
        tempWorkTime = jobsFilteredArray[i].workingHours * 60;
        tempWalkTime = jobsFilteredArray[i].travelTimeByWalkInMinutes * 2;
        tempBikeTime = jobsFilteredArray[i].travelTimeByBikeInMinutes * 2;
        tempPublicTime = jobsFilteredArray[i].travelTimeByPublicInMinutes * 2;
        tempCarTime = jobsFilteredArray[i].travelTimeByCarInMinutes * 2;

        jobsFilteredArray[i].ttrWalk = tempWalkTime / (tempWalkTime + tempWorkTime);
        jobsFilteredArray[i].ttrBike = tempBikeTime / (tempBikeTime + tempWorkTime);
        jobsFilteredArray[i].ttrPublic = tempPublicTime / (tempPublicTime + tempWorkTime);
        jobsFilteredArray[i].ttrCar = tempCarTime / (tempCarTime + tempWorkTime);
    */
   return 1.01;
}


