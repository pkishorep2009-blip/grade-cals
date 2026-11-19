/*
    STUDENT DETAILS MANAGEMENT SYSTEM
    C Programming - Unit Project

    Description:
    Collects and displays multiple student records containing:
    Roll Number, Name, Cut-off Mark, City, and Dream.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEXT 100

struct Student {
    int rollNumber;
    char name[MAX_TEXT];
    float cutoffMark;
    char city[MAX_TEXT];
    char dream[MAX_TEXT];
};

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* Discard remaining characters */
    }
}

void readText(const char prompt[], char text[], int size) {
    printf("%s", prompt);

    if (fgets(text, size, stdin) != NULL) {
        text[strcspn(text, "\n")] = '\0';
    }
}

int main(void) {
    int count;

    printf("========================================\n");
    printf("     STUDENT DETAILS MANAGEMENT SYSTEM\n");
    printf("========================================\n");

    printf("Enter the number of students: ");
    if (scanf("%d", &count) != 1) {
        printf("Invalid input. Please enter a number.\n");
        return 1;
    }
    clearInputBuffer();

    while (count <= 0) {
        printf("Student count must be positive.\n");
        printf("Enter the number of students: ");

        if (scanf("%d", &count) != 1) {
            printf("Invalid input. Please enter a number.\n");
            return 1;
        }
        clearInputBuffer();
    }

    struct Student *students =
        (struct Student *)malloc(count * sizeof(struct Student));

    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    /* Collect details of each student */
    for (int i = 0; i < count; i++) {
        printf("\n---------- Enter Student %d Details ----------\n", i + 1);

        printf("Enter Roll Number: ");
        if (scanf("%d", &students[i].rollNumber) != 1) {
            printf("Invalid roll number.\n");
            free(students);
            return 1;
        }
        clearInputBuffer();

        readText("Enter Name: ", students[i].name, MAX_TEXT);

        printf("Enter Cut-off Mark: ");
        if (scanf("%f", &students[i].cutoffMark) != 1) {
            printf("Invalid cut-off mark.\n");
            free(students);
            return 1;
        }
        clearInputBuffer();

        readText("Enter City: ", students[i].city, MAX_TEXT);
        readText("Enter Dream: ", students[i].dream, MAX_TEXT);
    }

    /* Display all student records */
    printf("\n========================================\n");
    printf("          STUDENT RECORDS\n");
    printf("========================================\n");

    for (int i = 0; i < count; i++) {
        printf("\nStudent %d\n", i + 1);
        printf("----------------------------------------\n");
        printf("Roll Number  : %d\n", students[i].rollNumber);
        printf("Name         : %s\n", students[i].name);
        printf("Cut-off Mark : %.2f\n", students[i].cutoffMark);
        printf("City         : %s\n", students[i].city);
        printf("Dream        : %s\n", students[i].dream);
    }

    printf("\nAll student records displayed successfully.\n");

    free(students);
    return 0;
}

/*
========================================
             SAMPLE OUTPUT
========================================

STUDENT DETAILS MANAGEMENT SYSTEM
========================================
Enter the number of students: 2

---------- Enter Student 1 Details ----------
Enter Roll Number: 101
Enter Name: Arun Kumar
Enter Cut-off Mark: 185.5
Enter City: Chennai
Enter Dream: Software Engineer

---------- Enter Student 2 Details ----------
Enter Roll Number: 102
Enter Name: Priya
Enter Cut-off Mark: 190
Enter City: Ambur
Enter Dream: Doctor

========================================
          STUDENT RECORDS
========================================

Student 1
----------------------------------------
Roll Number  : 101
Name         : Arun Kumar
Cut-off Mark : 185.50
City         : Chennai
Dream        : Software Engineer

Student 2
----------------------------------------
Roll Number  : 102
Name         : Priya
Cut-off Mark : 190.00
City         : Ambur
Dream        : Doctor

All student records displayed successfully.

*/
