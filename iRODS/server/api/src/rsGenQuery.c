/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

/* See genQuery.h for a description of this API call.*/

#include "genQuery.h"
#include "icatHighLevelRoutines.h"
#include "miscUtil.h"

/* can be used for debug: */
/* extern int printGenQI( genQueryInp_t *genQueryInp); */

int
rsGenQuery (rsComm_t *rsComm, genQueryInp_t *genQueryInp, 
genQueryOut_t **genQueryOut)
{
    rodsServerHost_t *rodsServerHost;
    int status;
    char *zoneHint;

    zoneHint = getZoneHintForGenQuery (genQueryInp);
 
    status = getAndConnRcatHost(rsComm, SLAVE_RCAT, zoneHint,
				&rodsServerHost);
    if (status < 0) {
       return(status);
    }

    if (rodsServerHost->localFlag == LOCAL_HOST) {
#ifdef RODS_CAT
       status = _rsGenQuery (rsComm, genQueryInp, genQueryOut);
#else
       rodsLog(LOG_NOTICE, 
	       "rsGenQuery error. RCAT is not configured on this host");
       return (SYS_NO_RCAT_SERVER_ERR);
#endif 
    } else {
        status = rcGenQuery(rodsServerHost->conn,
			   genQueryInp, genQueryOut);
    }
    if (status < 0  && status != CAT_NO_ROWS_FOUND) {
        rodsLog (LOG_NOTICE,
		 "rsGenQuery: rcGenQuery failed, status = %d", status);
    }
    return (status);
}

#ifdef RODS_CAT
int
_rsGenQuery (rsComm_t *rsComm, genQueryInp_t *genQueryInp,
	     genQueryOut_t **genQueryOut)
{
    int status;

    static int ruleExecuted=0;
    ruleExecInfo_t rei;
    static int ruleResult=0;

    /*  printGenQI(genQueryInp);  for debug */

    *genQueryOut = malloc(sizeof(genQueryOut_t));
    memset((char *)*genQueryOut, 0, sizeof(genQueryOut_t));

    if (ruleExecuted==0) {
#if 0
       msParam_t *outMsParam;
#endif
       memset((char*)&rei,0,sizeof(rei));
       status = applyRule ("acAclPolicy", NULL, &rei, NO_SAVE_REI);
       ruleResult = rei.status;
       if (status==0) {
	  ruleExecuted=1; /* No need to retry next time since it
                             succeeded.  Since this is called at
                             startup, the Rule Engine may not be
                             initialized yet, in which case the
                             default setting is fine and we should
                             retry next time. */
#if 0
	  /* No longer need this as msiAclPolicy calls
	     chlGenQueryAccessControlSetup to set the flag.  Leaving
	     it in the code for now in case needed later. */
	  outMsParam = getMsParamByLabel(&rei.inOutMsParamArray, "STRICT");
	  printf("outMsParam=%x\n",(int)outMsParam);
	  if (outMsParam != NULL) {
	     ruleResult=1;
	  }
#endif
       }
       printf("rsGenQuery rule status=%d ruleResult=%d\n",status,ruleResult);
    }

    chlGenQueryAccessControlSetup(rsComm->clientUser.userName, 
			      rsComm->clientUser.rodsZone,
	 		      rsComm->clientUser.authInfo.authFlag,
			      -1);
#if 0
    rodsLog (LOG_NOTICE, 
	     "_rsGenQuery debug: client %s %d proxy %s %d", 
	     rsComm->clientUser.userName, 
	     rsComm->clientUser.authInfo.authFlag,
	     rsComm->proxyUser.userName, 
	     rsComm->proxyUser.authInfo.authFlag);
#endif

    status = chlGenQuery(*genQueryInp, *genQueryOut);

    if (status < 0) {
       clearGenQueryOut (*genQueryOut);
       free (*genQueryOut);
       *genQueryOut = NULL;
       if (status != CAT_NO_ROWS_FOUND) {
	  rodsLog (LOG_NOTICE, 
		   "_rsGenQuery: genQuery status = %d", status);
       }
       return (status);
    }
    return (status);
} 
#endif
