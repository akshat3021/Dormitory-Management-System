#include <stdio.h>
#include "admin.h"

int main() {
    int choice;
    do {
        printf("\n--- Main Menu ---\n");
        printf("1. Admin Panel\n");
        printf("2. Student Panel\n");
        printf("3. Generate Invoice\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                int password;
                printf("Enter admin password: ");
                scanf("%d", &password);
                if (password == 7302) {
                    admin_panel();
                } else {
                    printf("Incorrect password. Access denied.\n");
                }
                break;
            }
            case 2: student_panel(); break;
            case 3: generate_invoice(); break;
            case 4: printf("Exiting...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);

    return 0;
}
