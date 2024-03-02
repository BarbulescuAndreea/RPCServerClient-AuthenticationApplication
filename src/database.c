#include "../headers/database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int userCount;
int resourcesCount;

char** databaseInitUsers(char* usersDatabaseFile) {
    FILE* userIDfile = fopen(usersDatabaseFile, "r");
    char** userIds = NULL;
    if (userIDfile == NULL) {
        fprintf(stderr, "Error opening file: %s\n", usersDatabaseFile);
        exit(EXIT_FAILURE);
    }
    char line[256];  // Adjust the size based on your needs
    if (fgets(line, sizeof(line), userIDfile) != NULL) {
        line[strcspn(line, "\n")] = '\0';  // Remove the newline character

        // Convert the first line to an integer to get the userCount
        int userCount = atoi(line);
        userIds = (char**)malloc(userCount * sizeof(char*));

        // Allocate memory for user IDs
        if (userIds == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        // Read and store user IDs
        for (int i = 0; i < userCount; ++i) {
            if (fgets(line, sizeof(line), userIDfile) != NULL) {
                line[strcspn(line, "\n")] = '\0';  // Remove the newline character
                userIds[i] = strdup(line);

                if (userIds[i] == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error reading user ID from file\n");
                exit(EXIT_FAILURE);
            }
        }

        // Now userIds contains the user IDs, and userCount has the total number of user IDs
        // You can use userIds and userCount as needed
    } else {
        fprintf(stderr, "Error reading user count from file\n");
        exit(EXIT_FAILURE);
    }
    // Close the file when done
    fclose(userIDfile);
    return userIds;
}

char** databaseInitResources(char* resourcesDBFile){
    FILE* resourcesfile = fopen(resourcesDBFile, "r");
    char** resources = NULL;

    if (resourcesfile == NULL) {
        fprintf(stderr, "Error opening file: %s\n", resourcesDBFile);
        exit(EXIT_FAILURE);
    }

    char line[256];  // Adjust the size based on your needs
    if (fgets(line, sizeof(line), resourcesfile) != NULL) {
        line[strcspn(line, "\n")] = '\0';  // Remove the newline character

        // Convert the first line to an integer to get the userCount
        int resourcesCount = atoi(line);

        // Allocate memory for user IDs
        resources = (char**)malloc(resourcesCount * sizeof(char*));

        if (resources == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }

        // Read and store user IDs
        for (int i = 0; i < resourcesCount; ++i) {
            if (fgets(line, sizeof(line), resourcesfile) != NULL) {
                line[strcspn(line, "\n")] = '\0';  // Remove the newline character
                resources[i] = strdup(line);

                if (resources[i] == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    exit(EXIT_FAILURE);
                }
            } else {
                fprintf(stderr, "Error reading user ID from file\n");
                exit(EXIT_FAILURE);
            }
        }
        // Now userIds contains the user IDs, and userCount has the total number of user IDs
        // You can use userIds and userCount as needed
    } else {
        fprintf(stderr, "Error reading user count from file\n");
        exit(EXIT_FAILURE);
    }

    // Close the file when done
    fclose(resourcesfile);
    return resources;
}

// Initialize an empty queue
void initQueue(Queue* queue) {
    queue->front = queue->rear = NULL;
}

void enqueue(Queue* queue, PermissionNode* data) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = data;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
}

// Check if the queue is empty
int isEmpty(Queue* queue) {
    return queue->front == NULL;
}

// Corrected dequeue function
void dequeue(Queue* queue, PermissionNode** result) {
    if (queue->front == NULL) {
        fprintf(stderr, "Queue is empty\n");
        exit(EXIT_FAILURE);
    }
    QueueNode* temp = queue->front;

    *result = temp->data; // Assign the structure directly (no need to dereference)

    queue->front = temp->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
}

// Corrected readPermissionsFile function
Queue readPermissionsFile(char *permissionsFile) {
    FILE *file = fopen(permissionsFile, "r");

    if (file == NULL) {
        fprintf(stderr, "Error opening the file\n");
        exit(EXIT_FAILURE);
    }

    char line[512];
    Queue queue;
    initQueue(&queue);

    while (fgets(line, sizeof(line), file) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (strlen(line) == 0) {
            continue;
        }

        PermissionNode* permissions = NULL;
        PermissionNode* tail = NULL;
        char *token = strtok(line, ",");
        while (token != NULL) {
            PermissionNode* newNode = (PermissionNode*)malloc(sizeof(PermissionNode));
            if (newNode == NULL) {
                fprintf(stderr, "Memory allocation error\n");
                exit(EXIT_FAILURE);
            }

            strcpy(newNode->data.action, token);

            token = strtok(NULL, ",");
            if (token != NULL) {
                strcpy(newNode->data.permission, token);
            }

            newNode->next = NULL;

            if (permissions == NULL) {
                permissions = tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }

            token = strtok(NULL, ",");
        }

        enqueue(&queue, permissions);
    }

    fclose(file);
    return queue;
}

// Initialize an empty UserPermissionsList
void initUserPermissionsList(struct UserPermissionsList** userList) {
    *userList = (struct UserPermissionsList*)malloc(sizeof(struct UserPermissionsList));
    if (*userList == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    (*userList)->head = NULL;
}

// Add a new UserPermissions entry to the UserPermissionsList
void addUserPermissions(struct UserPermissionsList* userList, const char* userId, struct PermissionNode* permissions) {
    // Create a new UserPermissions node
    struct UserPermissionsNode* newUserPermissionsNode = (struct UserPermissionsNode*)malloc(sizeof(struct UserPermissionsNode));
    if (newUserPermissionsNode == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Copy the userId and permissions to the new node
    strncpy(newUserPermissionsNode->data.userId, userId, sizeof(newUserPermissionsNode->data.userId));
    newUserPermissionsNode->data.userId[sizeof(newUserPermissionsNode->data.userId)] = '\0';  // Ensure null-terminated
    newUserPermissionsNode->data.data = permissions;

    // Add the new node to the beginning of the list
    newUserPermissionsNode->next = userList->head;
    userList->head = newUserPermissionsNode;
}

// Find a UserPermissions entry by userId in the UserPermissionsList
struct UserPermissions* findUserPermissions(struct UserPermissionsList* userList, const char* userId) {
    struct UserPermissionsNode* current = userList->head;
    while (current->data.userId != NULL) {
        if (strcmp(current->data.userId, userId) == 0) {
            return &current->data;  // Found the UserPermissions with the given userId
        }
        current = current->next;
    }
    return NULL;  // UserPermissions not found
}

bool containsChar(const char* str, char ch) {
    // Iterate through the string to check if the character is present
    while (*str != '\0') {
        if (*str == ch) {
            return true;
        }
        ++str;
    }
    return false;
}


bool verifyPermissions(PermissionNode* permissionNode, char* resource, char* action) {
    while (permissionNode != NULL) {
        // Check if the current node has the desired resource
        if (strcmp(permissionNode->data.action, resource) == 0) {
            // Resource and action match
            if (strcmp(action, "INSERT") == 0) {
                return containsChar(permissionNode->data.permission, 'I');
            } else if (strcmp(action, "MODIFY") == 0) {
                return containsChar(permissionNode->data.permission, 'M');
            } else if (strcmp(action, "DELETE") == 0) {
                return containsChar(permissionNode->data.permission, 'D');
            } else if (strcmp(action, "READ") == 0) {
                return containsChar(permissionNode->data.permission, 'R');
            } else if (strcmp(action, "EXECUTE") == 0) {
                return containsChar(permissionNode->data.permission, 'X');
            } else {
                return false;
            }
        }
        // Move to the next node in the linked list
        permissionNode = permissionNode->next;
    }
    // Resource not found in the linked list
    return false;
}

void iterateUserPermissionsList(struct UserPermissionsList* userList) {
    struct UserPermissionsNode* current = userList->head;

    while (current != NULL) {
        printf("UserId: %s\n", current->data.userId);

        struct PermissionNode* permissionNode = current->data.data;
        while (permissionNode != NULL) {
            printf("Action: %s, Permission: %s\n", permissionNode->data.action, permissionNode->data.permission);
            permissionNode = permissionNode->next;
        }

        current = current->next;
    }
}