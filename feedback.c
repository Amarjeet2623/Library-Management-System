#include <stdio.h>
#include <string.h>
#include "feedback.h"

#define MAX_FEEDBACK 500

void giveFeedback() {
    FILE *file = fopen("feedback.csv", "a");
    if (file == NULL) {
        printf("Error opening file for feedback!\n");
        return;
    }

    char feedback[MAX_FEEDBACK];
    printf("Enter your feedback or suggestions: ");
    getchar();
    fgets(feedback, MAX_FEEDBACK, stdin);

    
    feedback[strcspn(feedback, "\n")] = 0;

    
    fprintf(file, "\"%s\"\n", feedback);
    fclose(file);

    printf("Thank you for your feedback!\n");
}

void displayFeedback() {
    FILE *file = fopen("feedback.csv", "r");
    if (file == NULL) {
        printf("No feedback available yet.\n");
        return;
    }

    char feedback[MAX_FEEDBACK];
    printf("\n--- Feedback and Suggestions ---\n");
    while (fgets(feedback, MAX_FEEDBACK, file)) {
        printf("%s", feedback); 
    }
    fclose(file);
    printf("\n--- End of Feedback ---\n");
}
