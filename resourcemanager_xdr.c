/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "resourcemanager.h"

bool_t
xdr_Request_user_authentication (XDR *xdrs, Request_user_authentication *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->userId, 20))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->action, 20))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->resource, 20))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Response_authentication (XDR *xdrs, Response_authentication *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->authToken, 30))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Request_user_access (XDR *xdrs, Request_user_access *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->userId, 20))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->authToken, 30))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->refreshToken, 30))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->mode))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Response_user_access (XDR *xdrs, Response_user_access *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->accessToken, 30))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->refreshToken, 30))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->message, 50))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->opNumber))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Request_user_validate (XDR *xdrs, Request_user_validate *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->action, 20))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->resource, 20))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->accessToken, 30))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Response_user_validate (XDR *xdrs, Response_user_validate *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->info, 50))
		 return FALSE;
	return TRUE;
}