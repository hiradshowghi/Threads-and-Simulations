#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_STR         64
#define MAX_RUNS        50
#define BOREDOM_MAX     100
#define C_TRUE          1
#define C_FALSE         0
#define HUNTER_WAIT     5000
#define GHOST_WAIT      600
#define NUM_HUNTERS     4
#define FEAR_MAX        10
#define LOGGING         C_TRUE

typedef enum EvidenceType EvidenceType;
typedef enum GhostClass GhostClass;
typedef enum LoggerDetails LoggerDetails;

enum EvidenceType { EMF, TEMPERATURE, FINGERPRINTS, SOUND, EV_COUNT, EV_UNKNOWN };
enum GhostClass { POLTERGEIST, BANSHEE, BULLIES, PHANTOM, GHOST_COUNT, GH_UNKNOWN };
enum LoggerDetails { LOG_FEAR, LOG_BORED, LOG_EVIDENCE, LOG_SUFFICIENT, LOG_INSUFFICIENT, LOG_UNKNOWN };

// Helper Utilies
int randInt(int,int);        // Pseudo-random number generator function
float randFloat(float, float);  // Pseudo-random float generator function
enum GhostClass randomGhost();  // Return a randomly selected a ghost type
void ghostToString(enum GhostClass, char*); // Convert a ghost type to a string, stored in output paremeter
void evidenceToString(enum EvidenceType, char*); // Convert an evidence type to a string, stored in output parameter

// Logging Utilities
void l_hunterInit(char* name, enum EvidenceType equipment);
void l_hunterMove(char* name, char* room);
void l_hunterReview(char* name, enum LoggerDetails reviewResult);
void l_hunterCollect(char* name, enum EvidenceType evidence, char* room);
void l_hunterExit(char* name, enum LoggerDetails reason);
void l_ghostInit(enum GhostClass type, char* room);
void l_ghostMove(char* room);
void l_ghostEvidence(enum EvidenceType evidence, char* room);
void l_ghostExit(enum LoggerDetails reason);

// Data Definitions
typedef struct Room Room;
typedef struct RoomNode RoomNode;
typedef struct House House;
typedef struct Ghost Ghost;
typedef struct Hunter Hunter;
typedef struct Evidence Evidence;

// Evidence Structure
struct Evidence {
    EvidenceType type;
    sem_t semaphore;  // Semaphore to protect evidence modifications
};

// Room Structure
struct Room {
    char name[MAX_STR];
    RoomNode* connectedRooms;  // Linked list of connected rooms
    EvidenceType* evidenceCollection;  // Linked list of evidence in the room
    Hunter* hunters;  // Collection of hunters in the room
    Ghost* ghost;  // Pointer to the ghost in the room
    sem_t semaphore;  // Semaphore to protect room modifications
};

// RoomNode Structure
struct RoomNode {
    Room* room;
    RoomNode* next;
};

// House Structure
struct House {
    RoomNode* rooms;  // Linked list of rooms in the house
    Hunter* hunters;  // Collection of hunters in the house
    Evidence* sharedEvidence;  // Shared evidence collection
};

// Ghost Structure
struct Ghost {
    GhostClass type;
    Room* currentRoom;
    int boredomTimer;
    sem_t actionSemaphore;  // Semaphore to protect ghost actions
};

// Hunter Structure
struct Hunter {
    char name[MAX_STR];
    Room* currentRoom;
    EvidenceType equipment;
    Evidence* sharedEvidence;
    int fear;
    int boredom;
    sem_t actionSemaphore;  // Semaphore to protect hunter actions
};

// Function Prototypes

// House Functions
void initHouse(House* house);
void populateRooms(House* house);

// Room Functions
Room* createRoom(char* name);
void connectRooms(Room* room1, Room* room2);
void addRoom(RoomNode** roomList, Room* room);

// Ghost Functions
void initGhost(Ghost* ghost, GhostClass type, Room* room);
void moveGhost(Ghost* ghost, House* house);
void leaveEvidence(Ghost* ghost, Room* room, Evidence* sharedEvidence);

// Hunter Functions
void initHunter(Hunter* hunter, char* name, EvidenceType equipment, Room* startingRoom, Evidence* sharedEvidence);
void moveHunter(Hunter* hunter, House* house);
void collectEvidence(Hunter* hunter, Room* room, Evidence* sharedEvidence);
void reviewEvidence(Hunter* hunter, Evidence* sharedEvidence);
int checkEvidence(Evidence* sharedEvidence);