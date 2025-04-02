#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "fine.h"
#include "book.h"

#define FINE_PER_DAY 10
#define CSV_FILE "fines.csv"

typedef struct {
    int bookId;
    char bookTitle[100];
    int overdueDays;
    int fineAmount;
} FineRecord;

FineRecord fines[100];
int fineCount = 0;

void getCurrentDate(char *buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

int calculateDaysBetween(char *startDate, char *endDate) {
    struct tm start = {0}, end = {0};

    
    if (sscanf(startDate, "%4d-%2d-%2d", &start.tm_year, &start.tm_mon, &start.tm_mday) != 3) {
        printf("Error parsing start date.\n");
        return -1; // Error code for date parsing failure
    }

    
    if (sscanf(endDate, "%4d-%2d-%2d", &end.tm_year, &end.tm_mon, &end.tm_mday) != 3) {
        printf("Error parsing end date.\n");
        return -1; // Error code for date parsing failure
    }

    start.tm_year -= 1900;
    end.tm_year -= 1900;
    start.tm_mon -= 1;
    end.tm_mon -= 1;

    
    time_t start_time = mktime(&start);
    time_t end_time = mktime(&end);

    
    double seconds_diff = difftime(end_time, start_time);
    return (int)(seconds_diff / (60 * 60 * 24));
}


void saveFinesToCSV() {
    FILE *file = fopen(CSV_FILE, "w");
    if (file == NULL) {
        printf("Error: Could not open fines.csv for writing.\n");
        return;
    }

    fprintf(file, "BookID,BookTitle,OverdueDays,FineAmount\n");

   
    for (int i = 0; i < fineCount; i++) {
        fprintf(file, "%d,%s,%d,%d\n",
                fines[i].bookId, fines[i].bookTitle, fines[i].overdueDays, fines[i].fineAmount);
    }

    fclose(file);
    printf("Fines saved to CSV.\n");
}

void loadFinesFromCSV() {
    FILE *file = fopen(CSV_FILE, "r");
    if (file == NULL) {
        printf("No fine data found.\n");
        return;
    }

    char line[256];
    fineCount = 0;

    fgets(line, sizeof(line), file);

    
    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d,%99[^,],%d,%d", 
               &fines[fineCount].bookId, fines[fineCount].bookTitle,
               &fines[fineCount].overdueDays, &fines[fineCount].fineAmount);
        fineCount++;
    }

    fclose(file);
    printf("Fines loaded from CSV.\n");
}


void calculateFineForBook() {
    char currentDate[11], dueDate[11];

    printf("Total Books having due date greater than 15 days:\n\n");
    int flag = 0;
    fineCount = 0;  
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isIssued) {
            strcpy(dueDate, books[i].dueDate);

            getCurrentDate(currentDate);

            int overdueDays = calculateDaysBetween(dueDate, currentDate);

            if (overdueDays >= 15) {
                flag = 1;
                int fine = overdueDays * FINE_PER_DAY; 

                fines[fineCount].bookId = books[i].id;
                strcpy(fines[fineCount].bookTitle, books[i].title);
                fines[fineCount].overdueDays = overdueDays;
                fines[fineCount].fineAmount = fine;
                fineCount++;

                printf("Book ID %d (Title: %s) is overdue by %d days. Fine is %d Rupees.\n", 
                       books[i].id, books[i].title, overdueDays, fine);
            }
        }
    }
    if (!flag) {
        printf("\nNo books have overdue date in our library.\n");
    }

    saveFinesToCSV();
}


void manageFineAndFees() {
    int option;
    printf("\nFine and Fee Management:\n");
    printf("1. Calculate Fine for Overdue Book\n");
    printf("2. View Overdue Books\n");
    printf("3. Exit to Main Menu\n");
    printf("Enter your option: ");
    scanf("%d", &option);

    switch (option) {
    case 1:
        calculateFineForBook();
        break;
    case 2:
        viewOverdueBooks();
        break;
    case 3:
        return; // Exit to main menu
    default:
        printf("Invalid choice! Returning to main menu.\n");
        return;
    }
}


void viewOverdueBooks() {
    char currentDate[11];
    getCurrentDate(currentDate);

    printf("List of overdue books:\n");
    int overdueBooksFound = 0;
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isIssued && calculateDaysBetween(books[i].dueDate, currentDate) > 15) {
            printf("Book ID: %d | Title: %s | Due Date: %s\n", 
                   books[i].id, books[i].title, books[i].dueDate);
            overdueBooksFound = 1;
        }
    }
    if (!overdueBooksFound) {
        printf("No overdue books found.\n");
    }
}
