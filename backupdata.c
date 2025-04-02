#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h" 
#include "backupdata.h"
#include "reservation.h"

void backupData() {
    FILE *backupFile = fopen("backup_books.csv", "w");
    if (backupFile == NULL) {
        printf("Error: Could not open backup file!\n");
        return;
    }

    fprintf(backupFile, "ID,Title,Author,IsIssued,DueDate,ReservedBy\n");


    for (int i = 0; i < bookCount; i++) {
        fprintf(backupFile, "%d,%s,%s,%d,%s,%s\n",
                books[i].id,
                books[i].title,
                books[i].author,
                books[i].isIssued,
                books[i].dueDate[0] != '\0' ? books[i].dueDate : "N/A",
                reservations[i].reservedBy[0] != '\0' ? reservations[i].reservedBy : "N/A"
                );
    }

    fclose(backupFile);
    printf("Data successfully backed up to 'backup_books.csv'.\n");
}
