#ifndef BOOK_H
#define BOOK_H

typedef struct
{
    int id;
    char title[50];
    char author[50];
    int isIssued;     
    char dueDate[11];

} Book;

// Declare a global array of books and the count of books
extern Book books[100];
extern int bookCount;

// Function declarations
void addBook();
void deleteBook();
void searchBook();
void displayBooks();
void issueBook();
void returnBook();
void loadBooksFromCSV();
void saveBooksToCSV();
void editBookDetails();
void viewPopularBooks();
void viewIssuedBooks();
void getCurrentDate(char *buffer);
int calculateDaysBetween(char *startDate, char *endDate);


#endif
