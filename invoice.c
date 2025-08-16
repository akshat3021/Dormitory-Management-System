#include <stdio.h>
#include <stdlib.h>
#include "admin.h"

void generate_invoice() {
    int id;
    printf("Enter student ID to generate invoice: ");
    scanf("%d", &id);

    FILE *sf = fopen("students.txt", "r");
    if (!sf) {
        printf("Cannot open student file.\n");
        return;
    }

    Student s;
    int found = 0;

    while (fread(&s, sizeof(Student), 1, sf)) {
        if (s.id == id) {
            found = 1;
            break;
        }
    }
    fclose(sf);

    if (!found) {
        printf("Student not found.\n");
        return;
    }

    FILE *rf = fopen("rooms.txt", "r");
    if (!rf) {
        printf("Cannot open rooms file.\n");
        return;
    }

    Room r;
    found = 0;
    while (fread(&r, sizeof(Room), 1, rf)) {
        if (r.room_number == s.room_number) {
            found = 1;
            break;
        }
    }
    fclose(rf);

    if (!found) {
        printf("Room not found.\n");
        return;
    }

    printf("\n------- Invoice -------\n");
    printf("Student Name : %s\n", s.name);
    printf("Student ID   : %d\n", s.id);
    printf("Room Number  : %d\n", s.room_number);
    printf("Room Type    : %s\n", r.type);
    printf("Booking Date : %s\n", s.date);
    printf("Duration     : %d months\n", s.duration);
    printf("Rent (1 Year): %.2f\n", r.rent);
    printf("-----------------------\n");
}
