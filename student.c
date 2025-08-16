#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"

void book_room() {
    FILE *fp = fopen("rooms.txt", "r+");
    if (!fp) {
        printf("Could not open rooms file.\n");
        return;
    }

    Room r;
    int found = 0;
    char type_pref[10];
    printf("Enter preferred room type (single/double): ");
    scanf("%s", type_pref);

    printf("\nAvailable Rooms:\n");
    rewind(fp);
    while (fread(&r, sizeof(Room), 1, fp)) {
        if (r.available > 0 && strcmp(r.type, type_pref) == 0) {
            printf("Room %d | Type: %s | Rent: %.2f | Slots left: %d\n", r.room_number, r.type, r.rent, r.available);
            found = 1;
        }
    }

    if (!found) {
        printf("No available rooms of selected type.\n");
        fclose(fp);
        return;
    }

    int chosen;
    char confirm;
    printf("\nEnter room number to book: ");
    scanf("%d", &chosen);

    printf("Are you sure you want to book this room? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("Booking cancelled.\n");
        fclose(fp);
        return;
    }

    rewind(fp);
    while (fread(&r, sizeof(Room), 1, fp)) {
        if (r.room_number == chosen && r.available > 0) {
            r.available--;
            fseek(fp, -sizeof(Room), SEEK_CUR);
            fwrite(&r, sizeof(Room), 1, fp);
            break;
        }
    }
    fclose(fp);

    Student s;
    printf("Enter Your Name: ");
    scanf(" %[^\n]", s.name);  // Corrected: read full line with spaces
    printf("Enter Your ID: ");
    scanf("%d", &s.id);
    printf("Enter Booking Date (DD-MM-YYYY): ");
    scanf("%s", s.date);
    s.room_number = chosen;
    s.duration = 12;  // fixed 1 year

    FILE *sf = fopen("students.txt", "a");
    if (!sf) {
        printf("Error opening student file.\n");
        return;
    }
    fwrite(&s, sizeof(Student), 1, sf);
    fclose(sf);

    printf("Room booked successfully for 1 year.\n");
}

void cancel_booking() {
    int id;
    printf("Enter your ID to cancel booking: ");
    scanf("%d", &id);

    char cancel_date[15];
    printf("Enter cancellation date (DD-MM-YYYY): ");
    scanf("%s", cancel_date);

    FILE *sf = fopen("students.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!sf || !temp) {
        printf("Error opening file.\n");
        if (sf) fclose(sf);
        if (temp) fclose(temp);
        return;
    }

    int found = 0;
    Student s;

    while (fread(&s, sizeof(Student), 1, sf)) {
        if (s.id == id) {
            found = 1;

            int days_passed;
            printf("How many days have passed since booking? ");
            scanf("%d", &days_passed);

            if (days_passed <= 10) {
                printf("Booking cancelled. You will get 100%% refund.\n");
            } else {
                printf("Booking cancelled. You will get 70%% refund.\n");
            }

            FILE *rf = fopen("rooms.txt", "r+");
            if (rf) {
                Room r;
                while (fread(&r, sizeof(Room), 1, rf)) {
                    if (r.room_number == s.room_number) {
                        r.available++;
                        fseek(rf, -sizeof(Room), SEEK_CUR);
                        fwrite(&r, sizeof(Room), 1, rf);
                        break;
                    }
                }
                fclose(rf);
            }
        } else {
            fwrite(&s, sizeof(Student), 1, temp);
        }
    }

    fclose(sf);
    fclose(temp);

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (!found) {
        printf("Student ID not found.\n");
    }
}

void display_student_bookings() {
    int search_id;
    printf("Enter your Student ID to view booking: ");
    scanf("%d", &search_id);

    FILE *sf = fopen("students.txt", "r");
    if (!sf) {
        printf("Error opening student file.\n");
        return;
    }

    Student s;
    int found = 0;
    printf("\nStudent Bookings:\n");
    printf("Name        | ID   | Room | Booking Date\n");
    printf("----------------------------------------\n");

    while (fread(&s, sizeof(Student), 1, sf)) {
        if (s.id == search_id) {
            printf("%-12s %-5d %-5d %-12s\n", s.name, s.id, s.room_number, s.date);
            found = 1;
        }
    }

    if (!found) {
        printf("No booking found for this ID.\n");
    }

    fclose(sf);
}

void student_panel() {
    int choice;
    do {
        printf("\n--- Student Panel ---\n");
        printf("1. Book Room\n");
        printf("2. Cancel Booking\n");
        printf("3. View My Bookings\n");
        printf("4. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: book_room(); break;
            case 2: cancel_booking(); break;
            case 3: display_student_bookings(); break;
            case 4: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}
