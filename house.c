#include "defs.h"

/*
    Dynamically allocates several rooms and populates the provided house.
    Note: You may modify this as long as room names and connections are maintained.
        out: house - the house to populate with rooms. Assumes house has been initialized.
*/
void populateRooms(HouseType* house) {
    // First, create each room

    // createRoom assumes that we dynamically allocate a room, initializes the values, and returns a RoomType*
    // create functions are pretty typical, but it means errors are harder to return aside from NULL
    struct Room* van                = createRoom("Van");
    struct Room* hallway            = createRoom("Hallway");
    struct Room* master_bedroom     = createRoom("Master Bedroom");
    struct Room* boys_bedroom       = createRoom("Boy's Bedroom");
    struct Room* bathroom           = createRoom("Bathroom");
    struct Room* basement           = createRoom("Basement");
    struct Room* basement_hallway   = createRoom("Basement Hallway");
    struct Room* right_storage_room = createRoom("Right Storage Room");
    struct Room* left_storage_room  = createRoom("Left Storage Room");
    struct Room* kitchen            = createRoom("Kitchen");
    struct Room* living_room        = createRoom("Living Room");
    struct Room* garage             = createRoom("Garage");
    struct Room* utility_room       = createRoom("Utility Room");

    // This adds each room to each other's room lists
    // All rooms are two-way connections
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);

    // Add each room to the house's room list
    addRoom(&house->rooms, van);
    addRoom(&house->rooms, hallway);
    addRoom(&house->rooms, master_bedroom);
    addRoom(&house->rooms, boys_bedroom);
    addRoom(&house->rooms, bathroom);
    addRoom(&house->rooms, basement);
    addRoom(&house->rooms, basement_hallway);
    addRoom(&house->rooms, right_storage_room);
    addRoom(&house->rooms, left_storage_room);
    addRoom(&house->rooms, kitchen);
    addRoom(&house->rooms, living_room);
    addRoom(&house->rooms, garage);
    addRoom(&house->rooms, utility_room);
}

// Function to connect two rooms
void connectRooms(Room* room1, Room* room2) {
    sem_wait(&room1->semaphore);
    sem_wait(&room2->semaphore);

    // Connect room1 to room2
    RoomNode* newNode1 = createRoomNode(room2);
    RoomNode* newNode2 = createRoomNode(room1);
    addRoomNode(&room1->connectedRooms, newNode1);
    addRoomNode(&room2->connectedRooms, newNode2);

    sem_post(&room2->semaphore);
    sem_post(&room1->semaphore);
}

// Function to initialize a house
void initHouse(HouseType* house) {
    house->hunters = NULL;
    house->rooms = NULL;
    house->sharedEvidence = NULL;
}

// Function to append a room to the house's room list
void addRoomToHouse(HouseType* house, Room* room) {
    RoomNode* newNode = createRoomNode(room);
    addRoomNode(&house->rooms, newNode);
}

void initGhost(HouseType* house) {
    house->ghost.currentRoom = getRandomRoom(house);
    house->ghost.boredomTimer = 0;
    sem_init(&house->ghost.actionSemaphore, 0, 1);
}

// Function to finalize results after all threads have completed
void finalizeResults(HouseType* house) {
    printf("\nFinal Results:\n");

    // List all hunters with fear >= MAX_FEAR and boredom >= MAX_BOREDOM
    printf("Hunters with fear >= MAX_FEAR:\n");
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (house->hunters[i].fear >= MAX_FEAR) {
            printf("[%s] Fear: %d\n", house->hunters[i].name, house->hunters[i].fear);
        }
    }

    printf("\nHunters with boredom >= MAX_BOREDOM:\n");
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (house->hunters[i].boredom >= MAX_BOREDOM) {
            printf("[%s] Boredom: %d\n", house->hunters[i].name, house->hunters[i].boredom);
        }
    }

    // Check if the ghost has won
    int ghostWon = 1;
    for (int i = 0; i < NUM_HUNTERS; i++) {
        if (house->hunters[i].fear < MAX_FEAR && house->hunters[i].boredom < MAX_BOREDOM) {
            ghostWon = 0;
            break;
        }
    }

    if (ghostWon) {
        printf("\nThe ghost has won! All hunters have high fear or boredom.\n");
        return;
    }

    // Print all evidence collected by the hunters
    printf("\nEvidence Collected:\n");
    for (int i = 0; i < NUM_HUNTERS; i++) {
        printf("[%s] Evidence: ", house->hunters[i].name);
        for (int j = 0; j < house->hunters[i].evidenceCount; j++) {
            printf("%s ", evidenceStrings[house->hunters[i].evidence[j]]);
        }
        printf("\n");
    }

    // Check if three different pieces of evidence were collected
    int uniqueEvidenceCount = countUniqueEvidence(house->sharedEvidence);
    if (uniqueEvidenceCount >= 3) {
        printf("\nThe ghost has been identified!\n");

        // Print the ghost's type
        printf("Ghost Type: ");
        ghostToString(house->ghost.type, house->ghostTypeString);
        printf("%s\n", house->ghostTypeString);

        // Check if the pieces of evidence correctly identified the ghost
        if (isCorrectGhostIdentification(house->sharedEvidence, house->ghost.type)) {
            printf("The identification was correct!\n");
        } else {
            printf("The identification was incorrect.\n");
        }
    } else {
        printf("\nInsufficient evidence to identify the ghost.\n");
    }
}




