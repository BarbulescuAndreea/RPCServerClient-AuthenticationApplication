// find.h
#ifndef FIND_H
#define FIND_H
#include "insertToken.h"

// Function to compare userId
bool compareUserId(userData* user, const char* targetId) {
    return (strncmp(user->userId, targetId, sizeof(user->userId)) == 0);
}

// Function to compare tokenAccess
bool compareTokenAccess(userData* user, const char* targetToken) {
    return (strncmp(user->tokenAccess, targetToken, sizeof(user->tokenAccess)) == 0);
}


// Function to look for a user by userId
userData* UserTokenbyId(const char* IdOfUser, userList** users) {
    for (userData* current = (*users)->head; current != NULL; current = current->next) {
        // Use the compareUserId function for comparison
        if (compareUserId(current, IdOfUser)) {
            // Found the tokenForUserStruct with the given userId
            return current;
        }
    }

    // tokenForUserStruct not found
    return NULL;
}

// Function to look for a user by tokenAccess
userData* lookForUser(const char* tokenOfAccess, userList** listOfUsers) {
    for (userData* current = (*listOfUsers)->head; current != NULL; current = current->next) {
        // Use the compareTokenAccess function for comparison
        if (compareTokenAccess(current, tokenOfAccess)) {
            // Found the tokenForUserStruct with the given tokenAccess
            return current;
        }
    }

    // tokenForUserStruct not found
    return NULL;
}

// Check if a resource from the server input exists in the list of resources 
bool resourceCheck( char* inputResource, char** listOfResources, int nrOfResources) {
    int i = 0;
    bool ret = false;
    while(i < nrOfResources) {
        // Compare the current resource with the target resource
        if(strcmp(listOfResources[i], inputResource) == 0){
            // Resource found
            ret = true;
            return ret;
        }
        i++;
    }
    // Resource not found
    return ret;
}

#endif // FIND_H