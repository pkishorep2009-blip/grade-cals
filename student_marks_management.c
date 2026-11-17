#include <stdio.h>

int main()
{
    int choice = 0;
    int roll = 0;
    int m1 = 0, m2 = 0, m3 = 0, m4 = 0, m5 = 0;
    int total = 0;
    float average = 0;
    char name[30] = "";
    char grade = 'F';
    int pass = 0;
    int entered = 0;
    int calculated = 0;

    do
    {
        printf("\n\nSTUDENT MARKS MANAGEMENT SYSTEM");
        printf("\n--------------------------------");
        printf("\n1. Enter Student Details");
        printf("\n2. Calculate Result");
        printf("\n3. Display Result");
        printf("\n4. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:

                printf("\nEnter Roll Number: ");
                scanf("%d", &roll);

                printf("Enter Student Name: ");
                scanf("%29s", name);

                printf("Enter Subject 1 Mark: ");
                scanf("%d", &m1);

                printf("Enter Subject 2 Mark: ");
                scanf("%d", &m2);

                printf("Enter Subject 3 Mark: ");
                scanf("%d", &m3);

                printf("Enter Subject 4 Mark: ");
                scanf("%d", &m4);

                printf("Enter Subject 5 Mark: ");
                scanf("%d", &m5);

                entered = 1;
                calculated = 0;

                break;

            case 2:

                if(entered == 0)
                {
                    printf("\nEnter Student Details First!");
                    break;
                }

                total = m1 + m2 + m3 + m4 + m5;
                average = total / 5.0;

                pass = 1;

                if(m1 < 35 || m2 < 35 || m3 < 35 ||
                   m4 < 35 || m5 < 35)
                {
                    pass = 0;
                }

                if(pass == 0)
                    grade = 'F';
                else if(average >= 90)
                    grade = 'A';
                else if(average >= 80)
                    grade = 'B';
                else if(average >= 70)
                    grade = 'C';
                else if(average >= 60)
                    grade = 'D';
                else
                    grade = 'F';

                calculated = 1;

                printf("\nResult Calculated Successfully!");

                break;

            case 3:

                if(entered == 0 || calculated == 0)
                {
                    printf("\nPlease Enter Details and Calculate Result First!");
                    break;
                }

                printf("\n\nSTUDENT RESULT");
                printf("\n----------------------");

                printf("\nRoll Number : %d", roll);
                printf("\nName        : %s", name);
                printf("\nTotal       : %d", total);
                printf("\nAverage     : %.2f", average);
                printf("\nGrade       : %c", grade);

                if(pass == 1)
                    printf("\nResult      : PASS");
                else
                    printf("\nResult      : FAIL");

                break;

            case 4:

                printf("\nExiting Program...");
                break;

            default:

                printf("\nInvalid Choice!");
        }

    } while(choice != 4);

    return 0;
}


OUTPUT:

Roll Number: 101

Name: Arun

Total: 400

Average: 80.00

Grade: B

Result: PASS