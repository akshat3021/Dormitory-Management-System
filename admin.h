#ifndef ADMIN_H
#define ADMIN_H

typedef struct {
    int room_number;
    char type[10];
    float rent;
    int available;  // 1 for single room, 2 for double room slots available
} Room;

typedef struct {
    char name[50];
    int id;
    int room_number;
    int duration;      // always 12 months (1 year)
    char date[15];     // booking date DD-MM-YYYY
} Student;

// Admin panel functions
void add_room();
void modify_room();
void delete_room();
void display_rooms();
void admin_panel();

// Student panel functions
void book_room();
void cancel_booking();
void display_student_bookings();
void student_panel();

// Invoice function
void generate_invoice();

#endif
