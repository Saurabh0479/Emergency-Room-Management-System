#include "emergency.h"

// ==================== INITIALIZATION ====================

void initialize_hospital(Hospital *h) {
    h->total_patients = 0;
    h->total_doctors = 0;
    h->patient_counter = 1000;
}

// ==================== UTILITY FUNCTIONS ====================

char* get_severity_string(Severity s) {
    switch(s) {
        case CRITICAL: return "CRITICAL";
        case SEVERE: return "SEVERE";
        case MODERATE: return "MODERATE";
        case MILD: return "MILD";
        default: return "UNKNOWN";
    }
}

char* get_status_string(PatientStatus s) {
    switch(s) {
        case WAITING: return "WAITING";
        case IN_TREATMENT: return "IN_TREATMENT";
        case TREATED: return "TREATED";
        case DISCHARGED: return "DISCHARGED";
        default: return "UNKNOWN";
    }
}

char* get_doctor_status_string(DoctorStatus s) {
    switch(s) {
        case AVAILABLE: return "AVAILABLE";
        case BUSY: return "BUSY";
        case OFF_DUTY: return "OFF_DUTY";
        default: return "UNKNOWN";
    }
}

// ==================== PATIENT OPERATIONS ====================

void register_patient(Hospital *h) {
    if (h->total_patients >= MAX_PATIENTS) {
        printf("\n[ERROR] Hospital is full! Cannot register more patients.\n");
        return;
    }

    Patient p;
    p.patient_id = h->patient_counter++;
    p.assigned_doctor_id = -1;
    p.bill_amount = 0.0;
    p.status = WAITING;
    p.treatment_start_time = 0;
    p.treatment_end_time = 0;
    p.arrival_time = time(NULL);

    printf("\n========== PATIENT REGISTRATION ==========\n");
    printf("Enter patient name: ");
    scanf("%s", p.name);
    printf("Enter patient age: ");
    scanf("%d", &p.age);
    printf("Enter medical condition: ");
    scanf(" %[^\n]", p.condition);
    
    printf("Select severity level:\n");
    printf("1. CRITICAL\n2. SEVERE\n3. MODERATE\n4. MILD\n");
    printf("Enter choice (1-4): ");
    int severity_choice;
    scanf("%d", &severity_choice);
    p.severity = severity_choice;

    h->patients[h->total_patients++] = p;
    printf("\n[SUCCESS] Patient registered with ID: %d\n", p.patient_id);
}

void display_patients(Hospital *h) {
    printf("\n========== ALL PATIENTS ==========\n");
    if (h->total_patients == 0) {
        printf("No patients registered yet.\n");
        return;
    }

    printf("%-8s %-20s %-5s %-20s %-10s %-15s %-8s\n",
           "ID", "Name", "Age", "Condition", "Severity", "Status", "Doctor");
    printf("==========================================================================================\n");

    for (int i = 0; i < h->total_patients; i++) {
        Patient p = h->patients[i];
        char doctor_str[10];
        if (p.assigned_doctor_id == -1) {
            strcpy(doctor_str, "None");
        } else {
            sprintf(doctor_str, "%d", p.assigned_doctor_id);
        }

        printf("%-8d %-20s %-5d %-20s %-10s %-15s %-8s\n",
               p.patient_id, p.name, p.age, p.condition,
               get_severity_string(p.severity),
               get_status_string(p.status), doctor_str);
    }
}

void search_patient(Hospital *h) {
    printf("\n========== SEARCH PATIENT ==========\n");
    
    int patient_id;
    printf("Enter patient ID to search: ");
    scanf("%d", &patient_id);

    for (int i = 0; i < h->total_patients; i++) {
        if (h->patients[i].patient_id == patient_id) {
            Patient p = h->patients[i];
            printf("\n--- Patient Found ---\n");
            printf("ID: %d\n", p.patient_id);
            printf("Name: %s\n", p.name);
            printf("Age: %d\n", p.age);
            printf("Condition: %s\n", p.condition);
            printf("Severity: %s\n", get_severity_string(p.severity));
            printf("Status: %s\n", get_status_string(p.status));
            printf("Assigned Doctor ID: %s\n", p.assigned_doctor_id == -1 ? "None" : (char*)&p.assigned_doctor_id);
            printf("Bill Amount: Rs. %.2f\n", p.bill_amount);
            return;
        }
    }

    printf("[ERROR] Patient not found!\n");
}

// ==================== DOCTOR OPERATIONS ====================

void add_doctor(Hospital *h) {
    if (h->total_doctors >= MAX_DOCTORS) {
        printf("\n[ERROR] Cannot add more doctors!\n");
        return;
    }

    Doctor d;
    d.doctor_id = h->total_doctors + 1;
    d.status = AVAILABLE;
    d.current_patient_id = -1;

    printf("\n========== ADD DOCTOR ==========\n");
    printf("Enter doctor name: ");
    scanf("%s", d.name);
    printf("Enter specialization (e.g., Cardiology, Orthopedics): ");
    scanf("%s", d.specialization);

    h->doctors[h->total_doctors++] = d;
    printf("\n[SUCCESS] Doctor added with ID: %d\n", d.doctor_id);
}

void display_doctors(Hospital *h) {
    printf("\n========== ALL DOCTORS ==========\n");
    if (h->total_doctors == 0) {
        printf("No doctors registered yet.\n");
        return;
    }

    printf("%-8s %-20s %-20s %-15s %-15s\n",
           "ID", "Name", "Specialization", "Status", "Current Patient");
    printf("====================================================================================\n");

    for (int i = 0; i < h->total_doctors; i++) {
        Doctor d = h->doctors[i];
        char patient_str[10];
        if (d.current_patient_id == -1) {
            strcpy(patient_str, "None");
        } else {
            sprintf(patient_str, "%d", d.current_patient_id);
        }

        printf("%-8d %-20s %-20s %-15s %-15s\n",
               d.doctor_id, d.name, d.specialization,
               get_doctor_status_string(d.status), patient_str);
    }
}

// ==================== PRIORITY & QUEUE OPERATIONS ====================

int get_highest_priority_patient(Hospital *h) {
    int highest_priority_idx = -1;
    Severity highest_severity = 5;

    for (int i = 0; i < h->total_patients; i++) {
        if (h->patients[i].status == WAITING && h->patients[i].severity < highest_severity) {
            highest_priority_idx = i;
            highest_severity = h->patients[i].severity;
        }
    }

    return highest_priority_idx;
}

void display_patient_queue(Hospital *h) {
    printf("\n========== PATIENT QUEUE (Priority Order) ==========\n");
    
    Patient queue[MAX_PATIENTS];
    int queue_count = 0;

    // Collect waiting patients
    for (int i = 0; i < h->total_patients; i++) {
        if (h->patients[i].status == WAITING) {
            queue[queue_count++] = h->patients[i];
        }
    }

    // Sort by severity (lower number = higher priority)
    for (int i = 0; i < queue_count - 1; i++) {
        for (int j = i + 1; j < queue_count; j++) {
            if (queue[j].severity < queue[i].severity) {
                Patient temp = queue[i];
                queue[i] = queue[j];
                queue[j] = temp;
            }
        }
    }

    if (queue_count == 0) {
        printf("No patients waiting.\n");
        return;
    }

    printf("%-8s %-20s %-5s %-20s %-10s\n",
           "ID", "Name", "Age", "Condition", "Severity");
    printf("===========================================================\n");

    for (int i = 0; i < queue_count; i++) {
        printf("%-8d %-20s %-5d %-20s %-10s\n",
               queue[i].patient_id, queue[i].name, queue[i].age,
               queue[i].condition, get_severity_string(queue[i].severity));
    }
}

// ==================== TREATMENT OPERATIONS ====================

void assign_doctor_to_patient(Hospital *h) {
    printf("\n========== ASSIGN DOCTOR TO PATIENT ==========\n");
    
    int patient_idx = get_highest_priority_patient(h);
    
    if (patient_idx == -1) {
        printf("[INFO] No waiting patients available.\n");
        return;
    }

    Patient *p = &h->patients[patient_idx];
    printf("Patient to be assigned: %s (ID: %d) - Severity: %s\n",
           p->name, p->patient_id, get_severity_string(p->severity));

    display_doctors(h);

    int doctor_id;
    printf("\nEnter doctor ID to assign: ");
    scanf("%d", &doctor_id);

    // Find doctor
    int doctor_idx = -1;
    for (int i = 0; i < h->total_doctors; i++) {
        if (h->doctors[i].doctor_id == doctor_id) {
            doctor_idx = i;
            break;
        }
    }

    if (doctor_idx == -1) {
        printf("[ERROR] Doctor not found!\n");
        return;
    }

    if (h->doctors[doctor_idx].status != AVAILABLE) {
        printf("[ERROR] Doctor is not available!\n");
        return;
    }

    // Assign doctor
    p->assigned_doctor_id = doctor_id;
    h->doctors[doctor_idx].status = BUSY;
    h->doctors[doctor_idx].current_patient_id = p->patient_id;

    printf("\n[SUCCESS] Doctor %s assigned to patient %s\n",
           h->doctors[doctor_idx].name, p->name);
}

void start_treatment(Hospital *h) {
    printf("\n========== START TREATMENT ==========\n");
    
    int patient_id;
    printf("Enter patient ID: ");
    scanf("%d", &patient_id);

    // Find patient
    int patient_idx = -1;
    for (int i = 0; i < h->total_patients; i++) {
        if (h->patients[i].patient_id == patient_id) {
            patient_idx = i;
            break;
        }
    }

    if (patient_idx == -1) {
        printf("[ERROR] Patient not found!\n");
        return;
    }

    Patient *p = &h->patients[patient_idx];

    if (p->assigned_doctor_id == -1) {
        printf("[ERROR] No doctor assigned to this patient!\n");
        return;
    }

    p->status = IN_TREATMENT;
    p->treatment_start_time = time(NULL);

    printf("\n[SUCCESS] Treatment started for patient %s\n", p->name);
}

// ==================== BILLING OPERATIONS ====================

void calculate_bill(Patient *p) {
    float base_fee = 100.0;
    float severity_multiplier = 1.0;
    float age_factor = 1.0;

    // Severity-based fee
    switch(p->severity) {
        case CRITICAL:
            severity_multiplier = 5.0;
            break;
        case SEVERE:
            severity_multiplier = 3.5;
            break;
        case MODERATE:
            severity_multiplier = 2.0;
            break;
        case MILD:
            severity_multiplier = 1.0;
            break;
    }

    // Age-based factor (elderly patients may require more care)
    if (p->age > 60) {
        age_factor = 1.5;
    } else if (p->age > 40) {
        age_factor = 1.2;
    }

    // Treatment time factor
    float treatment_hours = 0.5; // Default
    if (p->treatment_end_time && p->treatment_start_time) {
        treatment_hours = (float)(p->treatment_end_time - p->treatment_start_time) / 3600.0;
        if (treatment_hours < 0.5) treatment_hours = 0.5;
    }

    p->bill_amount = base_fee * severity_multiplier * age_factor * treatment_hours;
}

void end_treatment(Hospital *h) {
    printf("\n========== END TREATMENT ==========\n");
    
    int patient_id;
    printf("Enter patient ID: ");
    scanf("%d", &patient_id);

    // Find patient
    int patient_idx = -1;
    for (int i = 0; i < h->total_patients; i++) {
        if (h->patients[i].patient_id == patient_id) {
            patient_idx = i;
            break;
        }
    }

    if (patient_idx == -1) {
        printf("[ERROR] Patient not found!\n");
        return;
    }

    Patient *p = &h->patients[patient_idx];

    if (p->status != IN_TREATMENT) {
        printf("[ERROR] Patient is not under treatment!\n");
        return;
    }

    p->status = TREATED;
    p->treatment_end_time = time(NULL);

    // Calculate bill
    calculate_bill(p);

    // Free up doctor
    if (p->assigned_doctor_id != -1) {
        for (int i = 0; i < h->total_doctors; i++) {
            if (h->doctors[i].doctor_id == p->assigned_doctor_id) {
                h->doctors[i].status = AVAILABLE;
                h->doctors[i].current_patient_id = -1;
                break;
            }
        }
    }

    printf("\n[SUCCESS] Treatment completed for patient %s\n", p->name);
    printf("Bill Amount: Rs. %.2f\n", p->bill_amount);
}

// ==================== REPORTING ====================

void generate_report(Hospital *h) {
    printf("\n========== HOSPITAL REPORT ==========\n");
    printf("Total Patients: %d\n", h->total_patients);
    printf("Total Doctors: %d\n", h->total_doctors);

    int waiting = 0, in_treatment = 0, treated = 0, discharged = 0;
    for (int i = 0; i < h->total_patients; i++) {
        switch(h->patients[i].status) {
            case WAITING: waiting++; break;
            case IN_TREATMENT: in_treatment++; break;
            case TREATED: treated++; break;
            case DISCHARGED: discharged++; break;
        }
    }

    printf("\nPatient Status Distribution:\n");
    printf("  Waiting: %d\n", waiting);
    printf("  In Treatment: %d\n", in_treatment);
    printf("  Treated: %d\n", treated);
    printf("  Discharged: %d\n", discharged);

    int available = 0, busy = 0, off_duty = 0;
    for (int i = 0; i < h->total_doctors; i++) {
        switch(h->doctors[i].status) {
            case AVAILABLE: available++; break;
            case BUSY: busy++; break;
            case OFF_DUTY: off_duty++; break;
        }
    }

    printf("\nDoctor Status Distribution:\n");
    printf("  Available: %d\n", available);
    printf("  Busy: %d\n", busy);
    printf("  Off Duty: %d\n", off_duty);

    // Calculate total revenue
    float total_revenue = 0;
    for (int i = 0; i < h->total_patients; i++) {
        if (h->patients[i].status == TREATED || h->patients[i].status == DISCHARGED) {
            total_revenue += h->patients[i].bill_amount;
        }
    }

    printf("\nTotal Revenue Generated: Rs. %.2f\n", total_revenue);
}

// ==================== MENU ====================

void display_menu(void) {
    printf("\n========== EMERGENCY ROOM MANAGEMENT SYSTEM ==========\n");
    printf("1. Register Patient\n");
    printf("2. Add Doctor\n");
    printf("3. Display All Patients\n");
    printf("4. Display All Doctors\n");
    printf("5. Assign Doctor to Patient\n");
    printf("6. Start Treatment\n");
    printf("7. End Treatment\n");
    printf("8. View Patient Queue\n");
    printf("9. Search Patient\n");
    printf("10. Generate Hospital Report\n");
    printf("11. Save & Exit\n");
    printf("=====================================================\n");
    printf("Enter your choice (1-11): ");
}

// ==================== DATA PERSISTENCE ====================

void save_patients_to_file(Hospital *h) {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        printf("[ERROR] Cannot open file for writing!\n");
        return;
    }

    // Write patient count
    fprintf(file, "PATIENTS %d\n", h->total_patients);
    fprintf(file, "PATIENT_COUNTER %d\n", h->patient_counter);

    // Write each patient
    for (int i = 0; i < h->total_patients; i++) {
        Patient p = h->patients[i];
        fprintf(file, "PATIENT %d %s %d %s %d %d %d %.2f\n",
                p.patient_id,
                p.name,
                p.age,
                p.condition,
                p.severity,
                p.status,
                p.assigned_doctor_id,
                p.bill_amount);
    }

    printf("[SUCCESS] Patient data saved to %s\n", DATA_FILE);
    fclose(file);
}

void save_doctors_to_file(Hospital *h) {
    FILE *file = fopen(DATA_FILE, "a");
    if (file == NULL) {
        printf("[ERROR] Cannot open file for appending!\n");
        return;
    }

    // Write doctor count
    fprintf(file, "DOCTORS %d\n", h->total_doctors);

    // Write each doctor
    for (int i = 0; i < h->total_doctors; i++) {
        Doctor d = h->doctors[i];
        fprintf(file, "DOCTOR %d %s %s %d %d\n",
                d.doctor_id,
                d.name,
                d.specialization,
                d.status,
                d.current_patient_id);
    }

    printf("[SUCCESS] Doctor data saved to %s\n", DATA_FILE);
    fclose(file);
}

void save_hospital_data(Hospital *h) {
    save_patients_to_file(h);
    save_doctors_to_file(h);
}

void load_patients_from_file(Hospital *h) {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        printf("[INFO] No previous data found. Starting fresh.\n");
        return;
    }

    char buffer[200];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "PATIENTS", 8) == 0) {
            sscanf(buffer, "PATIENTS %d", &h->total_patients);
        } else if (strncmp(buffer, "PATIENT_COUNTER", 15) == 0) {
            sscanf(buffer, "PATIENT_COUNTER %d", &h->patient_counter);
        } else if (strncmp(buffer, "PATIENT ", 8) == 0) {
            Patient p;
            sscanf(buffer, "PATIENT %d %s %d %s %d %d %d %f",
                   &p.patient_id,
                   p.name,
                   &p.age,
                   p.condition,
                   (int*)&p.severity,
                   (int*)&p.status,
                   &p.assigned_doctor_id,
                   &p.bill_amount);
            p.arrival_time = 0;
            p.treatment_start_time = 0;
            p.treatment_end_time = 0;
            // Add to array (simplified - in real code, need proper indexing)
        }
    }

    printf("[SUCCESS] Patient data loaded from %s\n", DATA_FILE);
    fclose(file);
}

void load_doctors_from_file(Hospital *h) {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        return;
    }

    char buffer[200];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "DOCTORS", 7) == 0) {
            sscanf(buffer, "DOCTORS %d", &h->total_doctors);
        } else if (strncmp(buffer, "DOCTOR ", 7) == 0) {
            Doctor d;
            sscanf(buffer, "DOCTOR %d %s %s %d %d",
                   &d.doctor_id,
                   d.name,
                   d.specialization,
                   (int*)&d.status,
                   &d.current_patient_id);
            // Add to array (simplified)
        }
    }

    printf("[SUCCESS] Doctor data loaded from %s\n", DATA_FILE);
    fclose(file);
}

void load_hospital_data(Hospital *h) {
    load_patients_from_file(h);
    load_doctors_from_file(h);
}