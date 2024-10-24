#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define max_rooms 100
#define name_length 50
#define max_slots 500
#define password "user@123"

typedef struct Room {
    char buildingName[name_length];
    char roomId[name_length];
    int count;
} Room;

typedef struct TimeSlot {
    char buildingName[name_length];
    char roomId[name_length];
    int startTime;
    int endTime;
    char name[64];
    int count;
} TimeSlot;

Room rooms[max_rooms];
int roomCount = 0;

TimeSlot timeslots[max_slots];
int timeSlotCount = 0;


int key = 0;
void addRoom(const char* buildingName, const char* roomId) {
    char input[20];
    while(key == 0){
    printf("Enter password for admin access: ");
    scanf("%s", input);
    if(strcmp(input, password) == 0) key = 1;
    else printf("\nIncorrect Password, Please Try Again.\n");
    }
    if (key == 1){
    if (roomCount >= max_rooms) {
        printf("Error: Cannot add more rooms, maximum limit reached.\n");
        return;
    }
    strcpy(rooms[roomCount].buildingName, buildingName);
    strcpy(rooms[roomCount].roomId, roomId);
    roomCount++;
    printf("Room added: %s %s\n", buildingName, roomId);
}
}

void removeRoom(const char* buildingName, const char* roomId) {
    for (int i = 0; i < roomCount; i++) {
        if (strcmp(rooms[i].buildingName, buildingName) == 0 &&
            strcmp(rooms[i].roomId, roomId) == 0) {
            // Shift all rooms after the removed one to fill the gap
            for (int j = i; j < roomCount - 1; j++) {
                rooms[j] = rooms[j + 1];
            }
            roomCount--;
            printf("Room removed: %s %s\n", buildingName, roomId);
            return;
        }
    }
    printf("Room not found: %s %s\n", buildingName, roomId);
}

void reserveRoom(const char* buildingName, const char* roomId, int startTime, int endTime, char* name) {
    if (timeSlotCount >= max_slots) {
        printf("Error: Cannot add more timeslots, maximum limit reached.\n");
        return;
    }
    // Check if the room exists in the list of rooms
    int roomExists = 0;
    for (int i = 0; i < roomCount; i++) {
        if (strcmp(rooms[i].buildingName, buildingName) == 0 && strcmp(rooms[i].roomId, roomId) == 0) {
            roomExists = 1; // Room found
            break;
        }
    }

    if (!roomExists) {
        printf("Error: Room %s %s does not exist!\n", buildingName, roomId);
        return;
    }
    // Check if the requested time slot overlaps with any existing reservation
    if (startTime >= 0 && endTime <= 24 && startTime < endTime) {
        for (int i = 0; i < timeSlotCount; i++) {
            if (strcmp(timeslots[i].buildingName, buildingName) == 0 && strcmp(timeslots[i].roomId, roomId) == 0) {
                // Check for overlapping time slots
                int existingStartTime = timeslots[i].startTime;
                int existingEndTime = timeslots[i].endTime;
                if (!(endTime <= existingStartTime || startTime >= existingEndTime)) {
                    // Time slot overlaps
                    printf("Error: Time slot %d to %d for room %s %s is already reserved!\n", existingStartTime, existingEndTime, buildingName, roomId);
                    return;
                }
            }
        }

        // If no overlap, reserve the time slot
        strcpy(timeslots[timeSlotCount].buildingName, buildingName);
        strcpy(timeslots[timeSlotCount].roomId, roomId);
        strcpy(timeslots[timeSlotCount].name, name);
        timeslots[timeSlotCount].startTime = startTime;
        timeslots[timeSlotCount].endTime = endTime;
        timeSlotCount++;

        printf("Reserved %s %s from %d to %d for %s\n", buildingName, roomId, startTime, endTime, name);
    } else {
        printf("Invalid Time, Booking is allowed only between 0 and 24 wihtin the same day.\n");
    }
}

void cancelRoom(const char* buildingName, const char* roomId, int startTime, int endTime, char* name) {
    for (int i = 0; i < timeSlotCount; i++) {
        if (strcmp(timeslots[i].buildingName, buildingName) == 0 &&
            strcmp(timeslots[i].roomId, roomId) == 0 &&
            timeslots[i].startTime == startTime &&
            timeslots[i].endTime == endTime) {
            // Shift all timeslots after the canceled one to fill the gap
            for (int j = i; j < timeSlotCount - 1; j++) {
                timeslots[j] = timeslots[j + 1];
            }
            timeSlotCount--;
            printf("Canceled %s %s from %d to %d for %s\n", buildingName, roomId, startTime, endTime, name);
            return;
        }
    }
    printf("Time slot not found: %s %s from %d to %d\n", buildingName, roomId, startTime, endTime);
}

void displayRooms() {
    printf("Available rooms:\n");
    for (int i = 0; i < roomCount; i++) {
        printf("%s %s\n", rooms[i].buildingName, rooms[i].roomId);
    }
}

// Function to display all reserved time slots
void displayTimeSlots() {
    printf("Reserved time slots:\n");
    for (int i = 0; i < timeSlotCount; i++) {
        printf("%s %s from %d to %d for %s\n", timeslots[i].buildingName, timeslots[i].roomId,
               timeslots[i].startTime, timeslots[i].endTime, timeslots[i].name);
    }
}

void load()
{
 FILE *data;
 
  data = fopen("datarooms.txt","rb");
  if(data != NULL)
  {
  fread(rooms,sizeof(Room),100,data);
  roomCount = rooms[0].count;
  }
  else
  printf("data file (dataroom.txt) not found");

  fclose(data);

   data = fopen("datatimeslots.txt","rb");
  if(data != NULL)
  {
   fread(timeslots,sizeof(TimeSlot),500,data);
    timeSlotCount = timeslots[0].count;
  }
    else
   printf("data file (datatimeslots.txt) not found");

  fclose(data);
}

void save()
{
 FILE *data;

  data = fopen("datarooms.txt","wb");
  if(data != NULL)
  {
    rooms[0].count = roomCount;
   fwrite(rooms,sizeof(Room),roomCount,data);
  }
  else
   printf("data file (dataroom.txt) not found");

  fclose(data);

  data = fopen("datatimeslots.txt","wb");
  if(data != NULL)
  {
    timeslots[0].count = timeSlotCount;
   fwrite(timeslots,sizeof(TimeSlot),timeSlotCount,data);
  }
  else
   printf("data file (timeslots.txt) not found");

  fclose(data);
}

void help(){
    printf("List of Available Commands\n");
    printf("1. add - Adds a room to the list of available rooms.\nUsage: add [Building Name] [Room Number]\n\n");
    printf("2. remove - Removes a room from the list of available rooms.\nUsage: remove [Building Name] [Room Number]\n\n");
    printf("3. reserve - Reserves a room from the list of available rooms.\nUsage: reserve [Building Name] [Room Number] [Start Time] [End Time] [Name]\n\n");
    printf("4. cancel - Cancels a reserved room.\nUsage: cancel [Building Name] [Room Number] [Start Time] [End Time] [Name]\n\n");
    printf("5. rooms - Displays a list of available rooms for reservation.\nUsage: rooms\n\n");
    printf("6. timeslots - Displays a list of reserved rooms along with their timeslots and name.\nUsage: timeslots.\n\n");
    printf("7. load - Loads the previously saved booking data.\nUsage: load\n\n");
    printf("8. save - Saves the current booking data.\nUsage: save\n\n");
    printf("9. exit - Exits the application.\nUsage: exit\n\n");
}

void processInstruction(char* instruction) {
    char command[20];
    char buildingName[name_length];
    char roomId[name_length];
    int startTime, endTime;
    char name[64];

    // Using sscanf to parse the command input
    if (sscanf(instruction, "add %s %s", buildingName, roomId) == 2) {
        addRoom(buildingName, roomId);
    } else if (sscanf(instruction, "remove %s %s", buildingName, roomId) == 2) {
        removeRoom(buildingName, roomId);
    } else if (sscanf(instruction, "reserve %s %s %d %d %s", buildingName, roomId, &startTime, &endTime, name) == 5) {
        reserveRoom(buildingName, roomId, startTime, endTime, name);
    } else if (sscanf(instruction, "cancel %s %s %d %d %s", buildingName, roomId, &startTime, &endTime, name) == 5) {
        cancelRoom(buildingName, roomId, startTime, endTime, name);
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
    } else if (strcmp(instruction, "exit") == 0) {
        printf("Exiting the application.\n");
        exit(0); 
    } else {
        printf("Invalid command\n");
    }
}

int main() {
    char instruction[100] = {0};
    while (1) {
        printf("Enter command: ");
        scanf(" %[^\n]", instruction);
        for (int i = 0; instruction[i] != '\0' && instruction[i] != ' '; i++){
            if (instruction[i] >= 65 && instruction[i] <= 90){
                instruction[i] = instruction[i] + 32;
            } 
        }
        processInstruction(instruction);
    }

    return 0;
}   
