#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max_rooms 100
#define name_length 50
#define max_slots 500
#define password "user@123"

typedef struct Room
{
    char buildingName[name_length];
    char roomId[name_length];
    int count;
} Room;

typedef struct TimeSlot
{
    char buildingName[name_length];
    char roomId[name_length];
    int startTime;
    int endTime;
    char name[name_length];
    int count;
} TimeSlot;

Room rooms[max_rooms];
int roomCount = 0;

TimeSlot timeslots[max_slots];
int timeSlotCount = 0;

int key = 0;
void addRoom(char *buildingName, char *roomId)
{
    char input[20];
    while (key == 0)
    {
        printf("Enter password for admin access: ");
        scanf("%s", input);
        if (strcmp(input, password) == 0)
            key = 1;
        else
            printf("\nIncorrect Password, Please Try Again.\n");
    }
    if (key == 1)
    {
        if (roomCount >= max_rooms)
        {
            printf("Error: Cannot add more rooms, maximum limit reached.\n");
            return;
        }
        for (int i = 0; i < roomCount; i++)
        {
            if (!strcmp(rooms[i].buildingName, buildingName) && !strcmp(rooms[i].roomId, roomId))
            {
                printf("Room already exists!\n");
                return;
            }
        }

        strcpy(rooms[roomCount].buildingName, buildingName);
        strcpy(rooms[roomCount].roomId, roomId);
        roomCount++;
        printf("Room added: %s %s\n", buildingName, roomId);
    }
    return;
}

void removeRoom(char *buildingName, char *roomId)
{
    char input[20];
    while (key == 0)
    {
        printf("Enter password for admin access: ");
        scanf("%s", input);
        if (strcmp(input, password) == 0)
            key = 1;
        else
            printf("\nIncorrect Password, Please Try Again.\n");
    }
    if (key == 1)
    {
        for (int i = 0; i < roomCount; i++)
        {
            if (strcmp(rooms[i].buildingName, buildingName) == 0 &&
                strcmp(rooms[i].roomId, roomId) == 0)
            {
                for (int j = i; j < roomCount - 1; j++)
                {
                    rooms[j] = rooms[j + 1];
                }
                roomCount--;
                printf("Room removed: %s %s\n", buildingName, roomId);
                return;
            }
        }
        printf("Room not found: %s %s\n", buildingName, roomId);
        return;
    }
}

void reserveRoom(char *buildingName, char *roomId, int startTime, int endTime, char *name)
{
    if (timeSlotCount >= max_slots)
    {
        printf("Error: Cannot add more timeslots, maximum limit reached.\n");
        return;
    }
    int roomExists = 0;
    for (int i = 0; i < roomCount; i++)
    {
        if (strcmp(rooms[i].buildingName, buildingName) == 0 && strcmp(rooms[i].roomId, roomId) == 0)
        {
            roomExists = 1;
            break;
        }
    }

    if (!roomExists)
    {
        printf("Error: Room %s %s does not exist!\n", buildingName, roomId);
        return;
    }

    if (endTime - startTime > 6)
    {
        printf("Reservation is allowed only upto 6 hours!\n");
        return;
    }

    else if (startTime >= 0 && endTime <= 24 && startTime < endTime)
    {
        for (int i = 0; i < timeSlotCount; i++)
        {
            if (strcmp(timeslots[i].buildingName, buildingName) == 0 && strcmp(timeslots[i].roomId, roomId) == 0)
            {
                int existingStartTime = timeslots[i].startTime;
                int existingEndTime = timeslots[i].endTime;
                if (!(endTime <= existingStartTime || startTime >= existingEndTime))
                {
                    printf("Error: Time slot %d to %d for room %s %s is already reserved!\n", existingStartTime, existingEndTime, buildingName, roomId);
                    return;
                }
            }
        }
        strcpy(timeslots[timeSlotCount].buildingName, buildingName);
        strcpy(timeslots[timeSlotCount].roomId, roomId);
        strcpy(timeslots[timeSlotCount].name, name);
        timeslots[timeSlotCount].startTime = startTime;
        timeslots[timeSlotCount].endTime = endTime;
        timeSlotCount++;

        printf("Reserved %s %s from %d to %d for %s\n", buildingName, roomId, startTime, endTime, name);
    }
    else
    {
        printf("Invalid Time/Booking is allowed only between 0 and 24 within the same day.\n");
    }
    return;
}

int cancelRoom(char *buildingName, char *roomId, int startTime, int endTime, char *name)
{
    for (int i = 0; i < timeSlotCount; i++)
    {
        if (strcmp(timeslots[i].buildingName, buildingName) == 0 &&
            strcmp(timeslots[i].roomId, roomId) == 0 &&
            timeslots[i].startTime == startTime &&
            timeslots[i].endTime == endTime)
        {
            for (int j = i; j < timeSlotCount - 1; j++)
            {
                timeslots[j] = timeslots[j + 1];
            }
            timeSlotCount--;
            printf("Canceled %s %s from %d to %d for %s\n", buildingName, roomId, startTime, endTime, name);
            return 1;
        }
    }
    printf("Time slot not found: %s %s from %d to %d\n", buildingName, roomId, startTime, endTime);
    return 0;
}

void displayRooms()
{
    printf("Available rooms:\n");
    for (int i = 0; i < roomCount; i++)
    {
        printf("%s %s\n", rooms[i].buildingName, rooms[i].roomId);
    }
    return;
}

void displayTimeSlots()
{
    printf("Reserved time slots:\n");
    for (int i = 0; i < timeSlotCount; i++)
    {
        printf("%s %s from %d to %d for %s\n", timeslots[i].buildingName, timeslots[i].roomId,
               timeslots[i].startTime, timeslots[i].endTime, timeslots[i].name);
    }
    return;
}

void load()
{
    FILE *data;

    data = fopen("datarooms.txt", "rb");
    if (data != NULL)
    {
        fread(rooms, sizeof(Room), 100, data);
        roomCount = rooms[0].count;
    }
    else
        printf("data file (datarooms.txt) not found\n");

    fclose(data);

    data = fopen("datatimeslots.txt", "rb");
    if (data != NULL)
    {
        fread(timeslots, sizeof(TimeSlot), 500, data);
        timeSlotCount = timeslots[0].count;
    }
    else
        printf("data file (datatimeslots.txt) not found\n");

    fclose(data);
    return;
}

void save()
{
    FILE *data;

    data = fopen("datarooms.txt", "wb");
    if (data != NULL)
    {
        rooms[0].count = roomCount;
        fwrite(rooms, sizeof(Room), roomCount, data);
    }
    else
        printf("data file (dataroom.txt) not found");

    fclose(data);

    data = fopen("datatimeslots.txt", "wb");
    if (data != NULL)
    {
        timeslots[0].count = timeSlotCount;
        fwrite(timeslots, sizeof(TimeSlot), timeSlotCount, data);
    }
    else
        printf("data file (timeslots.txt) not found");

    fclose(data);
    return;
}

void edit(char *buildingName, char *roomId, int startTime, int endTime, char *name)
{
    for (int i = 0; i < timeSlotCount; i++)
    {
        if (strcmp(timeslots[i].buildingName, buildingName) == 0 &&
            strcmp(timeslots[i].roomId, roomId) == 0 &&
            timeslots[i].startTime == startTime &&
            timeslots[i].endTime == endTime)
        {
            printf("Enter the new details: ");
            char input[100];
            char newBuildingName[name_length], newRoomId[name_length], newName[name_length];
            int newStartTime, newEndTime;
            getchar();
            scanf("%[^\n]", input);

            if (sscanf(input, "%s %s %d %d %s", newBuildingName, newRoomId, &newStartTime, &newEndTime, newName) == 5){
                if (newEndTime <= newStartTime){
                    printf("Invalid time: End time must be greater than start time.\n");
                    return;
                }

                int roomExists = 0;
                for (int j = 0; j < roomCount; j++)
                {
                    if (strcmp(rooms[j].buildingName, newBuildingName) == 0 && strcmp(rooms[j].roomId, newRoomId) == 0)
                    {
                        roomExists = 1;
                        break;
                    }
                }

                if (!roomExists)
                {
                    printf("Error: Room %s %s does not exist!\n", newBuildingName, newRoomId);
                    return;
                }

                if (newEndTime - newStartTime > 6)
                {
                    printf("Reservation is allowed only up to 6 hours!\n");
                    return;
                }

                strcpy(timeslots[i].buildingName, newBuildingName);
                strcpy(timeslots[i].roomId, newRoomId);
                strcpy(timeslots[i].name, newName);
                timeslots[i].startTime = newStartTime;
                timeslots[i].endTime = newEndTime;
                printf("\nReservation updated\n");
                return;
            }
            else
            {
                printf("Invalid format!\n");
                return;
            }
        }
    }
    printf("This reservation doesn't exist!\n");
}


void logout()
{
    if (key == 1)
    {
        key = 0;
        printf("Successfully logged out\n");
    }
    else
        printf("Not logged in!\n");
}

void help()
{
    printf("List of Available Commands\n");
    printf("1. add - Adds a room to the list of available rooms.\nUsage: add [Building Name] [Room Number]\n\n");
    printf("2. remove - Removes an existing room.\nUsage: remove [Building Name] [Room Number]\n\n");
    printf("3. reserve - Reserves a room from the list of available rooms.\nUsage: reserve [Building Name] [Room Number] [Start Time] [End Time] [Name]\n\n");
    printf("4. cancel - Cancels an existing reservation.\nUsage: cancel [Building Name] [Room Number] [Start Time] [End Time] [Name]\n\n");
    printf("5. rooms - Displays a list of available rooms for reservation.\nUsage: rooms\n\n");
    printf("6. timeslots - Displays a list of reserved rooms along with their timeslots and name.\nUsage: timeslots.\n\n");
    printf("7. load - Loads the previously saved booking data.\nUsage: load\n\n");
    printf("8. edit - Edits an existing reservation.l\nUsage: edit [Building Name] [Room Number] [Start Time] [End Time] [Name].\nThen enter the new details in the same format.\n\n");
    printf("9. save - Saves the current booking data.\nUsage: save\n\n");
    printf("10. logout - logs out from admin user. \nUsage: logout\n\n");
    printf("11. exit - Exits the program.\nUsage: exit\n\n");
}   

void processInstruction(char *instruction)
{
    char command[20];
    char buildingName[name_length];
    char roomId[name_length];
    int startTime, endTime;
    char name[64];

  if (sscanf(instruction, "add %s %s", buildingName, roomId) == 2) {
        addRoom(buildingName, roomId);
    } else if (sscanf(instruction, "remove %s %s", buildingName, roomId) == 2) {
        removeRoom(buildingName, roomId);
    } else if (sscanf(instruction, "reserve %s %s %d %d %s", buildingName, roomId, &startTime, &endTime, name) == 5) {
        reserveRoom(buildingName, roomId, startTime, endTime, name);
    } else if (sscanf(instruction, "cancel %s %s %d %d %s", buildingName, roomId, &startTime, &endTime, name) == 5) {
        cancelRoom(buildingName, roomId, startTime, endTime, name);
    } else if (sscanf(instruction, "edit %s %s %d %d %s", buildingName, roomId, &startTime, &endTime, name) == 5) {
        edit(buildingName, roomId, startTime, endTime, name);
    } else if (strcmp(instruction, "rooms") == 0) {
        displayRooms();
    } else if (strcmp(instruction, "timeslots") == 0) {
        displayTimeSlots();
    } else if (strcmp(instruction, "help") == 0){
        help();
    } else if (strcmp(instruction, "load") == 0) {
        printf("loading data\n");
        load();
    } else if (strcmp(instruction, "save") == 0) {
        printf("saving data\n");
        save();
    } else if (strcmp(instruction, "logout") == 0) {
        printf("logging out...\n");
        logout();
    } else if (strcmp(instruction, "exit") == 0) {
        printf("Exiting the application.\n");
        exit(0); 
    } else {
        printf("Invalid command\n");
    }
}

int main()
{
    load();
    char instruction[100] = {0};
    while (1)
    {
        printf("Enter command: ");
        scanf(" %[^\n]", instruction);
        for (int i = 0; instruction[i] != '\0' && instruction[i] != ' '; i++)
        {
            if (instruction[i] >= 65 && instruction[i] <= 90)
            {
                instruction[i] = instruction[i] + 32;
            }
        }
        processInstruction(instruction);
    }

    return 0;
}