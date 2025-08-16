#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"

void add_room() {
    FILE *fp = fopen("rooms.txt", "a");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    int n;
    printf("How many rooms do you want to add? ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        Room r;
        printf("\nEnter details for room %d:\n", i + 1);
        printf("Room Number: ");
        scanf("%d", &r.room_number);
        printf("Room Type (single/double): ");
        scanf("%s", r.type);
        printf("Rent (for one year): ");
        scanf("%f", &r.rent);

        if (strcmp(r.type, "double") == 0)
            r.available = 2;  // double room has 2 slots
        else
            r.available = 1;  // single room has 1 slot

        fwrite(&r, sizeof(Room), 1, fp);
        printf("Room added successfully.\n");
    }

    fclose(fp);
}

void modify_room() {
    int room_no;
    printf("Enter room number to modify: ");
    scanf("%d", &room_no);

    FILE *fp = fopen("rooms.txt", "r+");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    Room r;
    int found = 0;
    while (fread(&r, sizeof(Room), 1, fp)) {
        if (r.room_number == room_no) {
            printf("Enter new rent (for one year): ");
            scanf("%f", &r.rent);
            printf("Enter available slots (0-2): ");
            scanf("%d", &r.available);
            fseek(fp, -sizeof(Room), SEEK_CUR);
            fwrite(&r, sizeof(Room), 1, fp);
            printf("Room details updated.\n");
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Room not found.\n");
    }

    fclose(fp);
}

void delete_room() {
    int room_no;
    printf("Enter room number to delete: ");
    scanf("%d", &room_no);

    FILE *fp = fopen("rooms.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp) {
        printf("Error opening file.\n");
        return;
    }

    Room r;
    int found = 0;
    while (fread(&r, sizeof(Room), 1, fp)) {
        if (r.room_number != room_no) {
            fwrite(&r, sizeof(Room), 1, temp);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("rooms.txt");
    rename("temp.txt", "rooms.txt");

    if (found) {
        printf("Room deleted successfully.\n");
    } else {
        printf("Room not found.\n");
    }
}

void display_rooms() {
    FILE *fp = fopen("rooms.txt", "r");
    if (!fp) {
        printf("Error opening file.\n");
        return;
    }

    Room r;
    printf("\nRoom List:\n");
    printf("Room No. | Type   | Rent    | Status\n");
    while (fread(&r, sizeof(Room), 1, fp)) {
        printf("%-9d %-7s %-8.2f %s (%d slot%s left)\n", 
            r.room_number, 
            r.type, 
            r.rent, 
            r.available > 0 ? "Available" : "Booked",
            r.available,
            r.available == 1 ? "" : "s");
    }

    fclose(fp);
}

void admin_panel() {
    int choice;
    do {
        printf("\n--- Admin Panel ---\n");
        printf("1. Add Room\n");
        printf("2. Modify Room\n");
        printf("3. Delete Room\n");
        printf("4. Display All Rooms\n");
        printf("5. Exit to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_room(); break;
            case 2: modify_room(); break;
            case 3: delete_room(); break;
            case 4: display_rooms(); break;
            case 5: printf("Returning to main menu...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}
