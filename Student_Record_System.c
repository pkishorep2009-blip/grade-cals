/*
 * UNIT 5 MINI PROJECT
 * Student Record and File Management System
 * Concepts: File Handling, Dynamic Memory Allocation, Storage Classes
 *
 * Files created by this program:
 *   students.dat  - binary student records
 *   students.txt  - readable text copy of student records
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BINARY_FILE "students.dat"
#define TEXT_FILE   "students.txt"
#define NAME_SIZE  50
#define DEPT_SIZE  40

typedef struct {
    int roll;
    char name[NAME_SIZE];
    char department[DEPT_SIZE];
    int year;
    float marks;
    char grade[3];
} Student;

static int operation_count = 0; /* static storage class */

void remove_newline(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

void clear_input(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) { }
}

void read_line(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    if (strchr(buffer, '\n') == NULL) clear_input();
    remove_newline(buffer);
}

int read_int(const char *prompt) {
    int value;
    char line[100];
    for (;;) {
        read_line(prompt, line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1) return value;
        printf("Invalid number. Please try again.\n");
    }
}

float read_float(const char *prompt) {
    float value;
    char line[100];
    for (;;) {
        read_line(prompt, line, sizeof(line));
        if (sscanf(line, "%f", &value) == 1) return value;
        printf("Invalid marks. Please try again.\n");
    }
}

void assign_grade(Student *s) {
    if (s->marks >= 90) strcpy(s->grade, "A+");
    else if (s->marks >= 80) strcpy(s->grade, "A");
    else if (s->marks >= 70) strcpy(s->grade, "B");
    else if (s->marks >= 60) strcpy(s->grade, "C");
    else if (s->marks >= 50) strcpy(s->grade, "D");
    else strcpy(s->grade, "F");
}

int find_student(const Student *records, size_t count, int roll) {
    size_t i;
    for (i = 0; i < count; i++)
        if (records[i].roll == roll) return (int)i;
    return -1;
}

/* Load binary records into dynamically allocated memory. */
int load_records(Student **records, size_t *count, size_t *capacity) {
    FILE *fp = fopen(BINARY_FILE, "rb");
    Student temp;
    if (fp == NULL) {
        *records = (Student *)calloc(2, sizeof(Student));
        if (*records == NULL) {
            printf("Memory allocation failed.\n");
            return 0;
        }
        *count = 0;
        *capacity = 2;
        return 1; /* first run: file does not exist yet */
    }

    *capacity = 4;
    *count = 0;
    *records = (Student *)malloc(*capacity * sizeof(Student));
    if (*records == NULL) {
        fclose(fp);
        printf("Memory allocation failed.\n");
        return 0;
    }

    while (fread(&temp, sizeof(Student), 1, fp) == 1) {
        if (*count == *capacity) {
            size_t new_capacity = (*capacity) * 2;
            Student *new_records =
                (Student *)realloc(*records, new_capacity * sizeof(Student));
            if (new_records == NULL) {
                printf("Memory resize failed.\n");
                free(*records);
                fclose(fp);
                *records = NULL;
                return 0;
            }
            *records = new_records;
            *capacity = new_capacity;
        }
        (*records)[(*count)++] = temp;
    }

    if (ferror(fp)) {
        printf("Error while reading binary file.\n");
        free(*records);
        *records = NULL;
        fclose(fp);
        return 0;
    }
    fclose(fp);
    return 1;
}

/* Save records to a temporary binary file, then replace the old file. */
int save_binary(const Student *records, size_t count) {
    FILE *fp = fopen("students.tmp", "wb");
    if (fp == NULL) {
        printf("Cannot open temporary binary file.\n");
        return 0;
    }
    if (count > 0 && fwrite(records, sizeof(Student), count, fp) != count) {
        printf("Error writing binary records.\n");
        fclose(fp);
        remove("students.tmp");
        return 0;
    }
    if (fclose(fp) != 0) {
        printf("Error closing temporary file.\n");
        remove("students.tmp");
        return 0;
    }
    if (rename("students.tmp", BINARY_FILE) != 0) {
        printf("Could not replace binary data file.\n");
        remove("students.tmp");
        return 0;
    }
    return 1;
}

/* Export a readable text version using fprintf(). */
int save_text(const Student *records, size_t count) {
    size_t i;
    FILE *fp = fopen(TEXT_FILE, "w");
    if (fp == NULL) {
        printf("Cannot open text file for writing.\n");
        return 0;
    }
    fprintf(fp, "ROLL\tNAME\tDEPARTMENT\tYEAR\tMARKS\tGRADE\n");
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d\t%s\t%s\t%d\t%.2f\t%s\n",
                records[i].roll, records[i].name, records[i].department,
                records[i].year, records[i].marks, records[i].grade);
    }
    if (fclose(fp) != 0) {
        printf("Error closing text file.\n");
        return 0;
    }
    return 1;
}

int save_all(const Student *records, size_t count) {
    if (!save_binary(records, count)) return 0;
    if (!save_text(records, count))
        printf("Warning: binary file saved, but text export failed.\n");
    return 1;
}

void display_one(const Student *s) {
    printf("%-8d %-20s %-16s %-5d %-8.2f %-5s\n",
           s->roll, s->name, s->department, s->year, s->marks, s->grade);
}

void display_all(const Student *records, size_t count) {
    size_t i;
    if (count == 0) {
        printf("No student records found.\n");
        return;
    }
    printf("\n%-8s %-20s %-16s %-5s %-8s %-5s\n",
           "Roll", "Name", "Department", "Year", "Marks", "Grade");
    printf("--------------------------------------------------------------------------\n");
    for (i = 0; i < count; i++) display_one(&records[i]);
}

void add_student(Student **records, size_t *count, size_t *capacity) {
    Student s;
    Student *temp;
    s.roll = read_int("Enter roll number: ");
    if (find_student(*records, *count, s.roll) != -1) {
        printf("That roll number already exists.\n");
        return;
    }
    read_line("Enter student name: ", s.name, sizeof(s.name));
    read_line("Enter department: ", s.department, sizeof(s.department));
    s.year = read_int("Enter year of study: ");
    do {
        s.marks = read_float("Enter marks (0-100): ");
        if (s.marks < 0 || s.marks > 100)
            printf("Marks must be between 0 and 100.\n");
    } while (s.marks < 0 || s.marks > 100);
    assign_grade(&s);

    if (*count == *capacity) {
        size_t new_capacity = (*capacity) * 2;
        temp = (Student *)realloc(*records, new_capacity * sizeof(Student));
        if (temp == NULL) {
            printf("Unable to increase memory capacity.\n");
            return;
        }
        *records = temp;
        *capacity = new_capacity;
    }
    (*records)[(*count)++] = s;
    operation_count++;
    if (save_all(*records, *count))
        printf("Student record added and saved successfully.\n");
}

void search_student(const Student *records, size_t count) {
    int roll = read_int("Enter roll number to search: ");
    int index = find_student(records, count, roll);
    if (index == -1) printf("Student not found.\n");
    else {
        printf("\n%-8s %-20s %-16s %-5s %-8s %-5s\n",
               "Roll", "Name", "Department", "Year", "Marks", "Grade");
        display_one(&records[index]);
    }
}

void update_student(Student *records, size_t count) {
    int roll = read_int("Enter roll number to update: ");
    int index = find_student(records, count, roll);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }
    printf("Enter updated details:\n");
    read_line("New student name: ", records[index].name, NAME_SIZE);
    read_line("New department: ", records[index].department, DEPT_SIZE);
    records[index].year = read_int("New year: ");
    do {
        records[index].marks = read_float("New marks (0-100): ");
        if (records[index].marks < 0 || records[index].marks > 100)
            printf("Marks must be between 0 and 100.\n");
    } while (records[index].marks < 0 || records[index].marks > 100);
    assign_grade(&records[index]);
    operation_count++;
    if (save_all(records, count))
        printf("Student record updated successfully.\n");
}

void delete_student(Student *records, size_t *count) {
    size_t i;
    int roll = read_int("Enter roll number to delete: ");
    int index = find_student(records, *count, roll);
    if (index == -1) {
        printf("Student not found.\n");
        return;
    }
    for (i = (size_t)index; i + 1 < *count; i++)
        records[i] = records[i + 1];
    (*count)--;
    operation_count++;
    if (save_all(records, *count))
        printf("Student record deleted successfully.\n");
}

void filter_by_marks(const Student *records, size_t count) {
    size_t i;
    int found = 0;
    float limit = read_float("Display students with marks greater than: ");
    printf("\nStudents scoring above %.2f:\n", limit);
    for (i = 0; i < count; i++) {
        if (records[i].marks > limit) {
            if (!found)
                printf("%-8s %-20s %-16s %-5s %-8s %-5s\n",
                       "Roll", "Name", "Department", "Year", "Marks", "Grade");
            display_one(&records[i]);
            found = 1;
        }
    }
    if (!found) printf("No matching students.\n");
}

/* Demonstrates fscanf() and EOF handling by reading the text file. */
void read_text_file(void) {
    FILE *fp = fopen(TEXT_FILE, "r");
    char line[250];
    if (fp == NULL) {
        printf("Text file is not available. Save a record first.\n");
        return;
    }
    printf("\nContents of %s:\n", TEXT_FILE);
    while (fgets(line, sizeof(line), fp) != NULL)
        printf("%s", line);
    if (ferror(fp)) printf("Error reading text file.\n");
    fclose(fp);
}

/* Demonstrates fscanf() with formatted fields (names/departments have no spaces
   in this demonstration reader; the full text export is displayed above). */
void demonstrate_fscanf(void) {
    FILE *fp = fopen("student_demo.txt", "r");
    int roll, year;
    float marks;
    char name[NAME_SIZE], dept[DEPT_SIZE], grade[3];
    if (fp == NULL) {
        printf("Formatted text input file not found.\n");
        return;
    }
    printf("\nFormatted text records (fscanf):\n");
    while (fscanf(fp, "%d %49s %39s %d %f %2s",
                  &roll, name, dept, &year, &marks, grade) == 6) {
        printf("%d %s %s %d %.2f %s\n",
               roll, name, dept, year, marks, grade);
    }
    if (!feof(fp) && ferror(fp)) printf("Error reading formatted text file.\n");
    fclose(fp);
}

void show_menu(void) {
    printf("\n========== STUDENT RECORD SYSTEM ==========\n");
    printf("1. Add Student Record\n");
    printf("2. Display All Records\n");
    printf("3. Search by Roll Number\n");
    printf("4. Update Student Record\n");
    printf("5. Delete Student Record\n");
    printf("6. Count Total Students\n");
    printf("7. Filter by Marks\n");
    printf("8. Display Text File\n");
    printf("9. Read formatted demo text (fscanf)\n");
    printf("10. Exit\n");
    printf("===========================================\n");
}

int main(void) {
    Student *records = NULL;
    size_t count = 0, capacity = 0;
    int choice;

    if (!load_records(&records, &count, &capacity)) return 1;

    do {
        show_menu();
        choice = read_int("Enter your choice: ");
        switch (choice) {
            case 1:
                add_student(&records, &count, &capacity);
                break;
            case 2:
                display_all(records, count);
                break;
            case 3:
                search_student(records, count);
                break;
            case 4:
                update_student(records, count);
                break;
            case 5:
                delete_student(records, &count);
                break;
            case 6:
                printf("Total number of students: %zu\n", count);
                break;
            case 7:
                filter_by_marks(records, count);
                break;
            case 8:
                read_text_file();
                break;
            case 9:
                demonstrate_fscanf();
                break;
            case 10:
                printf("Closing files and releasing memory...\n");
                break;
            default:
                printf("Invalid choice. Select 1 to 10.\n");
        }
    } while (choice != 10);

    free(records);
    printf("Memory released. Total add/update/delete operations: %d\n",
           operation_count);
    printf("Program closed successfully.\n");
    return 0;
}
