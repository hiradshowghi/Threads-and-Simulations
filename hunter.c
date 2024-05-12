#include "defs.h"

void hunterBehavior(Hunter* hunter, HouseType* house) {
    // Log hunter initialization
    l_hunterInit(hunter->name, hunter->evidenceType);

    while (1) {
        sem_wait(&hunter->actionSemaphore);

        // Check fear and boredom
        if (hunter->fear >= FEAR_MAX) {
            l_hunterExit(hunter->name, LOG_FEAR);
            sem_post(&hunter->actionSemaphore);
            pthread_exit(NULL);
        } else if (hunter->boredom >= BOREDOM_MAX) {
            l_hunterExit(hunter->name, LOG_BORED);
            sem_post(&hunter->actionSemaphore);
            pthread_exit(NULL);
        }

        // Implement hunter behavior: move to a room, collect evidence, review evidence, exit
        int action = randInt(0, 3);
        switch (action) {
            case 0:
                // Move to a random, connected room
                moveHunter(hunter, house);
                l_hunterMove(hunter->name, hunter->currentRoom->name);
                break;
            case 1:
                // Collect evidence in the current room
                if (collectEvidence(hunter, hunter->currentRoom, house->sharedEvidence)) {
                    l_hunterCollect(hunter->name, hunter->evidenceType, hunter->currentRoom->name);
                }
                break;
            case 2:
                // Review evidence
                if (reviewEvidence(hunter, house->sharedEvidence)) {
                    l_hunterReview(hunter->name, LOG_SUFFICIENT);
                } else {
                    l_hunterReview(hunter->name, LOG_INSUFFICIENT);
                }
                break;
            case 3:
                // Exit the thread
                l_hunterExit(hunter->name, LOG_EVIDENCE);
                sem_post(&hunter->actionSemaphore);
                pthread_exit(NULL);
                break;
            default:
                break;
        }

        // Increment boredom
        hunter->boredom++;

        // Check if enough evidence is collected
        if (checkEvidence(house->sharedEvidence)) {
            l_hunterExit(hunter->name, LOG_EVIDENCE);
            sem_post(&hunter->actionSemaphore);
            pthread_exit(NULL);
        }

        sem_post(&hunter->actionSemaphore);

        // Sleep or use some delay to simulate time passing
        usleep(HUNTER_WAIT);
    }
}
