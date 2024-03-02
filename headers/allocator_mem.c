#include "allocator_mem.h"
#include "../resourcemanager.h"

// Allocate memory for the request_user_authentication fields
void allocateRequestUserAuth(Request_user_authentication* request_auth) {
    if (request_auth == NULL) {
        // Handle memory allocation error
        exit(EXIT_FAILURE);
    }
    request_auth->userId = malloc(20 * sizeof(char));
    request_auth->action = malloc(20 * sizeof(char));
    request_auth->resource = malloc(20 * sizeof(char));
}

void allocateRequestUserAccess(Request_user_access* request_acc) {
    if (request_acc == NULL) {
        // Handle memory allocation error
        exit(EXIT_FAILURE);
    }
    request_acc->userId = malloc(20 * sizeof(char));
    request_acc->authToken = malloc(30 * sizeof(char));
    request_acc->refreshToken = malloc(30 * sizeof(char));
    request_acc->mode = 0;
}

void allocateRequestUserValidate(Request_user_validate* request_validate){
    if (request_validate == NULL) {
        // Handle memory allocation error
        exit(EXIT_FAILURE);
    }
    request_validate->accessToken = malloc(30 * sizeof(char));
    request_validate->action = malloc(20 * sizeof(char));
    request_validate->resource = malloc(20 *sizeof(char));
}

void copyAndTerminate(Request_user_validate *request_validate, Request_user_authentication *request_auth) {
        memcpy(request_validate->action, request_auth->action, 20 * sizeof(char));
        memcpy(request_validate->resource, request_auth->resource, 20 * sizeof(char));
        request_validate->resource[strlen(request_validate->resource) - 1] = '\0';  // Ensure null-terminated
}