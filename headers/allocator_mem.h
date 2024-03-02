#ifndef ALLOCSTOR_H
#define ALLOCSTOR_H

#include <stdlib.h>
#include "../resourcemanager.h"

// Function declarations
void allocateRequestUserAuth(Request_user_authentication* request_auth);

void allocateRequestUserAccess(Request_user_access* request_acc);

void allocateRequestUserValidate(Request_user_validate* request_validate);

void copyAndTerminate( Request_user_validate *request_validate, Request_user_authentication *request_auth);

#endif // ALLOCSTOR_H
