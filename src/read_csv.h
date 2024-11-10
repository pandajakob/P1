

typedef struct {
    int Id;
    char title[100];
    double salary; //
    char adress[100]; // navn på adresse
    double timeFromAAU; // estimeret tid givet af de studerende
    double distanceFromAAU; // km
} Job;

int read_csv(Job jobs[]);