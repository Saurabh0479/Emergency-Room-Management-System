#ifndef EMERGENCY_H
#define EMERGENCY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Constants
#define MAX_PATIENTS 100
#define MAX_DOCTORS 20
#define MAX_NAME_LENGTH 50
#define MAX_CONDITION_LENGTH 100
#define DATA_FILE "data.txt"

// ==================== ENUMS ====================

typedef enum {
    CRITICAL = 1,
    SEVERE = 2,
    MODERATE = 3,
    MILD = 4
} Severity;

typedef enum {
    WAITING = 1,
    IN_TREATMENT = 2,
    TREATED = 3,
    DISCHARGED = 4
} PatientStatus;

typedef enum {
    AVAILABLE = 1,
    BUSY = 2,
    OFF_DUTY = 3
} DoctorStatus;

// ==================== STRUCTURES ====================

typedef struct {
    int patient_id;
    char name[MAX_NAME_LENGTH];
    int age;
    char condition[MAX_CONDITION_LENGTH];
    Severity severity;
    PatientStatus status;
    int assigned_doctor_id;
    float bill_amount;
    time_t arrival_time;
    time_t treatment_start_time;
    time_t treatment_end_time;
} Patient;

typedef struct {
    int doctor_id;
    char name[MAX_NAME_LENGTH];
    char specialization[MAX_NAME_LENGTH];
    DoctorStatus status;
    int current_patient_id;
} Doctor;

typedef struct {
    Patient patients[MAX_PATIENTS];
    Doctor doctors[MAX_DOCTORS];
    int total_patients;
    int total_doctors;
    int patient_counter;
} Hospital;

// ==================== FUNCTION DECLARATIONS ====================

// Initialization
void initialize_hospital(Hospital *h);

// Patient Operations
void register_patient(Hospital *h);
void display_patients(Hospital *h);
void search_patient(Hospital *h);

// Doctor Operations
void add_doctor(Hospital *h);
void display_doctors(Hospital *h);

// Treatment Operations
void assign_doctor_to_patient(Hospital *h);
void start_treatment(Hospital *h);
void end_treatment(Hospital *h);

// Queue & Priority
int get_highest_priority_patient(Hospital *h);
void display_patient_queue(Hospital *h);

// Billing
void calculate_bill(Patient *p);

// Reporting
void generate_report(Hospital *h);

// Utility Functions
char* get_severity_string(Severity s);
char* get_status_string(PatientStatus s);
char* get_doctor_status_string(DoctorStatus s);

// Menu
void display_menu(void);

// Data Persistence
void save_hospital_data(Hospital *h);
void load_hospital_data(Hospital *h);
void save_patients_to_file(Hospital *h);
void save_doctors_to_file(Hospital *h);
void load_patients_from_file(Hospital *h);
void load_doctors_from_file(Hospital *h);

#endif // EMERGENCY_H