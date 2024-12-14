#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

typedef struct Job {
    int Id;
    char title[100];
    char adress[100]; 
    double salary; // kr/time
    double distanceFromAAUInKM; // km

    double workingHoursPerWeek;

    double travelTimeByWalkInMinutes;
    double travelTimeByBikeInMinutes;
    double travelTimeByPublicInMinutes;
    double travelTimeByCarInMinutes;
} Job; 

typedef enum CommuteMode {WALK, BIKE, PUBLIC_TRANSPORT, CAR} CommuteMode;
typedef enum CommuteModeCategory {ACTIVE=1, GREEN=2, NO_PREFERENCE=3} CommuteModeCategory;

void getParametersFromUser(int* minimumSalary, int* maximumWorkloadPerWeek, int* studyHoursPerWeek, CommuteModeCategory* commuteModeCategory);
Job *readJobs(int *n);
Job *filterJobs(int *n, int *k, Job *jobsArray, int minimumSalary, int maximumWorkloadPerWeek, int studyHoursPerWeek);
void merge(Job jobsFilteredArray[], int start, int end, int mid, CommuteMode commuteMode);
void mergeSort(Job jobsArray[], int start, int end, CommuteMode commuteMode);
double getTTR(Job job, CommuteMode commuteMode);
void printJobs(Job *jobsArray, int numberOfJobs);
void writeHTMLFile(Job jobsFilteredArray[], int numberOfJobsFiltered, CommuteModeCategory commuteModeCategory);
void witeHTMLTables(Job jobsArray[], int n, char commuteMode[100]);
void runTestOfResults(Job jobsArray[], int n, CommuteMode commuteMode);
int debugTest = 0;

int main(int argc, char *argv[]) {

    if (strcasecmp(argv[1], "debugTest1") == 0)
        debugTest = 1;
    

    int numberOfJobs = 0, numberOfJobsFiltered = 0, minimumSalary = 0, maximumWorkloadPerWeek = 0, studyHoursPerWeek = 0;


    CommuteModeCategory commuteModeCategory = NO_PREFERENCE;

    getParametersFromUser(&minimumSalary, &maximumWorkloadPerWeek, &studyHoursPerWeek, &commuteModeCategory);
    
    //Creating arrays for jobs and filtered jobs
    Job *jobsArray = readJobs(&numberOfJobs);
    Job *jobsFilteredArray = filterJobs(&numberOfJobs, &numberOfJobsFiltered, jobsArray, minimumSalary, maximumWorkloadPerWeek, studyHoursPerWeek);
    free(jobsArray);
    if(numberOfJobsFiltered == 0) {
        printf("Ingen jobs matcher de givne kriterier!");
        exit(EXIT_SUCCESS);
    }

    //based on preferred commute mode category; jobs are sorted and printed to user
    writeHTMLFile(jobsFilteredArray, numberOfJobsFiltered, commuteModeCategory);


    free(jobsFilteredArray);

    return 0;
}

//interaktion med user for at få parametre
void getParametersFromUser(int* minimumSalary, int* maximumWorkloadPerWeek, int* studyHoursPerWeek, CommuteModeCategory* commuteModeCategory){   
    double tempInput;

    if (debugTest == 1) {
        *minimumSalary = 5000;
        *studyHoursPerWeek = 24;
        *maximumWorkloadPerWeek = 40;
        *commuteModeCategory = NO_PREFERENCE;
        return;
    }
    // Prompt for minimum salary
    printf("Indtast dit minimumsbeløb for at betale regninger. Skriv 0, hvis du ikke har et beløb: ");
    while(true){
        // Check om input er et gyldigt tal
        if (scanf("%lf", &tempInput) != 1) {
            printf("Ugyldigt input! Vælg et tal: ");
            while (getchar() != '\n'); 
            continue; 
        }
        // Check om input er et heltal
        if (floor(tempInput) != tempInput) {
            printf("Ugyldigt input! Vælg et helt tal: ");
            while (getchar() != '\n'); 
            continue;
        }
        // Check om input er inden for det gyldige interval
        if (tempInput < 0 || tempInput > 15000) {
            printf("Ugyldigt input! Vælg et tal mellem 0 og 15.000: ");
            while (getchar() != '\n'); 
            continue; 
        }
        *minimumSalary = (int)tempInput;
        break;
    }

    // Prompt for study hours per week
    printf("Indtast studietimer pr. uge (0-38): ");
    while(true){
        // Check om input er et gyldigt tal
        if (scanf("%lf", &tempInput) != 1) {
            printf("Ugyldigt input! Vælg et tal: ");
            while (getchar() != '\n'); 
            continue; 
        }
        // Check om input er et heltal
        if (floor(tempInput) != tempInput) {
            printf("Ugyldigt input! Vælg et helt tal: ");
            while (getchar() != '\n'); 
            continue;
        }
        // Check om input er inden for det gyldige interval
        if (tempInput < 0 || tempInput > 38) {
            printf("Ugyldigt input! Vælg et tal mellem 0 og 38: ");
            while (getchar() != '\n'); 
            continue; 
        }
        *studyHoursPerWeek = (int)tempInput;
        break;
    }

    // Prompt for maximum workload per week
    printf("Indtast maksimal samlet arbejdsbyrde (studie+job, max 50 timer): ");
    while(true){
        // Check om input er et gyldigt tal
        if (scanf("%lf", &tempInput) != 1) {
            printf("Ugyldigt input! Vælg et tal: ");
            while (getchar() != '\n'); 
            continue;
        }
        // Check om input er et heltal
        if (floor(tempInput) != tempInput) {
            printf("Ugyldigt input! Vælg et helt tal: ");
            while (getchar() != '\n'); 
            continue;
        }
        // Check om input er inden for det gyldige interval (større end eller lig med studietimer)
        if (tempInput < *studyHoursPerWeek) {
            printf("Ugyldigt input! Minimum arbejdsbyrde er %d timer: ", *studyHoursPerWeek);
            while (getchar() != '\n'); 
            continue;
        }
        // Check om input overskrider den maksimalt tilladte arbejdsbyrde
        if (tempInput > 50) {
            printf("Ugyldigt input! Forskningen siger for stor arbejdsbyrde er skadeligt for mental velbefindende\nMaksimal arbejdsbyrde er 50 timer: ");
            while (getchar() != '\n'); 
            continue;
        }
        
        *maximumWorkloadPerWeek = (int)tempInput;
        break;
    }

    // Prompt for preferred transport mode
    printf("Foretrukken transport (1=Aktiv, 2=Grøn, 3=Ingen): ");
    while(true){
        // Check om input er et gyldigt tal
        if (scanf("%lf", &tempInput) != 1) {
            printf("Ugyldigt input! Vælg et tal: ");
            while (getchar() != '\n'); 
            continue; 
        }
        // Check om input er et heltal
        if (floor(tempInput) != tempInput) {
            printf("Ugyldigt input! Vælg et helt tal: ");
            while (getchar() != '\n'); 
            continue;
        }
        // Check om input er inden for det gyldige interval
        if (tempInput < 1 || tempInput > 3) {
            printf("Ugyldigt input! Vælg 1, 2 eller 3: ");
            while (getchar() != '\n'); 
            continue; 
        }
        *commuteModeCategory = (int)tempInput;
        break;
    }
}

//reads jobs from a file and puts in jobsArray
Job *readJobs(int *n) {
    FILE *fp;
    char row[1000];

    int capacity = 2;
    
    Job *jobsArray = malloc(sizeof(Job)*capacity);

    if (jobsArray == NULL) {
        printf("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    if (debugTest != 0) {
        fp = fopen("./src/jobsForTest.csv", "r"); 
    } else {
        fp = fopen("./src/jobs.csv", "r"); 
    }
    if (fp == NULL) {
        printf("Error opening file. Kontroller, at filen 'jobs.csv' findes i 'src'-mappen.\n");
        exit(EXIT_FAILURE);
    }
    
    int read = 0; // read holds the variables read in each line.

    fgets(row, sizeof(row), fp); // makes sure we skip the first header line

    do {
        //fscanf scans a row in survey.txt, into a given target. Our target is the jobs structure
        read = fscanf(fp, "%99[^,],%99[^,],%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", 
                    jobsArray[*n].title, jobsArray[*n].adress,
                    &jobsArray[*n].salary, &jobsArray[*n].distanceFromAAUInKM, 

                    &jobsArray[*n].workingHoursPerWeek, &jobsArray[*n].travelTimeByWalkInMinutes, 

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

//filters jobs from jobsArray and puts them in jobsFilteredArray (filtering based on user input parameters)
Job *filterJobs(int *n, int *k, Job *jobsArray, int minimumSalary, int maximumWorkloadPerWeek, int studyHoursPerWeek) {
    Job *jobsFilteredArray = malloc(*n * sizeof(Job));

    if (jobsFilteredArray == NULL) {
      printf("Memory not allocated.\n");
      exit(EXIT_FAILURE);
    }

    *k = 0; 
    int i; 
    for (i = 0; i < *n; i++) {
        double currentJobHoursPerWeek = jobsArray[i].workingHoursPerWeek;
        double currentJobMonthlySalary = jobsArray[i].salary * currentJobHoursPerWeek * 4.33;
        double totalWorkload = currentJobHoursPerWeek + studyHoursPerWeek;

        if (currentJobMonthlySalary >= minimumSalary && totalWorkload <= maximumWorkloadPerWeek) {
            jobsFilteredArray[*k] = jobsArray[i]; 
            (*k)++;
        }
    }

    return jobsFilteredArray;
}

//sorts jobsFilteredArray based on TTR in a given commute mode
void mergeSort(Job jobsArray[], int start, int end, CommuteMode commuteMode) {
    int mid = 0;

    if(start < end){
        mid = (end + start) / 2;

        mergeSort(jobsArray, start, mid, commuteMode);
        mergeSort(jobsArray, (mid + 1), end, commuteMode);
        merge(jobsArray, start, end, mid, commuteMode);
    }
}

//merging part of the merge sort algorithm
void merge(Job jobsFilteredArray[], int start, int end, int mid, CommuteMode commuteMode) {
    int i = 0, j = 0;
    
    // Definerer arrays L1 og L2 med korrekt størrelse (halvering af L)
    int lengthL1 = mid - start + 1;
    int lengthL2 = end - mid;

    Job L1[lengthL1];
    Job L2[lengthL2];

    // Fordeler første halvdel af L til L1
    for (i = 0; i < lengthL1; i++) {
        L1[i] = jobsFilteredArray[start + i];
    }
    // Fordeler anden halvdel af L til L2 
    for (i = 0; i < lengthL2; i++) {
        L2[i] = jobsFilteredArray[mid + 1 + i];
    }

    // Resetter i til 0 efter brugen af 'i' i for loops
    i = 0;

    // Introducerer vægtning
    double weightCommute = 0.7; // højere prioritet til commute time qua vores research
    double weightSalary = 0.3;

    // Loop så længe at hhv index i og j ikke overskrider længden af elementer i L1 og L2
    while (i < lengthL1 && j < lengthL2) {
        //få TTR for de tog aktuelle jobs
        double TTR1 = getTTR(L1[i], commuteMode);
        double TTR2 = getTTR(L2[j], commuteMode);
        
        //udregning af scores baseret på commutevægt, TTR og løn med vægtning
        double score1 = weightCommute / TTR1 + L1[i].salary * weightSalary;
        double score2 = weightCommute / TTR2 + L2[j].salary * weightSalary;

        // Hvis L1 har den største score (eller L1 og L2 har samme score) tilføjes L1's element til L 
        if (score1 >= score2) { // Higher score is better
            jobsFilteredArray[start + i + j] = L1[i];
            i++;
        } else {
            jobsFilteredArray[start + i + j] = L2[j];
            j++;
        }
    }

    /* Dette tager højde for elementer "til overs" efter merging. Dvs hvis enten L1 eller L2 er længere, 
    fordi L havde et ulige antal elementer. så alle resterende elementer tilføjes til sidst*/
    while (i < lengthL1) {
        jobsFilteredArray[start + i + j] = L1[i];
        i++;
    }
    while (j < lengthL2) {
        jobsFilteredArray[start + i + j] = L2[j];
        j++;
    }
}

//calculates the TTR for the student based on the given job
double getTTR(Job job, CommuteMode commuteMode) {
    double TTR = 0;
    double workDaysPerWeek;
    double workloadInMinutes = (job.workingHoursPerWeek)*60;

    if (job.workingHoursPerWeek <= 8)
        workDaysPerWeek = 1;
    else if (job.workingHoursPerWeek > 8 && job.workingHoursPerWeek <= 16) 
        workDaysPerWeek = 2;
    else if(job.workingHoursPerWeek > 16 && job.workingHoursPerWeek <= 24) 
        workDaysPerWeek = 3;
    else if (job.workingHoursPerWeek > 24 && job.workingHoursPerWeek <= 32) 
        workDaysPerWeek = 4;
    else if (job.workingHoursPerWeek > 32 && job.workingHoursPerWeek <= 40) 
        workDaysPerWeek = 5;
    else if (job.workingHoursPerWeek > 40 && job.workingHoursPerWeek <= 48)
        workDaysPerWeek = 6;
    else
        workDaysPerWeek = 7;

    switch (commuteMode) {
        case WALK: TTR = (job.travelTimeByWalkInMinutes * workDaysPerWeek) / (workloadInMinutes); break;
        case BIKE: TTR = (job.travelTimeByBikeInMinutes * workDaysPerWeek) / workloadInMinutes; break;
        case PUBLIC_TRANSPORT: TTR = (job.travelTimeByPublicInMinutes * workDaysPerWeek) / workloadInMinutes; break;
        case CAR: TTR = (job.travelTimeByCarInMinutes* workDaysPerWeek) / workloadInMinutes; break;
        default: break;
    }

    //printf("TTR for job: %lf\n", TTR);

   return TTR;
}

//prints the sorted jobs
void printJobs(Job *jobsArray, int numberOfJobs) {
    int i;
    int count;

    if (numberOfJobs > 10)
        count = 10;
    else 
        count = numberOfJobs;

    printf("Titel                           Adresse                              Timeloen                 afstand            arbejdstimer              gå              cykle               offentligt            bil\n");
    for (i = 0; i < count; i++) {
        printf("%-30s %-30s %15.1lf/time %15.1lfkm %15.1lf timer/uge %15.1lfmin %15.1lfmin %15.1lfmin %15.1lfmin \n",
        jobsArray[i].title, 
        jobsArray[i].adress, 
        jobsArray[i].salary, 
        jobsArray[i].distanceFromAAUInKM, 
        jobsArray[i].workingHoursPerWeek,
        jobsArray[i].travelTimeByWalkInMinutes,
        jobsArray[i].travelTimeByBikeInMinutes,
        jobsArray[i].travelTimeByPublicInMinutes,
        jobsArray[i].travelTimeByCarInMinutes);
    }
}


void writeHTMLFile(Job jobsFilteredArray[], int numberOfJobsFiltered, CommuteModeCategory commuteModeCategory){
    FILE *fp;

    fp = fopen("./src/output.html", "w"); //opens the file in write mode;
    if (fp == NULL) {
        printf("Error opening file");
    }
    fputs("<link rel='stylesheet' href='style.css'>\n", fp); //html code connects to the file styles.css
    fputs(" <div class=\"tableContainer\">", fp);

    if (commuteModeCategory == ACTIVE || commuteModeCategory == GREEN || commuteModeCategory == NO_PREFERENCE) {
        mergeSort(jobsFilteredArray, 0, numberOfJobsFiltered - 1, WALK);
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
        witeHTMLTables(jobsFilteredArray, numberOfJobsFiltered, "Gå");
        runTestOfResults(jobsFilteredArray, numberOfJobsFiltered, WALK);

        mergeSort(jobsFilteredArray, 0, numberOfJobsFiltered - 1, BIKE);
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
        witeHTMLTables(jobsFilteredArray, numberOfJobsFiltered, "Cykle");
        runTestOfResults(jobsFilteredArray, numberOfJobsFiltered, BIKE);

    }
    if (commuteModeCategory == GREEN || commuteModeCategory == NO_PREFERENCE) {
        mergeSort(jobsFilteredArray, 0, numberOfJobsFiltered - 1, PUBLIC_TRANSPORT);
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
        witeHTMLTables(jobsFilteredArray, numberOfJobsFiltered, "Offentlig-transport");
        runTestOfResults(jobsFilteredArray, numberOfJobsFiltered, PUBLIC_TRANSPORT);
    }
    if (commuteModeCategory == NO_PREFERENCE) {
        mergeSort(jobsFilteredArray, 0, numberOfJobsFiltered - 1, CAR);
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
        witeHTMLTables(jobsFilteredArray, numberOfJobsFiltered, "Bil");   
        runTestOfResults(jobsFilteredArray, numberOfJobsFiltered, CAR);
    }
    fputs(" </div>", fp);
    fclose(fp);
}


void witeHTMLTables(Job jobsArray[], int n, char commuteMode[100]) {
    int count;


    if (n > 10)
        count = 10;
    else 
        count = n;
    FILE *fp;

    fp = fopen("./src/output.html", "a"); //opens the file in add write mode;
    if (fp == NULL) {
        printf("Error opening file");
    }

    //html code connects to the file styles.css
    fputs("<link rel='stylesheet' href='style.css'>\n", fp);

    char salary[100];
    char distanceFromAAUInKM[100];
    char travelTimeByWalkInMinutes[100];
    char travelTimeByBikeInMinutes[100];
    char travelTimeByPublicInMinutes[100];
    char travelTimeByCarInMinutes[100];
    char workingHoursPerWeek[100];
    char comumuteMode10[100];

    fputs(" <div class=\"tableWrapper\">", fp);
    fputs(" <h3>", fp);
    sprintf(comumuteMode10, "Top 10 for %s", commuteMode);
    fputs(comumuteMode10, fp);
    fputs(" </h3>", fp);
    fputs(" <table>", fp);
    fputs(" <tr>", fp);
        fputs(" <th>", fp);
        fputs("Title", fp);
        fputs(" </th>\n", fp);

        fputs(" <th>", fp);
        fputs("Adresse", fp);
        fputs(" </th>\n", fp);

        fputs(" <th>", fp);
        fputs("Afstand", fp);
        fputs(" </th>\n", fp);

        fputs(" <th>", fp);
        fputs("Gå", fp);
        fputs(" </th>\n", fp);

        fputs(" <th>", fp);
        fputs("Cykle", fp);
        fputs(" </th>\n", fp);

        fputs(" <th>", fp);
        fputs("Offentligtransport", fp);
        fputs(" </th>\n", fp);

        fputs(" <th>", fp);
        fputs("Bil", fp);
        fputs(" </th>\n", fp);

        fputs(" <th>", fp);
        fputs("Time løn", fp);
        fputs(" </th>\n", fp);

        fputs(" <th>", fp);
        fputs("Timer om ugen", fp);
        fputs(" </th>\n", fp);

        fputs(" </tr>", fp);
        for (int i = 0; i < count; i++) {
            fputs(" <tr>", fp);
                fputs(" <td>", fp);
                fputs(jobsArray[i].title, fp);
                fputs("</td>\n", fp);

                fputs(" <td>", fp);
                fputs(jobsArray[i].adress, fp);
                fputs(" </td>\n", fp);

                sprintf(distanceFromAAUInKM, "%.1lf km", jobsArray[i].distanceFromAAUInKM);
                fputs(" <td>", fp);
                fputs(distanceFromAAUInKM, fp);
                fputs("</td>\n", fp);

                sprintf(travelTimeByWalkInMinutes, "%.1lf min", jobsArray[i].travelTimeByWalkInMinutes);
                fputs(" <td>", fp);
                fputs(travelTimeByWalkInMinutes, fp);
                fputs("</td>\n", fp);

                sprintf(travelTimeByBikeInMinutes, "%.1lf min", jobsArray[i].travelTimeByBikeInMinutes);
                fputs(" <td>", fp);
                fputs(travelTimeByBikeInMinutes, fp);
                fputs("</td>\n", fp);
                
                sprintf(travelTimeByPublicInMinutes, "%.1lf min", jobsArray[i].travelTimeByPublicInMinutes);
                fputs(" <td>", fp);
                fputs(travelTimeByPublicInMinutes, fp);
                fputs("</td>\n", fp);

                sprintf(travelTimeByCarInMinutes, "%.1lf min", jobsArray[i].travelTimeByCarInMinutes);
                fputs(" <td>", fp);
                fputs(travelTimeByCarInMinutes, fp);
                fputs("</td>\n", fp);
                
                sprintf(salary, "%.1lf dkk", jobsArray[i].salary);
                fputs(" <td>", fp);
                fputs(salary, fp);
                fputs("</t>\n", fp);
             
                sprintf(workingHoursPerWeek, "%.1lf timer", jobsArray[i].workingHoursPerWeek);
                fputs(" <td>", fp);
                fputs(workingHoursPerWeek, fp);
                fputs("</t>\n", fp);

            fputs(" </tr>", fp);
        }
        fputs(" </table>", fp);
        fputs(" </div>", fp);
}

// if debug mode is on, it checks if the jobsForTest.csv has been sorted correctly
void runTestOfResults(Job jobsArray[], int n, CommuteMode commuteMode) {
    if (debugTest == 1) {
        assert(n == 2);
        if (commuteMode == WALK || commuteMode == BIKE) {
            assert(strcmp(jobsArray[0].title, "Studentermedhjælper i IT") == 0);
            assert(strcmp(jobsArray[1].title, "Barista") == 0);
        } else if (commuteMode == CAR || commuteMode == PUBLIC_TRANSPORT) {
            assert(strcmp(jobsArray[0].title, "Barista") == 0);
            assert(strcmp(jobsArray[1].title, "Studentermedhjælper i IT") == 0);
        }
        printf("Test completed successfully!\n");
    } else {
        return;
    }
}
