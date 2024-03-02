// insertToken.h
#include <stdbool.h>
#ifndef INSERTTOKEN_H
#define INSERTTOKEN_H
#define USER_TOKEN_SIZE 76

// Define the tokenForUserStruct struct
typedef struct tokenForUserStruct {
    struct tokenForUserStruct* next;
    char tokenAccess[20];
    char tokenAuthent[20];
    char tokenReference[20];
    char userId[20];
    int remaining_ops;
    int mode;
} userData;

// Define a linked list structure to hold tokenForUserStruct entries
typedef struct UserTokenList {
    userData* head;
} userList;

// Create a new, empty UserTokenList
void TokenDatabase(userList** userListtt) {
    *userListtt = (userList*)malloc(USER_TOKEN_SIZE);
    (*userListtt)->head = NULL;
}


void initAuthToken(userData* newUserToken, char* token){
    memcpy(newUserToken->tokenAuthent, token, sizeof(newUserToken->tokenAuthent) - 1);
    newUserToken->tokenAuthent[sizeof(newUserToken->tokenAuthent) - 1] = '\0';
}

void initAccToken(userData* newUserToken, char* token){
    memcpy(newUserToken->tokenAccess, token, sizeof(newUserToken->tokenAccess) - 1);
    newUserToken->tokenAccess[sizeof(newUserToken->tokenAccess) - 1] = '\0';
}

void initRefToken(userData* newUserToken, char* token){
    memcpy(newUserToken->tokenReference, token, sizeof(newUserToken->tokenReference) - 1);
    newUserToken->tokenReference[sizeof(newUserToken->tokenReference) - 1] = '\0';
}

void setMode(userData* newUserToken, int mode){
    newUserToken->mode = mode;
}

// Insert a new tokenForUserStruct into the UserTokenList
void insertTokenForUser(char* IdOfUser, char* token, userList** listOfUser, int mode) {
    // Create a new tokenForUserStruct node
    userData* newUserToken = (userData*)malloc(sizeof(userData));
    // Copy the userId and token to the new node
    memcpy(newUserToken->userId, IdOfUser, sizeof(newUserToken->userId) - 1);
    newUserToken->userId[sizeof(newUserToken->userId) - 1] = '\0';  // Ensure null-terminated
    // Init authent token
    initAuthToken(newUserToken, token);
    // Init access token
    memcpy(newUserToken->tokenAccess, "", sizeof("") - 1);

    *(int*)&newUserToken->remaining_ops = -1;
    setMode(newUserToken, mode);
    // Add the new node to the beginning of the list
    newUserToken->next = NULL;
    (*listOfUser)->head = newUserToken;
}

void insertTokensForUser( char* auth_token, char* acc_token, char* ref_token, userList** IdOfUsers, char* IdOfUser, int mode, int remaining_op) {
    // Create a new tokenForUserStruct node
    userData* newUserToken = malloc(sizeof(*newUserToken));
    // Copy the userId and token to the new node
    memcpy(newUserToken->userId, IdOfUser, sizeof(newUserToken->userId) - 1);
    newUserToken->userId[sizeof(newUserToken->userId) - 1] = '\0';

    // Copy tokenAuthent and ensure null-terminated
    initAuthToken(newUserToken, auth_token);

    // Copy tokenAccess and ensure null-terminated
    initAccToken(newUserToken, acc_token);

    // Copy tokenReference and ensure null-terminated
    initRefToken(newUserToken, ref_token);

    setMode(newUserToken, mode);
    newUserToken->remaining_ops = remaining_op;

    // Add the new node to the beginning of the list
    newUserToken->next = ((*IdOfUsers)->head = newUserToken);
}

#endif // INSERTTOKEN_H
