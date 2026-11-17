#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 50
#define SIZE 100

int bookCount = 0;

int readInt(char message[])
{
    char input[100];
    int value;

    while (1)
    {
        printf("%s", message);
        if (fgets(input, sizeof(input), stdin) != NULL &&
            sscanf(input, "%d", &value) == 1)
            return value;

        printf("Invalid input! Please enter a number.\n");
    }
}

float readFloat(char message[])
{
    char input[100];
    float value;

    while (1)
    {
        printf("%s", message);
        if (fgets(input, sizeof(input), stdin) != NULL &&
            sscanf(input, "%f", &value) == 1 && value >= 0)
            return value;

        printf("Invalid price! Please enter a valid non-negative price.\n");
    }
}

void readString(char message[], char value[], int size)
{
    printf("%s", message);

    if (fgets(value, size, stdin) != NULL)
    {
        value[strcspn(value, "\n")] = '\0';
    }
}

int findBook(int id[], int count, int searchId)
{
    int i;

    for (i = 0; i < count; i++)
    {
        if (id[i] == searchId)
            return i;
    }

    return -1;
}

/* Static local variable retains the issue count between calls. */
int updateIssueCount(int change)
{
    static int issueCount = 0;

    issueCount += change;

    return issueCount;
}

void addBook(int id[], char name[][SIZE], char author[][SIZE],
             char category[][SIZE], float price[], int available[])
{
    int newId;

    if (bookCount >= MAX)
    {
        printf("\nLibrary is full! Cannot add more books.\n");
        return;
    }

    newId = readInt("Enter Book ID: ");

    if (findBook(id, bookCount, newId) != -1)
    {
        printf("Duplicate Book ID! Book not added.\n");
        return;
    }

    id[bookCount] = newId;

    readString("Enter Book Name: ", name[bookCount], SIZE);
    readString("Enter Author Name: ", author[bookCount], SIZE);
    readString("Enter Category: ", category[bookCount], SIZE);

    price[bookCount] = readFloat("Enter Price: ");
    available[bookCount] = 1;

    bookCount++;

    printf("\nBook added successfully!\n");
}

void displayOneBook(int id[], char name[][SIZE], char author[][SIZE],
                    char category[][SIZE], float price[], int available[],
                    int index)
{
    printf("\nBook ID       : %d", id[index]);
    printf("\nBook Name     : %s", name[index]);
    printf("\nAuthor Name   : %s", author[index]);
    printf("\nCategory      : %s", category[index]);
    printf("\nPrice         : %.2f", price[index]);
    printf("\nAvailability  : %s\n",
           available[index] == 1 ? "Available" : "Issued");
}

void displayBooks(int id[], char name[][SIZE], char author[][SIZE],
                  char category[][SIZE], float price[], int available[],
                  int count)
{
    int i;

    if (count == 0)
    {
        printf("\nNo books available in the library.\n");
        return;
    }

    printf("\n========== ALL BOOK DETAILS ==========\n");

    for (i = 0; i < count; i++)
    {
        printf("\n--------------------------------------");
        displayOneBook(id, name, author, category, price, available, i);
    }
}

void searchById(int id[], char name[][SIZE], char author[][SIZE],
                char category[][SIZE], float price[], int available[],
                int count)
{
    int searchId;
    int index;

    searchId = readInt("Enter Book ID to search: ");
    index = findBook(id, count, searchId);

    if (index == -1)
        printf("\nBook not found!\n");
    else
        displayOneBook(id, name, author, category, price, available, index);
}

void searchByName(int id[], char name[][SIZE], char author[][SIZE],
                  char category[][SIZE], float price[], int available[],
                  int count)
{
    char searchName[SIZE];
    int i, found = 0;

    readString("Enter Book Name to search: ", searchName, SIZE);

    for (i = 0; i < count; i++)
    {
        if (strstr(name[i], searchName) != NULL)
        {
            displayOneBook(id, name, author, category, price, available, i);
            found = 1;
        }
    }

    if (!found)
        printf("\nBook not found!\n");
}

void issueBook(int id[], int available[], int count)
{
    int searchId;
    int index;

    searchId = readInt("Enter Book ID to issue: ");
    index = findBook(id, count, searchId);

    if (index == -1)
    {
        printf("\nBook not found!\n");
    }
    else if (available[index] == 0)
    {
        printf("\nThis book is already issued!\n");
    }
    else
    {
        available[index] = 0;
        updateIssueCount(1);
        printf("\nBook issued successfully!\n");
    }
}

void returnBook(int id[], int available[], int count)
{
    int searchId;
    int index;

    searchId = readInt("Enter Book ID to return: ");
    index = findBook(id, count, searchId);

    if (index == -1)
    {
        printf("\nBook not found!\n");
    }
    else if (available[index] == 1)
    {
        printf("\nThis book is already available!\n");
    }
    else
    {
        available[index] = 1;
        updateIssueCount(-1);
        printf("\nBook returned successfully!\n");
    }
}

void updateBook(int id[], char name[][SIZE], char author[][SIZE],
                char category[][SIZE], float price[], int count)
{
    int searchId, index, choice;

    searchId = readInt("Enter Book ID to update: ");
    index = findBook(id, count, searchId);

    if (index == -1)
    {
        printf("\nBook not found!\n");
        return;
    }

    printf("\n1. Update Book Name");
    printf("\n2. Update Author Name");
    printf("\n3. Update Category");
    printf("\n4. Update Price\n");

    choice = readInt("Enter your choice: ");

    switch (choice)
    {
        case 1:
            readString("Enter New Book Name: ", name[index], SIZE);
            break;

        case 2:
            readString("Enter New Author Name: ", author[index], SIZE);
            break;

        case 3:
            readString("Enter New Category: ", category[index], SIZE);
            break;

        case 4:
            price[index] = readFloat("Enter New Price: ");
            break;

        default:
            printf("\nInvalid update choice!\n");
            return;
    }

    printf("\nBook details updated successfully!\n");
}

void countBooks(int count)
{
    printf("\nTotal number of books: %d\n", count);
}

void displayAvailable(int id[], char name[][SIZE], char author[][SIZE],
                      char category[][SIZE], float price[],
                      int available[], int count)
{
    int i, found = 0;

    printf("\n========== AVAILABLE BOOKS ==========\n");

    for (i = 0; i < count; i++)
    {
        if (available[i] == 1)
        {
            displayOneBook(id, name, author, category, price, available, i);
            found = 1;
        }
    }

    if (!found)
        printf("\nNo books are currently available.\n");
}

int main()
{
    int id[MAX], available[MAX];
    char name[MAX][SIZE];
    char author[MAX][SIZE];
    char category[MAX][SIZE];
    float price[MAX];

    int choice;

    do
    {
        printf("\n\n========== LIBRARY MANAGEMENT SYSTEM ==========");
        printf("\n1. Add New Book");
        printf("\n2. Display All Books");
        printf("\n3. Search Book by ID");
        printf("\n4. Search Book by Name");
        printf("\n5. Issue Book");
        printf("\n6. Return Book");
        printf("\n7. Update Book Details");
        printf("\n8. Count Total Books");
        printf("\n9. Display Available Books");
        printf("\n10. Exit\n");

        choice = readInt("Enter your choice: ");

        switch (choice)
        {
            case 1:
                addBook(id, name, author, category, price, available);
                break;

            case 2:
                displayBooks(id, name, author, category, price, available, bookCount);
                break;

            case 3:
                searchById(id, name, author, category, price, available, bookCount);
                break;

            case 4:
                searchByName(id, name, author, category, price, available, bookCount);
                break;

            case 5:
                issueBook(id, available, bookCount);
                break;

            case 6:
                returnBook(id, available, bookCount);
                break;

            case 7:
                updateBook(id, name, author, category, price, bookCount);
                break;

            case 8:
                countBooks(bookCount);
                break;

            case 9:
                displayAvailable(id, name, author, category, price, available, bookCount);
                break;

            case 10:
                printf("\nThank you for using the Library Management System!\n");
                break;

            default:
                printf("\nInvalid choice! Please enter a number from 1 to 10.\n");
        }

    } while (choice != 10);

    return 0;
}


OUTPUT:

========== LIBRARY MANAGEMENT SYSTEM ========== 
1. Add New Book 
2. Display All Books 
3. Search Book by ID 
4. Search Book by Name 
5. Issue Book 
6. Return Book 
7. Update Book Details
 8. Count Total Books 
 9. Display Available Books 
 10. Exit Enter your choice: _
Enter your choice: 1 
Enter Book ID: 101 
Enter Book Name: C Programming 
Enter Author Name: E. Balagurusamy 
Enter Category: Programming 
Enter Price: 450 
Book added successfully! 
Enter your choice: 5
 Enter Book ID to issue: 101
  Book issued successfully!