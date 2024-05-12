#include "defs.h"

void ghostBehavior(Ghost* ghost, HouseType* house) {
    // Log ghost initialization
    l_ghostInit(getGhostClass(ghost), ghost->currentRoom->name);

    while (1) {
        sem_wait(&ghost->actionSemaphore);

        // Check boredom
        if (ghost->boredomTimer >= BOREDOM_MAX) {
            l_ghostExit(LOG_BORED);
            sem_post(&ghost->actionSemaphore);
            pthread_exit(NULL);
        }

        // Implement ghost behavior: do nothing, leave evidence, move to another room
        int action = randInt(0, 3);
        switch (action) {
            case 0:
                // Do nothing
                l_ghostMove(ghost->currentRoom->name);
                break;
            case 1:
                // Leave evidence
                leaveEvidence(ghost, ghost->currentRoom, house->sharedEvidence);
                l_ghostEvidence(getRandomEvidenceType(), ghost->currentRoom->name);
                break;
            case 2:
                // Move to another room
                moveGhost(ghost, house);
                l_ghostMove(ghost->currentRoom->name);
                break;
            default:
                break;
        }

        // Increment boredom if no hunter is in the room
        if (ghost->currentRoom->hunters == NULL) {
            ghost->boredomTimer++;
        } else {
            // Reset boredom if a hunter is in the room
            ghost->boredomTimer = 0;
        }

        sem_post(&ghost->actionSemaphore);

        // Sleep or use some delay to simulate time passing
        usleep(GHOST_WAIT);
    }
}
