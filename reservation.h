#ifndef RESERVATION_H
#define RESERVATION_H

typedef struct {
    int bookId;
    char reservedBy[50];
} Reservation;

extern Reservation reservations[100];
extern int reservationCount;

void reserveBook();
void holdBook();
void cancelReservation();
void viewReservedBooks();
void loadReservationsFromCSV();  
void saveReservationsToCSV();    
#endif
