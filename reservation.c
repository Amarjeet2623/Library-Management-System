#include <stdio.h>
#include <string.h>
#include "book.h"
#include "reservation.h"

Reservation reservations[100];
int reservationCount = 0; 

#define FILE_NAME "reservations.csv"

void saveReservationsToCSV() {
    FILE *file = fopen("reservations.csv", "w");

    if (file == NULL) {
        printf("Error: Could not open reservations file for writing.\n");
        return;
    }

    fprintf(file, "BookID,ReservedBy\n");

    for (int i = 0; i < reservationCount; i++) {
        if (reservations[i].bookId != 0) { 
            fprintf(file, "%d,%s\n", reservations[i].bookId, reservations[i].reservedBy);
        }
    }

    fclose(file);
    printf("Reservations saved successfully.\n");
}

void loadReservationsFromCSV() {
    FILE *file = fopen("reservations.csv", "r");

    if (file == NULL) {
        printf("No reservations file found. Starting with an empty reservation list.\n");
        return;
    }

    char line[100];
    reservationCount = 0;

    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%49s", &reservations[reservationCount].bookId, reservations[reservationCount].reservedBy);
        reservationCount++;
    }

    fclose(file);
    printf("Reservations loaded successfully.\n");
}

void reserveBook() {
    int id;
    char user[50];
    int check = 0;

    printf("Enter the Book ID to reserve: ");
    scanf("%d", &id);

    if (id > 0) {
        for (int i = 0; i < bookCount; i++) {
            if (books[i].isIssued && books[i].id == id) {
                printf("Enter your name to reserve the book: ");
                scanf("%s", user);

                reservations[reservationCount].bookId = id;
                strcpy(reservations[reservationCount].reservedBy, user);
                reservationCount++;

                saveReservationsToCSV();
                printf("Book with ID %d has been reserved by %s.\n", id, user);
                check = 1;
                break;
            }
        }
        if (check == 0) {
            printf("The book is available and doesn't need to be reserved.\n");
        }
    } else {
        printf("Invalid book ID!\n");
    }
}

void holdBook() {
    int id;
    printf("Enter the Book ID that is being held: ");
    scanf("%d", &id);

    for (int i = 0; i < reservationCount; i++) {
        if (reservations[i].bookId == id) {
            printf("Book with ID %d is now on hold for %s.\n", id, reservations[i].reservedBy);
            return;
        }
    }
    printf("No reservations found for this book.\n");
}

void cancelReservation() {
    int id;
    printf("Enter the ID of the reserved book to cancel: ");
    scanf("%d", &id);

    for (int i = 0; i < reservationCount; i++) {
        if (reservations[i].bookId == id) {
            reservations[i].bookId = 0; 
            strcpy(reservations[i].reservedBy, "");

            saveReservationsToCSV();
            printf("Reservation for Book ID %d has been cancelled.\n", id);
            return;
        }
    }
    printf("No reservation found for this book.\n");
}

// Function to view all reserved books
void viewReservedBooks() {
    int reservedBooksFound = 0;

    printf("List of all reserved books:\n");
    for (int i = 0; i < reservationCount; i++) {
        if (reservations[i].bookId != 0) { 
            printf("Book ID: %d | Reserved By: %s\n", reservations[i].bookId, reservations[i].reservedBy);
            reservedBooksFound = 1;
        }
    }

    if (!reservedBooksFound) {
        printf("No books are currently reserved.\n");
    }
}
