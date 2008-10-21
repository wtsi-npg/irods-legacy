/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
/* This is script-generated code (for the most part).  */ 
/* See dataObjCopy.h for a description of this API call.*/

#include "dataObjCopy.h"
#include "rodsLog.h"
#include "objMetaOpr.h"
#include "dataObjOpen.h"
#include "dataObjCreate.h"
#include "dataObjRepl.h"
#include "regDataObj.h"
#include "rsGlobalExtern.h"
#include "rcGlobalExtern.h"
#include "getRemoteZoneResc.h"

int
rsDataObjCopy (rsComm_t *rsComm, dataObjCopyInp_t *dataObjCopyInp,
transStat_t **transStat)
{
    dataObjInp_t *srcDataObjInp, *destDataObjInp;
    int srcL1descInx, destL1descInx;
    int status;
    int existFlag;
    uint createMode;
#if 0
    int remoteFlag;
    rodsServerHost_t *rodsServerHost;

    srcDataObjInp = &dataObjCopyInp->srcDataObjInp;
    destDataObjInp = &dataObjCopyInp->destDataObjInp;

    remoteFlag = getAndConnRemoteZone (rsComm, destDataObjInp, &rodsServerHost,
      REMOTE_OPEN);

    if (remoteFlag < 0) {
        return (remoteFlag);
    } else if (remoteFlag == REMOTE_HOST) {
        status = _rcDataObjCopy (rodsServerHost->conn, dataObjCopyInp,
	  transStat);
	return status;
    }
#endif
    srcDataObjInp = &dataObjCopyInp->srcDataObjInp;
    destDataObjInp = &dataObjCopyInp->destDataObjInp;

    *transStat = malloc (sizeof (transStat_t));
    memset (*transStat, 0, sizeof (transStat_t));

    if (strcmp (srcDataObjInp->objPath, destDataObjInp->objPath) == 0) {
        rodsLog (LOG_ERROR,
          "rsDataObjCopy: same src and dest objPath %s not allowed",
	  srcDataObjInp->objPath);
	return (USER_INPUT_PATH_ERR);
    }
	
    addKeyVal (&srcDataObjInp->condInput, PHYOPEN_BY_SIZE_KW, "");
    srcL1descInx = rsDataObjOpen (rsComm, srcDataObjInp);

    if (srcL1descInx < 0)
        return srcL1descInx;

    /* have to set L1desc[srcL1descInx].dataSize because open set this to -1 */
    destDataObjInp->dataSize = L1desc[srcL1descInx].dataSize =
     L1desc[srcL1descInx].dataObjInfo->dataSize;

    createMode = atoi (L1desc[srcL1descInx].dataObjInfo->dataMode);
    if (createMode >= 0100)
	destDataObjInp->createMode = createMode; 
    L1desc[srcL1descInx].oprType = COPY_SRC;

    if (L1desc[srcL1descInx].l3descInx <= 2) {
        /* dataSingleBuf */
        addKeyVal (&destDataObjInp->condInput, NO_OPEN_FLAG_KW, "");
    }

    destL1descInx = rsDataObjCreate (rsComm, destDataObjInp);

    if (destL1descInx < 0) {
	return (destL1descInx);
    }

    if (L1desc[destL1descInx].replStatus == NEWLY_CREATED_COPY) {
	existFlag = 0;
    } else {
	existFlag = 1;
    }
	
    L1desc[destL1descInx].oprType = COPY_DEST;

    L1desc[destL1descInx].srcL1descInx = srcL1descInx;

    rstrcpy (L1desc[destL1descInx].dataObjInfo->dataType, 

    L1desc[srcL1descInx].dataObjInfo->dataType, NAME_LEN);
    /* set dataSize for verification in _rsDataObjClose */

    L1desc[destL1descInx].dataSize = 
      L1desc[srcL1descInx].dataObjInfo->dataSize;

    (*transStat)->bytesWritten = L1desc[srcL1descInx].dataObjInfo->dataSize;

    status = _rsDataObjCopy (rsComm, destL1descInx, existFlag);

    if (status >= 0) {
        (*transStat)->numThreads = destDataObjInp->numThreads;
    }

    return (status);
}

int
_rsDataObjCopy (rsComm_t *rsComm, int destL1descInx, int existFlag)
{
    dataObjInp_t *srcDataObjInp, *destDataObjInp;
    dataObjCloseInp_t dataObjCloseInp;
    dataObjInfo_t *srcDataObjInfo, *destDataObjInfo;
    int srcL1descInx;
    int status;

    destDataObjInp = L1desc[destL1descInx].dataObjInp;
    destDataObjInfo = L1desc[destL1descInx].dataObjInfo;
    srcL1descInx = L1desc[destL1descInx].srcL1descInx;

    srcDataObjInp = L1desc[srcL1descInx].dataObjInp;
    srcDataObjInfo = L1desc[srcL1descInx].dataObjInfo;

    if (L1desc[srcL1descInx].l3descInx <= 2) {
        /* no physical file was opened */
        status = l3DataCopySingleBuf (rsComm, destL1descInx);
	/* has not been registered yet because of NO_OPEN_FLAG_KW */
	if (status >= 0 && existFlag == 0 && 
	  destDataObjInfo->specColl == NULL) {
            status = svrRegDataObj (rsComm, destDataObjInfo);
            if (status < 0) {
                rodsLog (LOG_NOTICE,
                  "_rsDataObjCopy: svrRegDataObj for %s failed, status = %d",
                  destDataObjInfo->objPath, status);
                return (status);
	    }
	}
    } else {
        destDataObjInp->numThreads = getNumThreads (rsComm,
	 srcDataObjInfo->dataSize, destDataObjInp->numThreads, NULL);
        status = dataObjCopy (rsComm, destL1descInx);
    }

    memset (&dataObjCloseInp, 0, sizeof (dataObjCloseInp));

    dataObjCloseInp.l1descInx = destL1descInx;
    if (status >= 0) {
        dataObjCloseInp.bytesWritten = srcDataObjInfo->dataSize;
    }

    rsDataObjClose (rsComm, &dataObjCloseInp);

    return (status);
}

