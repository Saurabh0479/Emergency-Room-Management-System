#include "emergency.h"

int main() {
    Hospital hospital;
    initialize_hospital(&hospital);

    // Load previous data if exists
    load_hospital_data(&hospital);

    int choice;
    printf("\n**** Welcome to Emergency Room Management System ****\n");
    printf("***** Modular Version with Data Persistence *****\n\n");

    while (1) {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                register_patient(&hospital);
                break;

            case 2:
                add_doctor(&hospital);
                break;

            case 3:
                display_patients(&hospital);
                break;

            case 4:
                display_doctors(&hospital);
                break;

            case 5:
                assign_doctor_to_patient(&hospital);
                break;

            case 6:
                start_treatment(&hospital);
                break;

            case 7:
                end_treatment(&hospital);
                break;

            case 8:
                display_patient_queue(&hospital);
                break;

            case 9:
                search_patient(&hospital);
                break;

            case 10:
                generate_report(&hospital);
                break;

            case 11:
                printf("\n[INFO] Saving data...\n");
                save_hospital_data(&hospital);
                printf("[INFO] Shutting down ER Management System. Goodbye!\n");
                return 0;

            default:
                printf("\n[ERROR] Invalid choice! Please try again.\n");
        }
    }

    return 0;
}