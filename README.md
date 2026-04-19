# Emergency Room Management System 

## Project Structure

```
ER-Management-System/
├── main.c              # Main program with entry point
├── emergency.h         # Header file with data structures and declarations
├── emergency.c         # Implementation of all functions
├── data.txt            # Persistent data file for hospital records       
└── README.md           # Documentation
```

## File Descriptions

### 1. **emergency.h** - Header File
Contains all data structure definitions and function declarations.

**Key Components:**
- **Macros**: MAX_PATIENTS, MAX_DOCTORS, MAX_NAME_LENGTH, etc.
- **Enums**: Severity, PatientStatus, DoctorStatus
- **Structures**: Patient, Doctor, Hospital
- **Function Prototypes**: All 20+ function declarations


### 2. **emergency.c** - Implementation File
Contains complete implementation of all functions.

**Sections:**
- Initialization (1 function)
- Utility Functions (3 functions)
- Patient Operations (3 functions)
- Doctor Operations (2 functions)
- Priority & Queue (2 functions)
- Treatment Operations (3 functions)
- Billing (2 functions)
- Reporting (1 function)
- Menu (1 function)
- Data Persistence (4 functions)


### 3. **main.c** - Main Program
Entry point of the application with the main() function.

**Functionality:**
- Initializes hospital
- Loads previous data
- Runs main menu loop
- Saves data on exit


### 4. **data.txt** - Data File
Persistent storage file for patient and doctor records.

**Format:**
```
PATIENTS [count]
PATIENT_COUNTER [next_id]
PATIENT [fields...]
DOCTOR [fields...]
```

---

## Data Structures 
Data Structure	            Used For

Array	                     Store patients & doctors
Structure	                 Define Patient, Doctor, Hospital
Linear Search	             Find patient/doctor
Priority Queue(logic)	     Emergency handling (severity)
Temporary Array	             Queue display
File Handling	             Data persistence

### Patient Structure
```c
typedef struct {
    int patient_id;              // Unique identifier (1000+)
    char name[50];               // Full name
    int age;                     // Age in years
    char condition[100];         // Medical condition description
    Severity severity;           // 1=Critical to 4=Mild
    PatientStatus status;        // Current status
    int assigned_doctor_id;      // -1 if none assigned
    float bill_amount;           // Generated bill in Rs.
    time_t arrival_time;         // Registration timestamp
    time_t treatment_start_time; // When treatment began
    time_t treatment_end_time;   // When treatment ended
} Patient;
```

### Doctor Structure
```c
typedef struct {
    int doctor_id;               // Unique identifier (1-20)
    char name[50];               // Full name
    char specialization[50];     // Medical specialization
    DoctorStatus status;         // Available/Busy/Off-duty
    int current_patient_id;      // -1 if not treating anyone
} Doctor;
```

### Hospital Structure
```c
typedef struct {
    Patient patients[100];       // Array of patients
    Doctor doctors[20];          // Array of doctors
    int total_patients;          // Current patient count
    int total_doctors;           // Current doctor count
    int patient_counter;         // Next patient ID to assign
} Hospital;
```

---

## Function Breakdown

### Initialization Functions
```c
void initialize_hospital(Hospital *h)
```
- Sets all counters to 0
- Called at program startup

### Patient Management Functions
```c
void register_patient(Hospital *h)
void display_patients(Hospital *h)
void search_patient(Hospital *h)
```

**register_patient()**: 
- Takes patient details as input
- Assigns unique ID
- Adds to hospital database
- Sets status to WAITING

**display_patients()**:
- Shows all registered patients
- Formatted table output
- Shows assigned doctor

**search_patient()**:
- Searches by patient ID
- Displays complete patient details
- Shows bill amount and status

### Doctor Management Functions
```c
void add_doctor(Hospital *h)
void display_doctors(Hospital *h)
```

**add_doctor()**:
- Registers new doctor
- Assigns unique ID
- Sets status to AVAILABLE

**display_doctors()**:
- Shows all registered doctors
- Displays specialization and status
- Shows current patient (if any)

### Treatment Functions
```c
void assign_doctor_to_patient(Hospital *h)
void start_treatment(Hospital *h)
void end_treatment(Hospital *h)
void calculate_bill(Patient *p)
```

**assign_doctor_to_patient()**:
- Finds highest priority waiting patient
- Allows selection of doctor
- Updates both records
- Changes doctor status to BUSY

**start_treatment()**:
- Marks patient status as IN_TREATMENT
- Records start time
- Requires doctor assignment

**end_treatment()**:
- Marks patient status as TREATED
- Records end time
- Calculates bill automatically
- Frees up doctor (status = AVAILABLE)

**calculate_bill()**:
- Base: Rs. 100
- Severity multiplier: 1x-5x
- Age factor: 1x-1.5x
- Treatment duration factor

### Queue & Priority Functions
```c
int get_highest_priority_patient(Hospital *h)
void display_patient_queue(Hospital *h)
```

**get_highest_priority_patient()**:
- Returns index of WAITING patient with lowest severity number
- Returns -1 if none found
- Used by auto-assignment

**display_patient_queue()**:
- Shows all WAITING patients
- Sorted by severity (highest to lowest priority)
- Helps visualize queue

### Reporting Functions
```c
void generate_report(Hospital *h)
```
- Total patients and doctors
- Patient status distribution
- Doctor status distribution
- Total revenue generated

### Utility Functions
```c
char* get_severity_string(Severity s)
char* get_status_string(PatientStatus s)
char* get_doctor_status_string(DoctorStatus s)
```
- Convert enum values to readable strings

### Data Persistence Functions
```c
void save_hospital_data(Hospital *h)
void load_hospital_data(Hospital *h)
void save_patients_to_file(Hospital *h)
void save_doctors_to_file(Hospital *h)
void load_patients_from_file(Hospital *h)
void load_doctors_from_file(Hospital *h)
```

**save_hospital_data()**:
- Saves all data to data.txt
- Called when exiting (option 11)

**load_hospital_data()**:
- Loads previous data on startup
- Reconstructs hospital state
- Silent if no data exists

### Menu Function
```c
void display_menu(void)
```
- Shows all 11 menu options
- Prompts for choice

---

## Compilation Instructions

1. gcc main.c Emergency.c -o main
2. main

---

## Data File Format (data.txt)

### Header Lines
```
PATIENTS 5                    # Number of patients in file
PATIENT_COUNTER 1005          # Next patient ID to assign
```

### Patient Records
```
PATIENT 1000 Rajesh 45 Chest_Pain 1 3 1 375.00
         │    │      │  │          │ │ │ │
         ID   Name   Age Condition  S P D Bill
         
S = Severity (1-4)
P = Patient Status (1-4)
D = Doctor ID (-1 if none)
```

### Doctor Records
```
DOCTOR 1 Dr_Singh Cardiology 1 -1
       │ │        │           │ │
       ID Name    Specialization D P
       
D = Doctor Status (1-3)
P = Current Patient ID (-1 if none)
```

---

## Program Flow

```
START
  │
  ├─► Initialize Hospital (empty)
  │
  ├─► Load Data from data.txt
  │
  └─► Enter Main Loop
       │
       ├─► Display Menu
       │
       ├─► Get User Choice
       │
       ├─► Execute Function
       │    ├─ Register Patient
       │    ├─ Add Doctor
       │    ├─ Display/Search
       │    ├─ Assign Doctor
       │    ├─ Start/End Treatment
       │    ├─ View Queue
       │    └─ Generate Report
       │
       └─► If Choice = 11
            ├─ Save Data to data.txt
            └─ Exit
```

---

## Billing Formula

```
Total Bill = Base Fee × Severity Multiplier × Age Factor × Treatment Hours

Where:
  Base Fee = Rs. 100
  
  Severity Multiplier:
    CRITICAL (1) = 5.0x
    SEVERE (2)   = 3.5x
    MODERATE (3) = 2.0x
    MILD (4)     = 1.0x
  
  Age Factor:
    > 60 years  = 1.5x
    > 40 years  = 1.2x
    ≤ 40 years  = 1.0x
  
  Treatment Hours:
    Minimum = 0.5 hours
    Actual = (end_time - start_time) / 3600 seconds
```

---

## Modularity Benefits

1. **Separation of Concerns**
   - Header defines interface
   - Implementation hidden in .c file
   - Main program is clean and simple

2. **Easier Maintenance**
   - Find functions in emergency.c
   - Change implementation without affecting main.c
   - Easy to debug specific modules

3. **Code Reusability**
   - Can use emergency.c in other projects
   - Just include emergency.h

4. **Scalability**
   - Easy to add new functions
   - Can split emergency.c further if needed
   - Extensions don't break existing code

5. **Team Development**
   - Different developers can work on different files
   - Merge changes easily
   - Clear interfaces and contracts

---

## Compilation Output Files

After successful compilation with Makefile:
```
main.o        # Compiled main.c
emergency.o   # Compiled emergency.c
er_system     # Final executable
```

---

## Data Persistence

### On Program Exit
- Option 11 calls `save_hospital_data()`
- All patients and doctors written to data.txt
- Records current patient counter
- File is human-readable text format

### On Program Startup
- `load_hospital_data()` called automatically
- Reads data.txt if it exists
- Reconstructs hospital state
- Patient IDs continue from where they left off

### File Safety
- Original data.txt backed up before writing
- Use `make clean-data` to reset data
- Text format allows manual editing if needed

---

## Testing the System

### Quick Test
```bash
make run
# Register a patient
# Add a doctor
# Assign doctor
# End treatment
# View report
# Save & exit
```

### Persistence Test
```bash
make run
# Add patient: "Test Patient"
# Exit (option 11)

make run
# Search for "Test Patient" - should exist
# Verify data persisted
```

### Clean Rebuild
```bash
make distclean  # Removes everything
make            # Fresh build
make run        # Start with clean data
```

---

## Advantages of Modular Structure

✅ **Clear Organization**: Each file has specific purpose
✅ **Easy Compilation**: Use Makefile for consistent builds
✅ **Data Persistence**: Save/load system state
✅ **Maintainability**: Change functions without affecting main
✅ **Readability**: Shorter files, easier to understand
✅ **Extensibility**: Add new features easily
✅ **Professional**: Industry-standard project structure

---

## Error Handling

The system handles:
- ✅ Hospital at capacity
- ✅ Invalid patient/doctor IDs
- ✅ Unavailable doctors
- ✅ Invalid menu choices
- ✅ Treatment without doctor assignment
- ✅ File I/O errors

---

## Enum Values Reference

### Severity
```c
CRITICAL = 1   (Highest priority)
SEVERE = 2
MODERATE = 3
MILD = 4       (Lowest priority)
```

### PatientStatus
```c
WAITING = 1
IN_TREATMENT = 2
TREATED = 3
DISCHARGED = 4
```

### DoctorStatus
```c
AVAILABLE = 1
BUSY = 2
OFF_DUTY = 3
```

---

## Future Enhancements

1. **Database Integration**: Replace text file with SQLite
2. **Multiple Files**: Split emergency.c into specialized modules
3. **Advanced Queue**: Implement priority queue data structure
4. **Logging**: Add audit trail for all operations
5. **Backup System**: Automatic data backups
6. **Encryption**: Secure patient data
7. **GUI**: Graphical user interface
8. **Networking**: Multi-user support
9. **Reports**: PDF/Excel export
10. **Analytics**: Statistical analysis and trends

---

## Summary

This modular structure provides:
- Professional C project organization
- Clean separation of interface and implementation
- Persistent data storage
- Easy compilation and maintenance
- Foundation for future enhancements


# Architecture & Diagrams

## 📐 Project Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     main.c (Entry Point)                     │
│                                                              │
│  • Initialize hospital                                      │
│  • Load data from file                                      │
│  • Main menu loop                                           │
│  • Save data on exit                                        │
└────────────────────┬────────────────────────────────────────┘
                     │
                     ▼
         ┌───────────────────────┐
         │   emergency.h         │
         │  (Interface/Header)   │
         │                       │
         │ • Data Structures     │
         │ • Type Definitions    │
         │ • Function Prototypes │
         └───────────┬───────────┘
                     │
                     ▼
         ┌───────────────────────┐
         │  emergency.c          │
         │  (Implementation)     │
         │                       │
         │                       │ 
         |                       │
         └───────────┬───────────┘
                     │
                     ▼
         ┌───────────────────────┐
         │    data.txt           │
         │  (Persistent Data)    │
         │                       │
         │ • Patient Records     │
         │ • Doctor Records      │
         └───────────────────────┘
```

## 🏗️ Module Organization

```
emergency.c()
├── Initialization() 
│   └── initialize_hospital()
│
├── Utility Functions ()
│   ├── get_severity_string()
│   ├── get_status_string()
│   └── get_doctor_status_string()
│
├── Patient Operations ()
│   ├── register_patient()
│   ├── display_patients()
│   └── search_patient()
│
├── Doctor Operations ()
│   ├── add_doctor()
│   └── display_doctors()
│
├── Priority & Queue ()
│   ├── get_highest_priority_patient()
│   └── display_patient_queue()
│
├── Treatment Operations ()
│   ├── assign_doctor_to_patient()
│   ├── start_treatment()
│   └── end_treatment()
│
├── Billing Operations ()
│   └── calculate_bill()
│
├── Reporting ()
│   └── generate_report()
│
├── Menu ()
│   └── display_menu()
│
└── Data Persistence ()
    ├── save_hospital_data()
    ├── load_hospital_data()
    ├── save_patients_to_file()
    ├── save_doctors_to_file()
    ├── load_patients_from_file()
    └── load_doctors_from_file()
```

## 🔄 Program Flow Diagram

```
START
  │
  ├─── Initialize Hospital
  │     ├─ total_patients = 0
  │     ├─ total_doctors = 0
  │     └─ patient_counter = 1000
  │
  ├─── Load Previous Data
  │     ├─ Try to open data.txt
  │     ├─ If found:
  │     │  └─ Restore patients & doctors
  │     └─ If not found:
  │        └─ Start with empty hospital
  │
  ├─── MAIN LOOP ──────┐
  │    Display Menu     │
  │                    │
  ├─── Get User Input  │
  │                    │
  └─── Switch (Choice)─┤
       ├─ 1: Register Patient ───┐
       │                         │
       ├─ 2: Add Doctor ─────────┤
       │                         │
       ├─ 3: Display Patients ──┤
       │                         │
       ├─ 4: Display Doctors ───┤
       │                         │
       ├─ 5: Assign Doctor ─────┤
       │                         │
       ├─ 6: Start Treatment ───┤
       │                         │
       ├─ 7: End Treatment ─────┤
       │                         │
       ├─ 8: View Queue ────────┤
       │                         │
       ├─ 9: Search Patient ────┤
       │                         │
       ├─ 10: Report ──────────┤
       │                         │
       ├─ 11: Save & Exit ──────┤
       │     ├─ Save data       │
       │     ├─ Close file      │
       │     └─ Return (END)    │
       │                         │
       └─── Go to MAIN LOOP ────┘
```

## 📊 Data Structure Relationships

```
┌──────────────────────────────────────┐
│           Hospital                   │
├──────────────────────────────────────┤
│ • Patient[] array (100 slots)        │
│ • Doctor[] array (20 slots)          │
│ • total_patients (count)             │
│ • total_doctors (count)              │
│ • patient_counter (next ID)          │
└──────┬──────────────────────┬────────┘
       │                      │
       ▼                      ▼
   ┌────────────────┐    ┌────────────────┐
   │    Patient     │    │     Doctor     │
   ├────────────────┤    ├────────────────┤
   │ • patient_id   │    │ • doctor_id    │
   │ • name         │    │ • name         │
   │ • age          │    │ • specialization
   │ • condition    │    │ • status       │
   │ • severity     │    │ • current_      │
   │ • status       │    │   patient_id   │
   │ • assigned_    │    └────────────────┘
   │   doctor_id ◄──────┐
   │ • bill_amount  │    │
   │ • timestamps   │    │
   └────────────────┘    │
        │                │
        └────────────────┘
        (Doctor-Patient
         Relationship)
```

## 🔀 Treatment State Machine

```
Patient Registration
        │
        ▼
    ┌────────┐
    │ WAITING│ ◄────────────────────┐
    └───┬────┘                      │
        │                           │
        │ (Assign Doctor)          │
        ▼                          │
    ┌─────────────┐              │
    │IN_TREATMENT │              │
    └─────┬───────┘              │
          │                      │
          │ (End Treatment)      │
          ▼                      │
    ┌────────┐            │ (Re-assign)
    │ TREATED│            │
    └─────┬──┘            │
          │               │
          │ (Discharge)   │
          ▼               │
    ┌──────────┐          │
    │DISCHARGED├──────────┘
    └──────────┘

Key Points:
• Patient starts in WAITING
• Can only proceed if doctor assigned
• Treatment records start & end time
• Bill calculated when treatment ends
• Discharged patients are complete
```

## 🎯 Billing Calculation Flow

```
End Treatment Called
        │
        ├─ Patient found? ──NO──> ERROR
        │
        └─ YES ──┐
                 ▼
        ┌──────────────────┐
        │ Calculate Bill   │
        └──────────────────┘
                 │
                 ├─ Get Base Fee (100)
                 │
                 ├─ Determine Severity Multiplier
                 │   ├─ CRITICAL → 5.0x
                 │   ├─ SEVERE   → 3.5x
                 │   ├─ MODERATE → 2.0x
                 │   └─ MILD     → 1.0x
                 │
                 ├─ Determine Age Factor
                 │   ├─ >60 years → 1.5x
                 │   ├─ >40 years → 1.2x
                 │   └─ ≤40 years → 1.0x
                 │
                 ├─ Calculate Treatment Hours
                 │   ├─ (end_time - start_time)/3600
                 │   └─ Minimum: 0.5 hours
                 │
                 ▼
        ┌──────────────────────────────────┐
        │ Bill = Base × Severity ×         │
        │        Age × Hours               │
        └──────────────────────────────────┘
                 │
                 ▼
        ┌──────────────────┐
        │ Store bill in    │
        │ patient.bill_    │
        │ amount           │
        └──────────────────┘
```

## 👥 Doctor Assignment Logic

```
Assignment Request
        │
        ▼
    ┌────────────────────────────────────┐
    │ Find Highest Priority Waiting      │
    │ Patient (Lowest Severity Number)   │
    └─────────────────┬──────────────────┘
                      │
        ┌─────────────┴─────────────┐
        │                           │
       NO                          YES
    (None waiting)            (Patient found)
        │                           │
        ▼                           ▼
     ERROR             ┌──────────────────────┐
                       │ Show patient details │
                       └──────────┬───────────┘
                                  │
                    ┌─────────────▼──────────┐
                    │ Show available doctors │
                    └──────────┬─────────────┘
                               │
                    ┌──────────▼──────────┐
                    │ Get doctor ID input │
                    └──────────┬──────────┘
                               │
               ┌───────────────┴───────────────┐
               │                               │
           Invalid ID                    Valid ID
               │                               │
               ▼                               ▼
            ERROR              ┌──────────────────────┐
                               │ Check doctor status  │
                               └──────────┬───────────┘
                                          │
                          ┌───────────────┴──────────────┐
                          │                              │
                      NOT AVAILABLE                 AVAILABLE
                          │                              │
                          ▼                              ▼
                       ERROR                    ┌────────────────┐
                                                │ Assign Doctor  │
                                                │ • Update patient
                                                │   record       │
                                                │ • Mark doctor  │
                                                │   as BUSY      │
                                                │ • Set current  │
                                                │   patient ID   │
                                                └────────────────┘
                                                        │
                                                        ▼
                                                    SUCCESS
```

## 📋 Patient Queue Priority Ordering

```
Input: All WAITING patients
        │
        ▼
    ┌─────────────────────────┐
    │ Collect all WAITING     │
    │ patients into array     │
    └──────────┬──────────────┘
               │
        ┌──────▼──────┐
        │ Sort by     │
        │ Severity    │
        │ (ascending) │
        └──────┬──────┘
               │
        ┌──────▼──────────────────────┐
        │ Display in order:           │
        │ 1. All CRITICAL (severity1) │
        │ 2. All SEVERE   (severity2) │
        │ 3. All MODERATE (severity3) │
        │ 4. All MILD     (severity4) │
        └─────────────────────────────┘

Example:
Input patients: P3(MILD), P1(CRITICAL), P2(SEVERE)
After sorting: P1(CRITICAL), P2(SEVERE), P3(MILD)
```

## 💾 Data File Format

```
data.txt
│
├─ Header Section
│  ├─ PATIENTS 5         (number of patients)
│  └─ PATIENT_COUNTER 1005 (next ID to use)
│
├─ Patient Records
│  ├─ PATIENT 1000 Rajesh 45 Chest_Pain 1 3 1 375.00
│  ├─ PATIENT 1001 Priya 28 Fracture 2 3 2 245.00
│  ├─ PATIENT 1002 Amit 52 Diabetes 3 3 3 120.00
│  ├─ PATIENT 1003 Sanjana 35 Cut 4 3 -1 50.00
│  └─ PATIENT 1004 Vikram 67 Heart 1 3 1 562.50
│
├─ Doctor Header
│  └─ DOCTORS 3         (number of doctors)
│
└─ Doctor Records
   ├─ DOCTOR 1 Dr_Singh Cardiology 1 -1
   ├─ DOCTOR 2 Dr_Sharma Orthopedics 1 -1
   └─ DOCTOR 3 Dr_Patel General_Medicine 1 -1

Field Explanations:
Patients: ID Name Age Condition Severity Status DoctorID Bill
Doctors: ID Name Specialization Status CurrentPatientID
```

## 🔗 Function Call Hierarchy

```
main()
├── initialize_hospital()
├── load_hospital_data()
│   ├── load_patients_from_file()
│   └── load_doctors_from_file()
│
└── Main Loop (while(1))
    ├── display_menu()
    │
    └── switch(choice)
        ├── 1: register_patient()
        │
        ├── 2: add_doctor()
        │
        ├── 3: display_patients()
        │
        ├── 4: display_doctors()
        │
        ├── 5: assign_doctor_to_patient()
        │     └── get_highest_priority_patient()
        │
        ├── 6: start_treatment()
        │
        ├── 7: end_treatment()
        │     └── calculate_bill()
        │
        ├── 8: display_patient_queue()
        │
        ├── 9: search_patient()
        │
        ├── 10: generate_report()
        │
        └── 11: save_hospital_data()
              ├── save_patients_to_file()
              └── save_doctors_to_file()
```

## 🎛️ Menu Decision Tree

```
Main Menu
│
├─ 1: Patient Registration
│  └─ Input: Name, Age, Condition, Severity
│     Output: Unique ID (1000+)
│
├─ 2: Doctor Registration
│  └─ Input: Name, Specialization
│     Output: Unique ID (1-20)
│
├─ 3: Show All Patients
│  └─ Output: Formatted table of all patients
│
├─ 4: Show All Doctors
│  └─ Output: Formatted table of all doctors
│
├─ 5: Assign Doctor to Patient
│  └─ Process:
│     1. Find highest priority patient
│     2. Show available doctors
│     3. User selects doctor
│     4. Create assignment
│
├─ 6: Start Treatment
│  ├─ Input: Patient ID
│  └─ Action: Mark as IN_TREATMENT, record start time
│
├─ 7: End Treatment
│  ├─ Input: Patient ID
│  └─ Actions:
│     1. Mark as TREATED
│     2. Record end time
│     3. Calculate bill
│     4. Free up doctor
│
├─ 8: View Patient Queue
│  └─ Output: Waiting patients sorted by priority
│
├─ 9: Search Patient
│  ├─ Input: Patient ID
│  └─ Output: Complete patient details
│
├─ 10: Hospital Report
│  └─ Output: Statistics (patients, doctors, revenue)
│
└─ 11: Save & Exit
   ├─ Actions:
   │  1. Save all data to file
   │  2. Close files
   │  3. Exit program
   └─ Output: Completion message
```

## 📈 Hospital State Over Time

```
Session 1:
  Initialize → Add 2 Doctors → Register 3 Patients → Exit

data.txt after Session 1:
┌────────────────────┐
│ PATIENTS 3         │
│ PATIENT_COUNTER ... │
│ PATIENT ...        │
│ PATIENT ...        │
│ PATIENT ...        │
│ DOCTORS 2          │
│ DOCTOR ...         │
│ DOCTOR ...         │
└────────────────────┘

Session 2:
  Load data → Register 2 more patients → Exit

data.txt after Session 2:
┌────────────────────┐
│ PATIENTS 5         │ ← Updated count
│ PATIENT_COUNTER ... │ ← Updated
│ PATIENT ... (old)  │
│ PATIENT ... (old)  │
│ PATIENT ... (old)  │
│ PATIENT ... (new)  │
│ PATIENT ... (new)  │
│ DOCTORS 2          │
│ DOCTOR ...         │
│ DOCTOR ...         │
└────────────────────┘
```

## 🔐 Memory Layout

```
Stack (Program Execution)
┌────────────────────┐
│ main() function    │
├────────────────────┤
│ Hospital struct    │ ← Points to heap arrays
│ (100 bytes)        │
├────────────────────┤
│ Local variables    │
└────────────────────┘
         │
         ▼
Heap (Dynamically Allocated)
┌────────────────────┐
│ Patient array[100] │ ← Each ~200 bytes
│ (20 KB)            │
├────────────────────┤
│ Doctor array[20]   │ ← Each ~150 bytes
│ (3 KB)             │
└────────────────────┘

Total Memory Usage: ~25 KB (at full capacity)
```

## ⚡ Performance Characteristics

```
Operation              Time Complexity    Space Complexity
──────────────────────────────────────────────────────────
Register Patient          O(1)            O(1)
Add Doctor               O(1)            O(1)
Display All Patients     O(n)            O(1)
Display All Doctors      O(m)            O(1)
Search Patient           O(n)            O(1)
Find Highest Priority    O(n)            O(1)
Sort Queue              O(n log n)      O(n)
Calculate Bill          O(1)            O(1)
Generate Report         O(n + m)        O(1)
Save to File           O(n + m)        O(1)
Load from File         O(n + m)        O(n + m)

Where: n = number of patients, m = number of doctors
```

---

These diagrams provide a comprehensive visual understanding of the system architecture and operation.