#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"  
#include "fine.h"
#include "reservation.h" 
#include "feedback.h" 
#include "backupdata.h"

#define CREDENTIALS_FILE "admin.csv"

int authenticateAdmin() {
    char username[20], password[20], storedUsername[20], storedPassword[20];

    printf("Enter Admin Username: ");
    scanf("%s", username);
    printf("Enter Admin Password: ");
    scanf("%s", password);

    FILE *file = fopen(CREDENTIALS_FILE, "r");
    if (file == NULL) {
        perror("Error opening credentials file");
        exit(1);
    }

    while (fscanf(file, "%[^,],%s\n", storedUsername, storedPassword) != EOF) {
        if (strcmp(username, storedUsername) == 0 && strcmp(password, storedPassword) == 0) {
            printf("Admin authenticated successfully.\n");
            fclose(file);
            return 1;
        }
    }

    printf("Invalid username or password! Returning to role selection.\n");
    fclose(file);
    return 0;
}

void addAdmin() {
    char username[20], password[20];

    printf("Enter new Admin Username: ");
    scanf("%s", username);
    printf("Enter new Admin Password: ");
    scanf("%s", password);

    FILE *file = fopen(CREDENTIALS_FILE, "a");
    if (file == NULL) {
        perror("Error opening credentials file");
        exit(1);
    }

    fprintf(file, "%s,%s\n", username, password);
    fclose(file);

    printf("New admin added successfully.\n");
}

void adminmenu() {
    int choice;
    do {
        printf("\nAdmin Menu - Library Management System\n");
        printf("0. Add admin\n");
        printf("1. Add Book\n");
        printf("2. Delete Book\n");
        printf("3. Display Books\n");
        printf("4. Search Book\n");
        printf("5. Edit Book Details\n");
        printf("6. Issue Book\n");
        printf("7. View Issued Books\n");
        printf("8. Return Book\n");
        printf("9. Fine and Fee Management\n");
        printf("10. View Overdue Books\n");
        printf("11. Reserve a Book\n");
        printf("12. Cancel Reservation\n");
        printf("13. View Reserved Books\n");
        printf("14. Hold a Book\n");
        printf("15. Give Feedback\n");
        printf("16. Display Feedback\n");
        printf("17. Backup Data\n");
        printf("18. Exit to Role Selection\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0: addAdmin(); break;
            case 1: addBook(); break;
            case 2: deleteBook(); break;
            case 3: displayBooks(); break;
            case 4: searchBook(); break;
            case 5: editBookDetails(); break;
            case 6: issueBook(); break;
            case 7: viewIssuedBooks(); break;
            case 8: returnBook(); break;
            case 9: manageFineAndFees(); break;
            case 10: viewOverdueBooks(); break;
            case 11: reserveBook(); break;
            case 12: cancelReservation(); break;
            case 13: viewReservedBooks(); break;
            case 14: holdBook(); break;
            case 15: giveFeedback(); break;
            case 16: displayFeedback(); break;
            case 17: backupData(); break;
            case 18: 
                saveBooksToCSV();
                printf("Returning to role selection.\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 18);
}

void usermenu() {
    int choice;
    do {
        printf("\nUser Menu - Library Management System\n");
        printf("1. Search Book\n");
        printf("2. Display Books\n");
        printf("3. Return Book\n");
        printf("4. Reserve a Book\n");
        printf("5. Cancel Reservation\n");
        printf("6. Hold a Book\n");
        printf("7. Give Feedback\n");
        printf("8. Display Feedback\n");
        printf("9. Exit to Role Selection\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: searchBook(); break;
            case 2: displayBooks(); break;
            case 3: returnBook(); break;
            case 4: reserveBook(); break;
            case 5: cancelReservation(); break;
            case 6: holdBook(); break;
            case 7: giveFeedback();break;
            case 8: displayFeedback(); break;
            case 9: 
                printf("Returning to role selection.\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 9);
}

int main() {
    loadBooksFromCSV();
    loadReservationsFromCSV();

    int roleChoice;
    
    do {
        printf("\nWelcome to the Library Management System\n");
        printf("1. Admin\n");
        printf("2. User\n");
        printf("3. Exit\n");
        printf("Enter your role: ");
        scanf("%d", &roleChoice);

        switch (roleChoice) {
            case 1:
                if (authenticateAdmin()) {
                    adminmenu();
                }
                break;
            case 2:
                usermenu();
                break;
            case 3:
                printf("Exiting system...\n");
                saveBooksToCSV();
                saveReservationsToCSV();
                exit(0);
            default:
                printf("Invalid role selection! Please try again.\n");
        }
    } while (roleChoice != 3); 

    return 0;
}
