/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* dataObjRsync.h - This dataObj may be generated by a program or script
 */

#ifndef DATA_OBJ_RSYNC_H
#define DATA_OBJ_RSYNC_H

/* This is a Object File I/O API call */

#include "rods.h"
#include "rcMisc.h"
#include "procApiRequest.h"
#include "apiNumber.h"
#include "initServer.h"
#include "dataObjWrite.h"
#include "dataObjClose.h"
#include "oprComplete.h"


#if defined(RODS_SERVER)
#define RS_DATA_OBJ_RSYNC rsDataObjRsync
/* prototype for the server handler */
int
rsDataObjRsync (rsComm_t *rsComm, dataObjInp_t *dataObjInp,
msParamArray_t **outParamArray); 
int
rsRsyncDataToFile (rsComm_t *rsComm, dataObjInp_t *dataObjInp);
int
rsRsyncFileToData (rsComm_t *rsComm, dataObjInp_t *dataObjInp);
int
rsRsyncDataToData (rsComm_t *rsComm, dataObjInp_t *dataObjInp);
int
_rsDataObjChksum (rsComm_t *rsComm, dataObjInp_t *dataObjInp, 
char **outChksumStr, dataObjInfo_t **dataObjInfoHead);
#else
#define RS_DATA_OBJ_RSYNC NULL
#endif

#ifdef  __cplusplus
extern "C" {
#endif

/* prototype for the client call */
/* rcDataObjRsync - Synchronize the data between a  local  copy  (local file  
 * system)  and the copy stored in iRODS or between two iRODS copies.
 * Input -
 *   rcComm_t *conn - The client connection handle.
 *   dataObjInp_t *dataObjInp - generic dataObj input. Relevant items are:
 *      objPath - the path of the data object.
 *      condInput - conditional Input
 *          ALL_KW - update all copies.
 *	    FORCE_FLAG_KW - always needed.
 *	    VERIFY_BY_SIZE_KW - use size instead of checksum value.
 *          DEST_RESC_NAME_KW - "value" = The destination Resource.
 *	    RSYNC_MODE_KW - valid "value" are :
 *	        IRODS_TO_IRODS - sync an iRods object to an iRods object.
 *		IRODS_TO_LOCAL - sync a local file to an iRods object.
 *		LOCAL_TO_IRODS - sync an iRods object to a local file.
 *	    RSYNC_DEST_PATH_KW - "value" = the destination path. 
 *		If RSYNC_MODE_KW == IRODS_TO_IRODS, it is an iRods path.
 *		if RSYNC_MODE_KW == IRODS_TO_LOCAL, it is an iRods object. 
 *		if RSYNC_MODE_KW == LOCAL_TO_IRODS, it is a local file. 
 *	    RSYNC_CHKSUM_KW - "value" = checksum value.
 *   return value - The status of the operation.
 */

int
rcDataObjRsync (rcComm_t *conn, dataObjInp_t *dataObjInp); 
int
_rcDataObjRsync (rcComm_t *conn, dataObjInp_t *dataObjInp,
msParamArray_t **outParamArray);

#ifdef  __cplusplus
}
#endif

#endif	/* DATA_OBJ_RSYNC_H */
