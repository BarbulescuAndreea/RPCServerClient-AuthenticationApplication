/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "resourcemanager.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "headers/database.h"

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

char **listOfUsers;
char **listOfResources;
int remainingOp;

static void
resource_manager_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		Request_user_authentication user_auth_1_arg;
		Request_user_access user_acc_1_arg;
		Request_user_validate user_validate_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case USER_AUTH:
		_xdr_argument = (xdrproc_t) xdr_Request_user_authentication;
		_xdr_result = (xdrproc_t) xdr_Response_authentication;
		local = (char *(*)(char *, struct svc_req *)) user_auth_1_svc;
		break;

	case USER_ACC:
		_xdr_argument = (xdrproc_t) xdr_Request_user_access;
		_xdr_result = (xdrproc_t) xdr_Response_user_access;
		local = (char *(*)(char *, struct svc_req *)) user_acc_1_svc;
		break;

	case USER_VALIDATE:
		_xdr_argument = (xdrproc_t) xdr_Request_user_validate;
		_xdr_result = (xdrproc_t) xdr_Response_user_validate;
		local = (char *(*)(char *, struct svc_req *)) user_validate_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

Queue permiss;

int
main (int argc, char **argv)
{	
	// set buffering
	setvbuf(stdout, 0, _IONBF, 0);
	register SVCXPRT *transp;
	// Initialize databases
	listOfUsers = databaseInitUsers(argv[1]);
	listOfResources = databaseInitResources(argv[2]);
	permiss = readPermissionsFile(argv[3]);
	remainingOp = atoi(argv[4]);

	pmap_unset (RESOURCE_MANAGER, RESOURCE_MANAGER_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, RESOURCE_MANAGER, RESOURCE_MANAGER_VERSION, resource_manager_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (RESOURCE_MANAGER, RESOURCE_MANAGER_VERSION, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, RESOURCE_MANAGER, RESOURCE_MANAGER_VERSION, resource_manager_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (RESOURCE_MANAGER, RESOURCE_MANAGER_VERSION, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
