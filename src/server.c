#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../headers/database.h"
#include "../headers/userTokenfunctions/find.h"
#include "../headers/userTokenfunctions/insertToken.h"
#include "../resourcemanager.h"
#include "../token.h"

#define SERVER_PORT 8080
#define userCount 5
#define resourcesCount 5

// Contors and databases declaration
int times = 0;
bool is_full = false;
struct UserTokenList* userTokenDb;
struct UserPermissionsList* userPermissionsDb;

// Get the data from resourcemanager_svc.c
extern char** listOfUsers;
extern Queue permiss;
extern int remainingOp;
extern char** listOfResources;

Response_authentication* user_auth_1_svc(Request_user_authentication* request, struct svc_req *client){
    struct UserTokenList* userTokenCopy;
    // Initialise data base - it exists in all the function because we can have any type of request at the begining from the client
    // and we need to do that no matter what function the client calls from the server
    if(is_full == false){
        TokenDatabase(&userTokenDb);
        initUserPermissionsList(&userPermissionsDb);
        is_full = true;
    }
    Response_authentication* resp = malloc(sizeof(resp));
    
    // Verify if the username from the request is in the list of usernames of server
    fprintf(stdout, "BEGIN %s AUTHZ\n", request->userId);
    resp->authToken = "USER_NOT_FOUND";
    for (int i = 0; i < userCount; ++i) {
        if(strcmp(listOfUsers[i], request->userId) == 0){
            resp->authToken = generate_access_token(request->userId);
            printf("  RequestToken = %s\n", resp->authToken);
            break;
        }
    }
    // If the username is saved in the DataBase but doesn't have an access token saved or isn't saved in the DataBase
    // at all then add it in the DataBase.
    struct tokenForUserStruct *userToken = UserTokenbyId( request->userId, &userTokenDb);
    if(userToken != NULL){
        if(strcmp(userToken->tokenAccess, "") == 0){
            insertTokenForUser( request->userId, resp->authToken, &userTokenDb, atoi(request->resource));
        }
    }
    else{
        insertTokenForUser(request->userId, resp->authToken, &userTokenDb, atoi(request->resource));
    }
    return resp;
}

Response_user_access* user_acc_1_svc(Request_user_access* request, struct svc_req *client){
    // Initialise data base - it exists in all the function because we can have any type of request at the begining from the client
    // and we need to do that no matter what function the client calls from the server
    if(is_full == false){
        TokenDatabase(&userTokenDb);
        initUserPermissionsList(&userPermissionsDb);
        is_full = true;
    }
    char access_token[30];
    Response_user_access* resp = malloc(sizeof(Response_user_access));
    struct tokenForUserStruct *userToken = UserTokenbyId( request->userId,&userTokenDb);
    PermissionNode* result;
    // For every access request, i need to get the first one(FIFO) from the permissions list to match the user request
    dequeue(&permiss, &result);
    // If the user doesn t have permissions (*, -) then the Request is denied
    if((strcmp(result->data.action, "*") == 0) && (strcmp(result->data.permission, "-") == 0)){
        resp->message = "REQUEST_DENIED";
        resp->accessToken = NULL;
        resp->refreshToken = NULL;
        resp->opNumber = -2;
    }
    else{
        // If the user has permissions, i generate the access token and set the remainingOp with the given remainingOp at that moment
        resp->accessToken = generate_access_token(request->authToken);
        resp->opNumber = remainingOp;
        resp->message = "Done!"; // dummy message in case it doesn t return REQUEST_DENIED
        printf("  AccessToken = %s\n", resp->accessToken);
        if(request->mode == 1){
            resp->refreshToken = generate_access_token(resp->accessToken);
            printf("  RefreshToken = %s\n", resp->refreshToken);
        }else{
            resp->refreshToken = "withoutRefresh";
        }
        // Updating in Database the generated access token and the number of remaning operations
        strcpy(userToken->tokenAccess, resp->accessToken);
        userToken->remaining_ops = resp->opNumber;
        // If an user is associated with a permission, i just change the permission =, otherwise i need to add the user in the list of 
        // user-permissions
        struct UserPermissions* permissionNode = findUserPermissions(userPermissionsDb, userToken->userId);
        if(permissionNode == NULL){
            addUserPermissions(userPermissionsDb, userToken->userId, result);
        }
        else{
            permissionNode->data = result;
        }
    }
    return resp;
}

Response_user_validate* user_validate_1_svc(Request_user_validate* request, struct svc_req *client) {
    // Initialise data base - it exists in all the function because we can have any type of request at the begining from the client
    // and we need to do that no matter what function the client calls from the server
    if(is_full == false){
        TokenDatabase(&userTokenDb);
        initUserPermissionsList(&userPermissionsDb);
        is_full = true;
    }
    // Allocate memory for the Response structure
    Response_user_validate* response_quick = calloc(1, sizeof(response_quick));
    response_quick->info = "PERMISSION_DENIED";
    if (response_quick == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    // // Find the tokenForUserStruct using the lookForUser function
    userData* userToken = lookForUser(request->accessToken, &userTokenDb);

    // // Allocate memory for the token
    struct UserPermissions* permissionNode = NULL;
    if(userToken != NULL){
        // Find the permissions for a user in data base
        permissionNode = findUserPermissions(userPermissionsDb, userToken->userId);
    }
    if (userToken == NULL) {
        // If the user has no permissions, then the attemp will be denied
        response_quick->info = "PERMISSION_DENIED";
        printf("DENY (%s,%s,%s,0)\n", request->action, request->resource, request->accessToken);
        return response_quick;
    }
    else if(userToken->remaining_ops == 0){
        // If the user don t have any remaining operations left (the last parameter of the server), then the token is expired 
        // and the action will not be performed
        response_quick->info = "TOKEN_EXPIRED";
        printf("DENY (%s,%s,,%d)\n", request->action, request->resource,  userToken->remaining_ops);
    }
    else if(resourceCheck(request->resource, listOfResources, resourcesCount) == false){
        // If the resource given from the user doesn t exist in the list of existing listOfResources given at the input
        // then the corresponding message is returned
        userToken->remaining_ops--;
        response_quick->info = "RESOURCE_NOT_FOUND";
        printf("DENY (%s,%s,%s,%d)\n", request->action, request->resource, request->accessToken,  userToken->remaining_ops);
    }
    else if(verifyPermissions(permissionNode->data, request->resource, request->action) == false){
        // If an user doesn t have the right permissions to do a certain action, the attemp will be denied
        userToken->remaining_ops--;
        response_quick->info = "OPERATION_NOT_PERMITTED";
        printf("DENY (%s,%s,%s,%d)\n", request->action, request->resource, request->accessToken, userToken->remaining_ops);
    }
    else {
        // The user has everything in order to perform the action
        userToken->remaining_ops--;
        response_quick->info = "PERMISSION_GRANTED";
        printf("PERMIT (%s,%s,%s,%d)\n", request->action, request->resource, request->accessToken,  userToken->remaining_ops);
    }
    if (response_quick->info == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    return response_quick;
}

