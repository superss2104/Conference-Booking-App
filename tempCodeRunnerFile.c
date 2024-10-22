#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int max_rooms = 100;
const int name_length = 50;
const int max_slots = 500;

typedef struct Room {
    char buildingName[name_length];
    char roomId[name_length];
} Room;

typedef struct TimeSlot {
    char buildingName[name_length];
    char roomId[name_length];
    int startTime;
    int endTime;
} TimeSlot;

Room rooms[max_rooms];
int roomCount = 0;

TimeSlot timeslots[max_slots];
int timeSlotCount = 0;

void addRoom(const char* buildingName, const char* roomId) {
    strcpy(rooms[roomCount].buildingName, buildingName);
    strcpy(rooms[roomCount].roomId, roomId);
    roomCount++;
    printf("Room added: %s %s\n", buildingName, roomId);
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

void reserveRoom(const char* buildingName, const char* roomId, int startTime, int endTime) {
    // Check if the requested time slot overlaps with any existing reservation
    if(startTime >= 8 && endTime <= 24 ){
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
    timeslots[timeSlotCount].startTime = startTime;
    timeslots[timeSlotCount].endTime = endTime;
    timeSlotCount++;

    printf("Reserved %s %s from %d to %d\n", buildingName, roomId, startTime, endTime);
    }
    else printf("Invalid Time, Booking is allowed only between 8 and 24.\n");
}




void cancelRoom(const char* buildingName, const char* roomId, int startTime, int endTime) {
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
            printf("Canceled %s %s from %d to %d\n", buildingName, roomId, startTime, endTime);
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
        printf("%s %s from %d to %d\n", timeslots[i].buildingName, timeslots[i].roomId,
               timeslots[i].startTime, timeslots[i].endTime);
    }
}

// Helper function to parse the user command
void processInstruction(char* instruction) {
    char command[20];
    char buildingName[name_length];
    char roomId[name_length];
    int startTime, endTime;

    // Using sscanf to parse the command input
    if (sscanf(instruction, "add %s %s", buildingName, roomId) == 2) {
        addRoom(buildingName, roomId);
    } else if (sscanf(instruction, "remove %s %s", buildingName, roomId) == 2) {
        removeRoom(buildingName, roomId);
    } else if (sscanf(instruction, "reserve %s %s %d %d", buildingName, roomId, &startTime, &endTime) == 4) {
        reserveRoom(buildingName, roomId, startTime, endTime);
    } else if (sscanf(instruction, "cancel %s %s %d %d", buildingName, roomId, &startTime, &endTime) == 4) {
        cancelRoom(buildingName, roomId, startTime, endTime);
    } else if (strcmp(instruction, "rooms") == 0) {
        displayRooms();
    } else if (strcmp(instruction, "timeslots") == 0) {
        displayTimeSlots();
    } else if (strcmp(instruction, "exit") == 0) {
        printf("Exiting the application.\n");
        exit(0);
    } else {
        printf("Invalid command\n");
    }
}

int main() {
    char instruction[100];

    while (1) {
        printf("Enter command: ");
        fgets(instruction, sizeof(instruction), stdin);
        instruction[strlen(instruction) - 1] = '\0';  // Remove newline character
        processInstruction(instruction);
    }

    return 0;
} 