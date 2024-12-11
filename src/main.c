#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

/*kør programmet windows
gcc -pedantic -Wall src/main.c -o ./bin/a.exe
./bin/a.exe
*/
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

void getParametersFromUser(int* minimumSalary, int* timeFromHomeToAAUInMinutes, int* maximumWorkloadPerWeek, int* studyHoursPerWeek, CommuteModeCategory* commuteModeCategory, int isDebugMode);
Job *readJobs(int *n);
Job *filterJobs(int *n, int *k, Job *jobsArray, int minimumSalary, int maximumWorkloadPerWeek, int studyHoursPerWeek, char jobTag[]);
int checkForJobTag(char jobTitle[], char jobTag[]);
void merge(Job jobsFilteredArray[], int start, int end, int mid, CommuteMode commuteMode);
void mergeSort(Job jobsArray[], int start, int end, CommuteMode commuteMode);
double getTTR(Job job, CommuteMode commuteMode);
void printJobs(Job *jobsArray, int numberOfJobs);
void writeHTMLFile(Job jobsArray[], int n);

int main(int argc, char *argv[]) {
    int isDebugMode = argc > 1;
    int numberOfJobs = 0, numberOfJobsFiltered = 0, minimumSalary = 0, timeFromHomeToAAUInMinutes = 0, maximumWorkloadPerWeek = 0, studyHoursPerWeek = 0;
    char jobTag[100];

    CommuteModeCategory commuteModeCategory = NO_PREFERENCE;

    getParametersFromUser(&minimumSalary, &timeFromHomeToAAUInMinutes, &maximumWorkloadPerWeek, &studyHoursPerWeek, &commuteModeCategory, isDebugMode);
    /*printf("Jobtags (q for ingen): ");
    scanf("%s", jobTag);
    
    printf("Minimum Salary: %d\nTime to AAU: %d\nAvg. Time on studies: %d\nChoosen comuute mode: %d\n", 
            inputMinimumSalary, inputTimeFromHomeToAAUInMinutes, inputTimeOnStudiesInHours, inputCommuteModeCategory);*/
    
    //Creating arrays for jobs and filtered jobs
    Job *jobsArray = readJobs(&numberOfJobs);
    Job *jobsFilteredArray = filterJobs(&numberOfJobs, &numberOfJobsFiltered, jobsArray, minimumSalary, maximumWorkloadPerWeek, studyHoursPerWeek, jobTag);
    free(jobsArray);

    //based on preferred commute mode category; jobs are sorted and printed to user
    if (commuteModeCategory == ACTIVE) {
        mergeSort(jobsFilteredArray, 0, numberOfJobsFiltered - 1, WALK);
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
    }
    else if (commuteModeCategory == GREEN) {
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
    }
    else {
        printJobs(jobsFilteredArray, numberOfJobsFiltered);
    }

    //writeHTMLFile(jobsFilteredArray, numberOfJobsFiltered);

    free(jobsFilteredArray);

    return 0;
}

//interaktion med user for at få parametre
void getParametersFromUser(int* minimumSalary, int* timeFromHomeToAAUInMinutes, int* maximumWorkloadPerWeek, int* studyHoursPerWeek, CommuteModeCategory* commuteModeCategory, int isDebugMode){   
    double tempInput;

    // Prompt for minimum salary
    printf("Indtast dit minimumsbeløb for at betale regninger. Skriv 0, hvis du ikke har et beløb: ");
    if (isDebugMode) {
        // indsæt test-værdier
        return;
    }
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

    // Prompt for time from home to AAU
    printf("Indtast transporttid til AAU (1-90 min): ");
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
        if (tempInput < 1 || tempInput > 90) {
            printf("Ugyldigt input! Vælg et tal mellem 1 og 90: ");
            while (getchar() != '\n'); 
            continue; 
        }
        *timeFromHomeToAAUInMinutes = (int)tempInput;
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
    
    fp = fopen("./src/survey.txt", "r"); 

    if (fp == NULL) {
        printf("Error opening file. Kontroller, at filen 'survey.txt' findes i 'src'-mappen.\n");
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
Job *filterJobs(int *n, int *k, Job *jobsArray, int minimumSalary, int maximumWorkloadPerWeek, int studyHoursPerWeek, char jobTag[]) {
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
        //&& checkForJobTag(jobsArray[i].title, jobTag)
        }
    }

    return jobsFilteredArray;
}


int checkForJobTag(char jobTitle[], char jobTag[]) {
    if (strstr(jobTag, "q") != NULL) {
        return 1;
    }

    // copies the strings, so we don't change the original
    char jobTitleCopy[99];
    char jobTagCopy[99];
    strcpy(jobTitleCopy, jobTitle);
    strcpy(jobTagCopy, jobTag);

    //lowercases both strings in order to compare them equally
    int i;
    for (i = 0; i < 100; i++) {
        jobTitleCopy[i] = toupper(jobTitle[i]);
    }
    for (i = 0; i < 100; i++) {
        jobTagCopy[i] = toupper(jobTag[i]);
    }

    // checks if the tag is in the job title
    if (strstr(jobTitleCopy, jobTagCopy) != NULL) {
        return 1;
    }
    return 0;
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
void merge(Job jobsFilteredArray[], int start, int end, int mid, CommuteMode commuteMode){
    int i = 0, j = 0, k = 0;

    // Definerer arrays L1 og L2 med korrekt størrelse (halvering af L)
    int lengthL1 = mid - start + 1;
    int lengthL2 = end - mid;
    double TTR1, TTR2;

    Job L1[lengthL1];
    Job L2[lengthL2];

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
        TTR1 = getTTR(L1[i], commuteMode);
        TTR2 = getTTR(L2[j], commuteMode);
        if (TTR1 <= TTR2){
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

//calculates the TTR for the student based on the given job
double getTTR(Job job, CommuteMode commuteMode) {
    double TTR = 0;
    double workloadInMinutes = (job.workingHoursPerWeek)*60;
    
    switch (commuteMode) {
        case WALK: TTR = job.travelTimeByWalkInMinutes/workloadInMinutes; break;
        case BIKE: TTR = job.travelTimeByWalkInMinutes/workloadInMinutes; break;
        case PUBLIC_TRANSPORT: TTR = job.travelTimeByWalkInMinutes/workloadInMinutes; break;
        case CAR: TTR = job.travelTimeByWalkInMinutes/workloadInMinutes; break;
        default: break;
    }

    printf("TTR for job: %lf\n", TTR);

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


void writeHTMLFile(Job jobsArray[], int n) {
    /*int count = 10;
    
    if (n<10)
        count = n;*/
    
    FILE *fp;

    fp = fopen("./src/output.html", "w"); //åbner filen i write mode;
    if (fp == NULL) {
        printf("Error opening file");
    }

    //html kode, som connecter filen til styles.css
    fputs("<link rel='stylesheet' href='style.css'>\n", fp);

    char salary[100];
    char distanceFromAAUInKM[100];
    char travelTimeByWalkInMinutes[100];
    char travelTimeByBikeInMinutes[100];
    char travelTimeByPublicInMinutes[100];
    char travelTimeByCarInMinutes[100];

    fputs(" <table>", fp);
        fputs(" </tr>", fp);
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
            fputs("Løn", fp);
            fputs(" </th>\n", fp);
        fputs(" </tr>", fp);
    for (int i = 0; i < n; i++) {
        fputs(" <tr>", fp);
            fputs(" <td>", fp);
            fputs(jobsArray[i].title, fp);
            fputs("</td>\n", fp);

            fputs(" <td>", fp);
            fputs(jobsArray[i].adress, fp);
            fputs(" </td>\n", fp);

            sprintf(distanceFromAAUInKM, "%.1lf", jobsArray[i].distanceFromAAUInKM);
            fputs(" <td>", fp);
            fputs(distanceFromAAUInKM, fp);
            fputs("</td>\n", fp);

            sprintf(travelTimeByWalkInMinutes, "%.1lf", jobsArray[i].travelTimeByWalkInMinutes);
            fputs(" <td>", fp);
            fputs(travelTimeByWalkInMinutes, fp);
            fputs("</td>\n", fp);

            sprintf(travelTimeByBikeInMinutes, "%.1lf", jobsArray[i].travelTimeByBikeInMinutes);
            fputs(" <td>", fp);
            fputs(travelTimeByBikeInMinutes, fp);
            fputs("</td>\n", fp);
            
            sprintf(travelTimeByPublicInMinutes, "%.1lf", jobsArray[i].travelTimeByPublicInMinutes);
            fputs(" <td>", fp);
            fputs(travelTimeByPublicInMinutes, fp);
            fputs("</td>\n", fp);

            sprintf(travelTimeByCarInMinutes, "%.1lf", jobsArray[i].travelTimeByCarInMinutes);
            fputs(" <td>", fp);
            fputs(travelTimeByCarInMinutes, fp);
            fputs("</td>\n", fp);
            
            sprintf(salary, "%.1lf", jobsArray[i].salary);
            fputs(" <td>", fp);
            fputs(salary, fp);
            fputs("</t>\n", fp);
        fputs(" </tr>", fp);
    }
    fputs(" <table>", fp);

}
