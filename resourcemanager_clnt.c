/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <memory.h> /* for memset */
#include "resourcemanager.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

Response_authentication *
user_auth_1(Request_user_authentication *argp, CLIENT *clnt)
{
	static Response_authentication clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, USER_AUTH,
		(xdrproc_t) xdr_Request_user_authentication, (caddr_t) argp,
		(xdrproc_t) xdr_Response_authentication, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

Response_user_access *
user_acc_1(Request_user_access *argp, CLIENT *clnt)
{
	static Response_user_access clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, USER_ACC,
		(xdrproc_t) xdr_Request_user_access, (caddr_t) argp,
		(xdrproc_t) xdr_Response_user_access, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}

Response_user_validate *
user_validate_1(Request_user_validate *argp, CLIENT *clnt)
{
	static Response_user_validate clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call (clnt, USER_VALIDATE,
		(xdrproc_t) xdr_Request_user_validate, (caddr_t) argp,
		(xdrproc_t) xdr_Response_user_validate, (caddr_t) &clnt_res,
		TIMEOUT) != RPC_SUCCESS) {
		return (NULL);
	}
	return (&clnt_res);
}
