#include <stdio.h>
#include <rpc/rpc.h>
#include "../token.h"
#include "../resourcemanager.h"
#include "../headers/userTokenfunctions/find.h"
#include "../headers/userTokenfunctions/insertToken.h"
#include "../headers/allocator_mem.h"

#define USER_ID_LEN 15
#define REQUEST_AUTH_SIZE sizeof(Request_user_authentication*)
#define REQUEST_USER_ACCESS sizeof(Request_user_access*)
#define REQUEST_USER_VALIDATE sizeof(Request_user_validate*)
#define SERVER_NAME "localhost"
#define PROT "udp"

struct UserTokenList* databaseForUsTok; 

int main(int argc, char **argv){
    Request_user_authentication* authReq = malloc(REQUEST_AUTH_SIZE);
    Request_user_access* accessReq = malloc(REQUEST_USER_ACCESS);
    Request_user_validate* valReq = malloc(REQUEST_USER_VALIDATE);
    // Allocate memory for the fields in requests
    allocateRequestUserAuth(authReq);
    allocateRequestUserAccess(accessReq);
    allocateRequestUserValidate(valReq);

    Response_authentication *authResp;
    Response_user_access *accResp;
    Response_user_validate *valResp;
    // Create the client and connect it to the server - localhost
    CLIENT *client = clnt_create(SERVER_NAME, RESOURCE_MANAGER, RESOURCE_MANAGER_VERSION, PROT);
    // Open the clients file where i have the users and the actions
    FILE *clientinfile = fopen(argv[2], "r");
    if(!clientinfile)
        exit(1);
    char line[256];
    char *tokens[3];
    char *context = NULL;
    TokenDatabase(&databaseForUsTok);
    while (fgets(line, sizeof(line), clientinfile) != NULL) {
        // Take the userId from the request
        char *aux = strtok(line, ",");
        // Put the userId in the request
        memset(authReq->userId, 0, sizeof(authReq->userId));  // Initialize the memory to 0
        memcpy(authReq->userId, aux, strlen(aux));
        // Continue taking the string from where it was lefted
        aux = strtok(NULL, ",");
        memset(authReq->action, 0, sizeof(authReq->action));
        memcpy(authReq->action, aux, strlen(aux));
        // Take the resource
        aux = strtok(NULL, ",");
        // Copy the resource in the request
        strcpy(authReq->resource, aux);

        memcpy(accessReq->userId, authReq->userId, 20 * sizeof(char));
        copyAndTerminate(valReq, authReq);

        switch (strcmp(authReq->action, "REQUEST")) {
            case 0: {
                // Action is "REQUEST"
                authResp = user_auth_1(authReq, client);
                if (memcmp(authResp->authToken, "USER_NOT_FOUND", sizeof("USER_NOT_FOUND") - 1) == 0) {
                    printf("USER_NOT_FOUND\n");
                    continue;
                }
                // Clear the memory in accessReq->authToken and accessReq->refreshToken
                memset(accessReq->authToken, 0, sizeof(accessReq->authToken));
                memset(accessReq->refreshToken, 0, sizeof(accessReq->refreshToken));
                // Copy the data from authResp->authToken to accessReq->authToken and accessReq->refreshToken
                memcpy(accessReq->authToken, authResp->authToken, strlen(authResp->authToken) + 1);
                memcpy(accessReq->refreshToken, authResp->authToken, strlen(authResp->authToken) + 1);
                char *endptr;
                long temp_mode = strtol(authReq->resource, &endptr, 10);

                // Check for errors during conversion
                if (*endptr != '\0' && !isspace((unsigned char)*endptr)) {
                    // Handle the error, e.g., invalid characters in the string
                    printf("Invalid input for mode: %s\n", authReq->resource);
                } else {
                    accessReq->mode = (int)temp_mode;
                }
                accResp = user_acc_1(accessReq, client);

                switch (accResp != NULL) {
                case 1: // accResp is not NULL so we have an access token and a refresh token to send
                    memcpy(valReq->accessToken, accResp->accessToken, sizeof(valReq->accessToken) - 1);
                    valReq->accessToken[sizeof(valReq->accessToken) - 1] = '\0';  // Ensure null-termination
                     insertTokensForUser(authResp->authToken, accResp->accessToken, accResp->refreshToken, &databaseForUsTok, authReq->userId, atoi(authReq->resource), accResp->opNumber);
                    if (memcmp(accResp->refreshToken, "withoutRefresh", sizeof("withoutRefresh") - 1) == 0) {
                        printf("%s -> %s\n", authResp->authToken, accResp->accessToken);
                    } else {
                        printf("%s -> %s,%s\n", authResp->authToken, accResp->accessToken, accResp->refreshToken);
                    }
                    break;

                default: // athe user doesn t have access
                    printf("REQUEST_DENIED\n");
                    break;
            }

                break;
            }

            default: {
                // Action is something else 
                // Find the user using the access token
                struct tokenForUserStruct *userStruct = UserTokenbyId(authReq->userId, &databaseForUsTok);
                switch (userStruct == NULL) {
                    case 1: // userStruct is NULL
                        // If the user is not found then the access token for him is removed
                        strcpy(valReq->accessToken, "");
                        break;

                    case 0: // Otherwise, copy the token in the request
                        strcpy(valReq->accessToken, userStruct->tokenAccess);
                        break;
                }
                // send the request for the validate layer
                valResp = user_validate_1(valReq, client);
                printf("%s\n", valResp->info);
            }
        }

    }

    fclose(clientinfile);
}