#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "book.h"
#include "utils.h"

#define FILE_NAME "library.csv"

Book books[100]; 
int bookCount = 0; 

void addBook()
{
    Book newBook;

    printf("Enter book ID: ");
    scanf("%d",&newBook.id);

    printf("Enter book title: ");
    getchar();
    fgets(newBook.title, sizeof(newBook.title), stdin);
    newBook.title[strcspn(newBook.title, "\n")] = 0;

    printf("Enter author name: ");
    fgets(newBook.author, sizeof(newBook.author), stdin);
    newBook.author[strcspn(newBook.author, "\n")] = 0;

    newBook.isIssued = 0;

    books[bookCount++] = newBook;

    saveBooksToCSV();
    printf("Book added successfully!\n");
}

void deleteBook()
{
    int id;
    printf("Enter book ID to delete: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == id)
        {
            found = 1;
            for (int j = i; j < bookCount - 1; j++)
            {
                books[j] = books[j + 1];
            }
            bookCount--;
            printf("Book deleted successfully!\n");
            break;
        }
    }

    if (!found)
    {
        printf("Book not found!\n");
    }

    saveBooksToCSV();
}


void searchBook()
{
    char term[50];
    printf("Enter book title or author name to search: ");
    getchar();
    fgets(term, sizeof(term), stdin);
    term[strcspn(term, "\n")] = 0;

    for (int i = 0; i < bookCount; i++)
    {
        if (strstr(books[i].title, term) != NULL || strstr(books[i].author, term) != NULL)
        {
            printf("ID: %d\nTitle: %s\nAuthor: %s\nStatus: %s\n", books[i].id, books[i].title, books[i].author, books[i].isIssued ? "Issued" : "Available");
        }
    }
}


void displayBooks()
{
    if (bookCount == 0)
    {
        printf("No books available in the library!\n");
        return;
    }

    for (int i = 0; i < bookCount; i++)
    {
        printf("ID: %d\nTitle: %s\nAuthor: %s\nStatus: %s\n", books[i].id, books[i].title, books[i].author, books[i].isIssued ? "Issued" : "Available");
    }
}


void editBookDetails() {
    int id, choice;
    printf("Enter the ID of the book to edit: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++) {
        if (books[i].id == id) {
            printf("Editing Book: %s\n", books[i].title);
            printf("1. Edit Title\n");
            printf("2. Edit Author\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch (choice) {
                case 1:
                    printf("Enter new title: ");
                    scanf(" %[^\n]", books[i].title);
                    break;
                case 2:
                    printf("Enter new author: ");
                    scanf(" %[^\n]", books[i].author);
                    break;
                default:
                    printf("Invalid choice!\n");
            }
            saveBooksToCSV();
            printf("Book details updated successfully!\n");
            return;
        }
    }
    printf("Book not found!\n");
}


void issueBook()
{
    int id;
    printf("Enter book ID to issue: ");
    scanf("%d", &id);

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == id)
        {
            if (!books[i].isIssued)
            {
                books[i].isIssued = 1;
                char date[11];

                printf("Enter the issue date (YYYY-MM-DD): ");
                scanf("%10s", date);

                printf("You entered: %s\n", date);
                strcpy(books[i].dueDate, date);
                printf("book issud date is %s ", books[i].dueDate);
                saveBooksToCSV();
                printf("Book issued successfully!\n");
            }
            else
            {
                printf("Book is already issued!\n");
            }
            return;
        }
    }
    printf("Book not found!\n");
}


void viewIssuedBooks() {
    int issuedBooksFound = 0;
    printf("List of all issued books:\n");
    for (int i = 0; i < bookCount; i++) {
        if (books[i].isIssued) {
            printf("Book ID: %d | Title: %s | Due Date: %s\n",
                   books[i].id, books[i].title, books[i].dueDate);
            issuedBooksFound = 1;
        }
    }
    if (!issuedBooksFound) {
        printf("No books are currently issued.\n");
    }
}


void returnBook()
{
    int id;
    char currentDate[11], dueDate[11];

    printf("Enter book ID to return: ");
    scanf("%d", &id);
    
    printf("Enter the return date (YYYY-MM-DD): ");
    scanf("%10s", currentDate); 

    
    printf("You entered: %s\n", currentDate);

    for (int i = 0; i < bookCount; i++)
    {
        if (books[i].id == id)
        {
            if (books[i].isIssued)
            {
                strcpy(dueDate, books[i].dueDate);

                int daysBetween = calculateDaysBetween(dueDate, currentDate);

                if (daysBetween < 0)
                {
                    printf("Error: Return date cannot be earlier than the issue date (%s).\n", dueDate);
                    return;  
                }

                if (daysBetween > 15)
                {
                    int overdueDays = daysBetween - 15;
                    int fine = overdueDays * 10; 
                    printf("Your book is overdue by %d days. Please pay a fine of $%d.\n", overdueDays, fine);
                }

                books[i].isIssued = 0;
                saveBooksToCSV();

                printf("Book returned successfully!\n");
            }
            else
            {
                printf("This book was not issued!\n");
            }
            return;
        }
    }
    printf("Book not found!\n");
}


void loadBooksFromCSV()
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
    {
        printf("No data file found. Starting fresh.\n");
        return;
    }

    bookCount = 0;
    while (fscanf(file, "%d,%[^,],%[^,],%d,%10s\n",
                  &books[bookCount].id,
                  books[bookCount].title,
                  books[bookCount].author,
                  &books[bookCount].isIssued,
                  books[bookCount].dueDate) != EOF)
    {
        bookCount++;
    }

    fclose(file);
    printf("Books loaded from CSV file.\n");
}


void saveBooksToCSV()
{
    FILE *file = fopen(FILE_NAME, "w");
    if (!file)
    {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < bookCount; i++)
    {
        fprintf(file, "%d,%s,%s,%d,%s\n", books[i].id, books[i].title, books[i].author, books[i].isIssued, books[i].dueDate);
    }

    fclose(file);
    printf("Books saved to CSV file.\n");
}
