// database.h

#ifndef DATABASE_H
#define DATABASE_H
#include <stdbool.h>

typedef struct {
    char action[20];
    char permission[20];
} PermissionEntry;

// Node for a linked list of PermissionEntry
typedef struct PermissionNode {
    PermissionEntry data;
    struct PermissionNode* next;
} PermissionNode;

// Node for a queue containing a list of PermissionEntry nodes
typedef struct QueueNode {
    PermissionNode* data;  // Head of the list
    struct QueueNode* next;
} QueueNode;

// Simple queue structure
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} Queue;

struct UserPermissions {
    char userId[15];
    PermissionNode* data;
};

struct UserPermissionsNode {
    struct UserPermissions data;
    struct UserPermissionsNode* next;
};

// Define a linked list structure to hold UserPermissions entries
struct UserPermissionsList {
    struct UserPermissionsNode* head;
};

// Function to initialize users from a file
char** databaseInitUsers(char* usersDatabaseFile);

// Function to initialize resources from a file
char** databaseInitResources(char* resourcesDBFile);

// Function to read permissions from a file
Queue readPermissionsFile(char *permissionsFile);

void initUserPermissionsList(struct UserPermissionsList** userList);

void initQueue(Queue* queue);

void enqueue(Queue* queue, PermissionNode* data) ;

int isEmpty(Queue* queue);

void dequeue(Queue* queue, PermissionNode** result);

Queue readPermissionsFile(char *permissionsFile);

void addUserPermissions(struct UserPermissionsList* userList, const char* userId, struct PermissionNode* permissions);

struct UserPermissions* findUserPermissions(struct UserPermissionsList* userList, const char* userId);

bool containsChar(const char* str, char ch);

bool verifyPermissions(PermissionNode* permissionNode, char* resource, char* action);

void iterateUserPermissionsList(struct UserPermissionsList* userList);

#endif  // DATABASE_H