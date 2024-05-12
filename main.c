
#include "defs.h"

int main() {
    // Initialize the random number generator
    srand(time(NULL));

    // Create the house: You may change this, but it's here for demonstration purposes
    // Note: This code will not compile until you have implemented the house functions and structures
    HouseType house;
    initHouse(&house);
    populateRooms(&house);

    // Get user input for hunter names
    char hunterNames[HUNTER_COUNT][MAX_NAME];
    for (int i = 0; i < HUNTER_COUNT; i++) {
        printf("Enter name for Hunter %d: ", i + 1);
        scanf("%s", hunterNames[i]);
    }

    // Create the hunters
    HunterType hunters[HUNTER_COUNT];
    for (int i = 0; i < HUNTER_COUNT; i++) {
        initHunter(&hunters[i], hunterNames[i], randomEvidenceType());
        placeHunter(&hunters[i], house.rooms);
    }

    // Create the ghost
    GhostType ghost;
    initGhost(&ghost, randomGhost(), house.rooms);
    placeGhost(&ghost, house.rooms);

    // Create threads for hunters and ghost
    pthread_t hunterThreads[HUNTER_COUNT];
    pthread_t ghostThread;

    for (int i = 0; i < HUNTER_COUNT; i++) {
        pthread_create(&hunterThreads[i], NULL, (void *(*)(void *)) hunterBehavior, &hunters[i], house);
    }

    pthread_create(&ghostThread, NULL, (void *(*)(void *)) ghostBehavior, &ghost, house);

    // Wait for all threads to complete
    for (int i = 0; i < HUNTER_COUNT; i++) {
        pthread_join(hunterThreads[i], NULL);
    }

    pthread_join(ghostThread, NULL);

    // Finalize results
    finalizeResults(&house);

    return 0;
}
