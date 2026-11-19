#include <stdio.h>
#include <string.h>

#define MAX_VEHICLES 100
#define NAME_LEN 50

enum VehicleType { CAR = 1, BIKE = 2 };
enum RentalStatus { AVAILABLE = 1, RENTED = 2 };

union VehicleDetails {
    int seats;          /* Used only for cars */
    int engineCapacity; /* Used only for bikes */
};

struct Vehicle {
    int id;
    char name[NAME_LEN];
    char brand[NAME_LEN];
    enum VehicleType type;
    enum RentalStatus status;
    union VehicleDetails details;
};

struct Vehicle vehicles[MAX_VEHICLES];
int vehicleCount = 0;

int findVehicle(int id) {
    int i;
    for (i = 0; i < vehicleCount; i++) {
        if (vehicles[i].id == id)
            return i;
    }
    return -1;
}

void readLine(const char *prompt, char *value, int size) {
    int ch;
    printf("%s", prompt);
    if (fgets(value, size, stdin) != NULL) {
        if (strchr(value, '\n') != NULL) {
            value[strcspn(value, "\n")] = '\0';
        } else {
            while ((ch = getchar()) != '\n' && ch != EOF) { }
        }
    }
}

int readInt(const char *prompt) {
    char line[100];
    int value;
    while (1) {
        readLine(prompt, line, sizeof(line));
        if (sscanf(line, "%d", &value) == 1)
            return value;
        printf("Invalid input. Please enter a number.\n");
    }
}

void addVehicle(void) {
    struct Vehicle v;
    int type;

    if (vehicleCount >= MAX_VEHICLES) {
        printf("Vehicle limit reached!\n");
        return;
    }

    v.id = readInt("Enter Vehicle ID: ");
    if (findVehicle(v.id) != -1) {
        printf("Vehicle ID already exists!\n");
        return;
    }

    readLine("Enter Vehicle Name: ", v.name, NAME_LEN);
    readLine("Enter Brand: ", v.brand, NAME_LEN);

    printf("Select Vehicle Type:\n1. Car\n2. Bike\n");
    type = readInt("Enter choice: ");

    if (type == CAR) {
        v.type = CAR;
        v.details.seats = readInt("Enter Number of Seats: ");
        if (v.details.seats <= 0) {
            printf("Seats must be greater than zero. Vehicle not added.\n");
            return;
        }
    } else if (type == BIKE) {
        v.type = BIKE;
        v.details.engineCapacity = readInt("Enter Engine Capacity (cc): ");
        if (v.details.engineCapacity <= 0) {
            printf("Engine capacity must be greater than zero. Vehicle not added.\n");
            return;
        }
    } else {
        printf("Invalid vehicle type! Vehicle not added.\n");
        return;
    }

    v.status = AVAILABLE;
    vehicles[vehicleCount++] = v;
    printf("Vehicle Added Successfully!\n");
}

void displayVehicle(struct Vehicle v) {
    printf("\nVehicle ID: %d\n", v.id);
    printf("Vehicle Name: %s\n", v.name);
    printf("Brand: %s\n", v.brand);
    printf("Type: %s\n", v.type == CAR ? "Car" : "Bike");

    if (v.type == CAR)
        printf("Number of Seats: %d\n", v.details.seats);
    else
        printf("Engine Capacity: %d cc\n", v.details.engineCapacity);

    printf("Status: %s\n",
           v.status == AVAILABLE ? "Available" : "Rented");
}

void displayAllVehicles(void) {
    int i;
    if (vehicleCount == 0) {
        printf("No vehicles registered.\n");
        return;
    }

    for (i = 0; i < vehicleCount; i++)
        displayVehicle(vehicles[i]);
}

void searchVehicle(void) {
    int id = readInt("Enter Vehicle ID to search: ");
    int index = findVehicle(id);

    if (index == -1)
        printf("Vehicle ID not found!\n");
    else
        displayVehicle(vehicles[index]);
}

void rentVehicle(void) {
    int id = readInt("Enter Vehicle ID to rent: ");
    int index = findVehicle(id);

    if (index == -1) {
        printf("Vehicle ID not found!\n");
    } else if (vehicles[index].status == RENTED) {
        printf("This vehicle is already rented!\n");
    } else {
        vehicles[index].status = RENTED;
        printf("Vehicle rented successfully!\n");
    }
}

void returnVehicle(void) {
    int id = readInt("Enter Vehicle ID to return: ");
    int index = findVehicle(id);

    if (index == -1) {
        printf("Vehicle ID not found!\n");
    } else if (vehicles[index].status == AVAILABLE) {
        printf("This vehicle is already available!\n");
    } else {
        vehicles[index].status = AVAILABLE;
        printf("Vehicle returned successfully!\n");
    }
}

void displayAvailableVehicles(void) {
    int i, found = 0;

    for (i = 0; i < vehicleCount; i++) {
        if (vehicles[i].status == AVAILABLE) {
            displayVehicle(vehicles[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No vehicles are currently available.\n");
}

int main(void) {
    int choice;

    do {
        printf("\n===== VEHICLE RENTAL MANAGEMENT SYSTEM =====\n");
        printf("1. Add Vehicle\n");
        printf("2. Display All Vehicles\n");
        printf("3. Search Vehicle\n");
        printf("4. Rent Vehicle\n");
        printf("5. Return Vehicle\n");
        printf("6. Display Available Vehicles\n");
        printf("7. Exit\n");

        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: addVehicle(); break;
            case 2: displayAllVehicles(); break;
            case 3: searchVehicle(); break;
            case 4: rentVehicle(); break;
            case 5: returnVehicle(); break;
            case 6: displayAvailableVehicles(); break;
            case 7: printf("Thank you for using the system!\n"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}
