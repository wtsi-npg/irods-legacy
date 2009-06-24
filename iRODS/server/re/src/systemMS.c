/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/
#include <string.h>
#include "reGlobalsExtern.h"
#include "icatHighLevelRoutines.h"
#include "execMyRule.h"

int
fillSubmitConditions (char *action, char *inDelayCondition, bytesBuf_t *packedReiAndArgBBuf, 
        ruleExecSubmitInp_t *ruleSubmitInfo,  ruleExecInfo_t *rei);

int assign(msParam_t* var, msParam_t* value, ruleExecInfo_t *rei)
{
  char *varName;
  char *varMap;
  char *varValue;
  char aVal[MAX_COND_LEN * 2];
  int i;
  char eaVal[MAX_COND_LEN * 2];
  varName = (char *)var->label;
  varValue = (char *) var->inOutStruct;
  if ( varValue[0] == '$') {
    i = getVarMap("", var->inOutStruct, &varMap, 0);
    if (i < 0) return(i);
    rstrcpy(aVal,value->inOutStruct,MAX_COND_LEN * 2);
    i = evaluateExpression(aVal, eaVal,rei);
    if (i < 0 ) 
      return(i);
    /*    if (strcmp(eaVal, aVal))*/
    rodsLog (LOG_NOTICE,"BEFORE:uioc=%s,uiop=%s:cp=%d,pp=%d,eaval=%s\n",rei->uoic->userName,rei->uoip->userName, rei->uoic,rei->uoip,eaVal);
    i = setVarValue(varMap,rei, eaVal);
    rodsLog (LOG_NOTICE,"AFTER:uioc=%s,uiop=%s:cp=%d,pp=%d\n",rei->uoic->userName,rei->uoip->userName, rei->uoic,rei->uoip);
  }
  else { /* it is a *-variable so just copy */
    if ((value->type == NULL && value->label != NULL) ||
	strcmp (value->type, STR_MS_T) == 0 ||
	strcmp (value->type, STR_PI) == 0 ) {
      if (value->inOutStruct != NULL)
	rstrcpy(aVal,value->inOutStruct,MAX_COND_LEN * 2);
      else
	rstrcpy(aVal,value->label,MAX_COND_LEN * 2);
      i = evaluateExpression(aVal, eaVal,rei);
      if (i < 0)
	fillMsParam (var, NULL, value->type, value->inOutStruct, value->inpOutBuf);
      else if (!strcmp(eaVal,aVal)) {
	var->inOutStruct = strdup(aVal);
	var->type = strdup(STR_MS_T);
      }
      else {
	/*	fillIntInMsParam(var,j); */
	var->inOutStruct = strdup(eaVal);
	var->type = strdup(STR_MS_T);
      }
    }
    else
      fillMsParam (var, NULL, value->type, value->inOutStruct, value->inpOutBuf);
  }
  return(0);
}


int whileExec(msParam_t* condition, msParam_t* whileBody,
	      msParam_t* recoverWhileBody, ruleExecInfo_t *rei)
{
  int i,done;
  char *cond, *ruleBody;
  msParamArray_t *inMsParamArray = NULL;
  char eaVal[MAX_COND_LEN * 2];
  done = 0;
  inMsParamArray = rei->msParamArray;
  /*  cond = (char *) malloc(strlen(condition->label) + 10 + MAX_COND_LEN * 2); */
  cond = (char *) malloc(strlen(condition->inOutStruct) + 10 + MAX_COND_LEN * 2);
  ruleBody = (char *) malloc ( strlen(whileBody->inOutStruct) +
			       strlen(recoverWhileBody->inOutStruct) + 10 + MAX_COND_LEN * 2);
  sprintf(ruleBody,"%s|%s",(char *) whileBody->inOutStruct,
	    (char *) recoverWhileBody->inOutStruct + MAX_COND_LEN * 8);
    
  while (done == 0) {
    /* strcpy(cond , condition->label); */
    strcpy(cond , condition->inOutStruct);
    i = evaluateExpression(cond, eaVal, rei);
    if (i < 0)
      return(i);
    if (i == 0) { /* FALSE */
      done = 1;
      break;
    }    
    i = execMyRuleWithSaveFlag(ruleBody, inMsParamArray, rei, 0);
    if (i == BREAK_ACTION_ENCOUNTERED_ERR) 
      return(0);
    if (i != 0) 
      return(i);
    /*  RECOVERY OF OTHER EXECUTED BODY IS NOT DONE. HOW CAN WE DO THAT */
  }
  return(0);
}


int forExec(msParam_t* initial, msParam_t* condition, msParam_t* step, 
	    msParam_t* forBody, msParam_t* recoverForBody, ruleExecInfo_t *rei)
{

  int i,done;
  char *cond, *ruleBody, *stepStr;
  char eaVal[MAX_COND_LEN * 2];
  done = 0;
  /*  i = execMyRuleWithSaveFlag(initial->label,rei->msParamArray, rei,0); */
  i = execMyRuleWithSaveFlag(initial->inOutStruct,rei->msParamArray, rei,0); 
  if (i != 0)
    return(i);
  /*  cond = (char *) malloc(strlen(condition->label) + 10 + MAX_COND_LEN * 2); */
  cond = (char *) malloc(strlen(condition->inOutStruct) + 10 + MAX_COND_LEN * 2);
  /* stepStr = (char *) malloc(strlen(step->label) + 10 + MAX_COND_LEN * 2); */
  stepStr = (char *) malloc(strlen(step->inOutStruct) + 10 + MAX_COND_LEN * 2);
  ruleBody = (char *) malloc ( strlen(forBody->inOutStruct) +
                                 strlen(recoverForBody->inOutStruct) + 10 + MAX_COND_LEN * 8);
  
  while (done == 0) {
    /* strcpy(cond , condition->label); */
    strcpy(cond , condition->inOutStruct);
    i = evaluateExpression(cond, eaVal, rei);
    if (i < 0)
      return(i);
    if (i == 0) { /* FALSE */
      done = 1;
      break;
    }
    sprintf(ruleBody,"%s|%s",(char *) forBody->inOutStruct,
            (char *) recoverForBody->inOutStruct);
    i = execMyRuleWithSaveFlag(ruleBody, rei->msParamArray, rei, 0);
    if (i == BREAK_ACTION_ENCOUNTERED_ERR) 
      return(0);
    if (i != 0)
      return(i);
    /* strcpy(cond , condition->label); */
    strcpy(stepStr,step->inOutStruct);
    i = execMyRuleWithSaveFlag(stepStr,rei->msParamArray, rei,0);
    if (i != 0)
      return(i);
  }
  return(0);
}

int ifExec(msParam_t* condition, msParam_t* thenC, msParam_t* recoverThen, 
	   msParam_t* elseC, msParam_t* recoverElse, ruleExecInfo_t *rei)
{
  int i;  
  char *cond, *thenStr, *elseStr;
  char eaVal[MAX_COND_LEN * 2];

  if (condition->inOutStruct == NULL) {
    /* cond = (char *) malloc(strlen(condition->label) + 10 + MAX_COND_LEN * 2);
       strcpy(cond , condition->label); */
    cond = (char *) malloc(strlen(condition->inOutStruct) + 10 + MAX_COND_LEN * 2);
    strcpy(cond , condition->inOutStruct);
  }
  else {
    cond = (char *) malloc(strlen(condition->inOutStruct) + 10 + MAX_COND_LEN * 2);
    strcpy(cond , condition->inOutStruct);
  }

  i = evaluateExpression(cond, eaVal, rei);
  free(cond);
  if (i < 0)
    return(i);
  if (i == 1) { /* TRUE */
    thenStr = (char *) malloc(strlen(thenC->inOutStruct) + strlen(recoverThen->inOutStruct) + 10 + MAX_COND_LEN * 8);
    sprintf(thenStr,"%s|%s",(char *) thenC->inOutStruct,
            (char *) recoverThen->inOutStruct);
    i = execMyRuleWithSaveFlag(thenStr, rei->msParamArray, rei, 0);
    free(thenStr);
  }
  else {
    elseStr = (char *) malloc(strlen(elseC->inOutStruct) + strlen(recoverElse->inOutStruct) + 10 + MAX_COND_LEN * 8);
    sprintf(elseStr,"%s|%s",(char *) elseC->inOutStruct,
            (char *) recoverElse->inOutStruct);
    i = execMyRuleWithSaveFlag(elseStr, rei->msParamArray, rei, 0);
    free(elseStr);
  }
  return(i);

}

int breakExec(ruleExecInfo_t *rei)
{
  return (BREAK_ACTION_ENCOUNTERED_ERR);
}
int forEachExec(msParam_t* inlist, msParam_t* body, msParam_t* recoverBody,
	      ruleExecInfo_t *rei)
{

  char *label ;
  int i;
  msParamArray_t *inMsParamArray;
  void *value, *restPtr, *inPtr, *buf, *inStructPtr, *msParamStruct ;
  char *typ;
  char *bodyStr;
  msParam_t *msParam, *list;
  bytesBuf_t *inBufPtr, *msParamBuf;
  int first = 1;
  int inx;
  char *outtyp;

  list = (msParam_t *) malloc (sizeof (msParam_t));
  memset (list, 0, sizeof (msParam_t));
  replMsParam (inlist,list);
  typ = strdup(list->type);
  label = list->label;
  inPtr = list->inOutStruct;
  inStructPtr = inPtr;
  inBufPtr = list->inpOutBuf;
  outtyp = typ;
  buf = inBufPtr;
  value = NULL;

  inMsParamArray = rei->msParamArray;
  msParam = getMsParamByLabel(inMsParamArray, label);
  if (msParam != NULL) {
    msParamStruct = msParam->inOutStruct;
    msParamBuf = msParam->inpOutBuf;

  }
  else 
    msParamStruct = NULL;

  i = 0;

  bodyStr = (char *) malloc ( strlen(body->inOutStruct) +
                               strlen(recoverBody->inOutStruct) + 10 + MAX_COND_LEN * 8);
  sprintf(bodyStr,"%s|%s",(char *) body->inOutStruct,
	    (char *) recoverBody->inOutStruct);
    
  restPtr = NULL;
  inx = 0;
  while (inPtr != NULL) {
    i = getNextValueAndBufFromListOrStruct(typ, inPtr, &value, (bytesBuf_t **) &buf, &restPtr, &inx, &outtyp);
    if ( i != 0)
      break;

    if (first == 1 && msParam == NULL) {
      addMsParam(inMsParamArray, label, outtyp ,value, buf);
      msParam = getMsParamByLabel(inMsParamArray, label);
      first = 0;
    }
    else {
      msParam->inOutStruct = value;
      msParam->inpOutBuf = buf;
      msParam->type = outtyp;
    }


    i = execMyRuleWithSaveFlag(bodyStr, inMsParamArray, rei, 0);
    if ( i != 0)
      break;
    /*  RECOVERY OF OTHER EXECUTED BODY IS NOT DONE. HOW CAN WE DO THAT */
    /***    freeNextValueStruct(&value,outtyp,"internal"); ***/
    inPtr = restPtr;    
  }
  /***  freeNextValueStruct(&value,outtyp,"all");***/
  if (msParamStruct != NULL) {
    /* value in rei->params get replaced back... */
    msParam->inOutStruct = msParamStruct;
    msParam->inpOutBuf = msParamBuf;
    msParam->type = typ;
  }
  /*** 
       clearMsParamArray (list,1);
       free(list);
  ***/
  if (i == BREAK_ACTION_ENCOUNTERED_ERR) 
      return(0);
  if (i == NO_VALUES_FOUND)
    return(0);
  return(i);
}

int delayExec(msParam_t *mPA, msParam_t *mPB, msParam_t *mPC, ruleExecInfo_t *rei)
{
  int i;
  char actionCall[MAX_ACTION_SIZE];  
  char recoveryActionCall[MAX_ACTION_SIZE];  
  char delayCondition[MAX_ACTION_SIZE]; 

  rstrcpy(delayCondition, (char *) mPA->inOutStruct,MAX_ACTION_SIZE);
  rstrcpy(actionCall, (char *) mPB->inOutStruct,MAX_ACTION_SIZE);
  rstrcpy(recoveryActionCall, (char *) mPC->inOutStruct,MAX_ACTION_SIZE);
  i = _delayExec(actionCall, recoveryActionCall, delayCondition, rei);
  return(i);
}



int _delayExec(char *inActionCall, char *recoveryActionCall, 
	       char *delayCondition,  ruleExecInfo_t *rei)
{

  char *args[MAX_NUM_OF_ARGS_IN_ACTION];
  int i, argc;
  ruleExecSubmitInp_t *ruleSubmitInfo;
  char action[MAX_ACTION_SIZE];  
  char tmpStr[NAME_LEN];  
  bytesBuf_t *packedReiAndArgBBuf = NULL;
  char *ruleExecId;
  char *actionCall;


  RE_TEST_MACRO ("    Calling _delayExec");

  actionCall = inActionCall;
  /* Get Arguments */
  if (strstr(actionCall,"##") == NULL && !strcmp(recoveryActionCall,"nop")) {
    /* single call */
    i = parseAction(actionCall,action,args, &argc);
    if (i != 0)
      return(i);
    mapExternalFuncToInternalProc(action);
    argc = 0;
  }
  else {
    actionCall = malloc(strlen(inActionCall) + strlen(recoveryActionCall) + 3);
    sprintf(actionCall,"%s|%s",inActionCall,recoveryActionCall);
    args[0] = NULL;
    args[1] = NULL;
    argc = 0;
  }
  /* Pack Rei and Args */
  i = packReiAndArg (rei->rsComm, rei, args, argc, &packedReiAndArgBBuf);
  if (i < 0) {
    if (actionCall != inActionCall) 
      free (actionCall);
    return(i);
  }
  /* fill Conditions into Submit Struct */
  ruleSubmitInfo = mallocAndZero(sizeof(ruleExecSubmitInp_t));
  i  = fillSubmitConditions (actionCall, delayCondition, packedReiAndArgBBuf, ruleSubmitInfo, rei);
  if (actionCall != inActionCall) 
    free (actionCall);
  if (i < 0) {
    free(ruleSubmitInfo);
    return(i);
  }
  
  /* Store ReiArgs Struct in a File */
  i = rsRuleExecSubmit(rei->rsComm, ruleSubmitInfo, &ruleExecId);
  if (packedReiAndArgBBuf != NULL) {
    clearBBuf (packedReiAndArgBBuf);
    free (packedReiAndArgBBuf);
  }
  
  free(ruleSubmitInfo);
  if (i < 0) 
    return(i);
  free (ruleExecId);
  snprintf(tmpStr,NAME_LEN, "%d",i);
  i = pushStack(&delayStack,tmpStr);
  return(i);
}

int recover_delayExec(msParam_t *actionCall, msParam_t *delayCondition,  ruleExecInfo_t *rei)
{

  int i;
  ruleExecDelInp_t ruleExecDelInp;

  RE_TEST_MACRO ("    Calling recover_delayExec");

  i  = popStack(&delayStack,ruleExecDelInp.ruleExecId);
  if (i < 0)
    return(i);

  i = rsRuleExecDel(rei->rsComm, &ruleExecDelInp);
  return(i);

}


int remoteExec(msParam_t *mPD, msParam_t *mPA, msParam_t *mPB, msParam_t *mPC, ruleExecInfo_t *rei)
{
  int i;
  execMyRuleInp_t execMyRuleInp;
  msParamArray_t *tmpParamArray, *aParamArray;
  msParamArray_t *outParamArray = NULL;
  char tmpStr[LONG_NAME_LEN];
  char tmpStr1[LONG_NAME_LEN];
  /*
  char actionCall[MAX_ACTION_SIZE];  
  char recoveryActionCall[MAX_ACTION_SIZE];  
  char delayCondition[MAX_ACTION_SIZE]; 
  char hostName[MAX_ACTION_SIZE]; 
  rstrcpy(hostName, (char *) mPD->inOutStruct,MAX_ACTION_SIZE);
  rstrcpy(delayCondition, (char *) mPA->inOutStruct,MAX_ACTION_SIZE);
  rstrcpy(actionCall, (char *) mPB->inOutStruct,MAX_ACTION_SIZE);
  rstrcpy(recoveryActionCall, (char *) mPC->inOutStruct,MAX_ACTION_SIZE);
  i = _remoteExec(actionCall, recoveryActionCall, delayCondition, hostName, rei);
  */
  memset (&execMyRuleInp, 0, sizeof (execMyRuleInp));
  execMyRuleInp.condInput.len=0;
  rstrcpy (execMyRuleInp.outParamDesc, ALL_MS_PARAM_KW, LONG_NAME_LEN);
  /*  rstrcpy (execMyRuleInp.addr.hostAddr, mPD->inOutStruct, LONG_NAME_LEN);*/
  rstrcpy (tmpStr, (char *) mPD->inOutStruct, LONG_NAME_LEN);
#if 0
  i = evaluateExpression(tmpStr, execMyRuleInp.addr.hostAddr, rei);
  if (i < 0)
    return(i);
#else
  i = evaluateExpression(tmpStr, tmpStr1, rei);
  if (i < 0)
    return(i);
  parseHostAddrStr (tmpStr1, &execMyRuleInp.addr);
#endif
  snprintf(execMyRuleInp.myRule, META_STR_LEN, "remExec||%s|%s",  (char*)mPB->inOutStruct,(char*)mPC->inOutStruct);
  addKeyVal(&execMyRuleInp.condInput,"execCondition",mPA->inOutStruct);
  
  tmpParamArray =  (msParamArray_t *) malloc (sizeof (msParamArray_t));
  memset (tmpParamArray, 0, sizeof (msParamArray_t));
  i = replMsParamArray (rei->msParamArray,tmpParamArray);
  if (i < 0) {
    free(tmpParamArray);
    return(i);
  }
  aParamArray = rei->msParamArray;
  rei->msParamArray = tmpParamArray;
  execMyRuleInp.inpParamArray = rei->msParamArray;
  i = rsExecMyRule (rei->rsComm, &execMyRuleInp,  &outParamArray);
  carryOverMsParam(outParamArray, aParamArray);
  rei->msParamArray = aParamArray;
  clearMsParamArray(tmpParamArray,0);
  free(tmpParamArray);
  return(i);
}


/*****
int _remoteExec(char *inActionCall, char *recoveryActionCall, 
	       char *delayCondition,  char *hostName, ruleExecInfo_t *rei)
{

  char *args[MAX_NUM_OF_ARGS_IN_ACTION];
  int i, argc;
  ruleExecSubmitInp_t *ruleSubmitInfo;
  char action[MAX_ACTION_SIZE];  
  char tmpStr[NAME_LEN];  
  bytesBuf_t *packedReiAndArgBBuf = NULL;
  char *ruleExecId;
  char *actionCall;


  RE_TEST_MACRO ("    Calling _delayExec");

  actionCall = inActionCall;
  if (strstr(actionCall,"##") == NULL && !strcmp(recoveryActionCall,"nop")) {
    i = parseAction(actionCall,action,args, &argc);
    if (i != 0)
      return(i);
    mapExternalFuncToInternalProc(action);
    argc = 0;
  }
  else {
    actionCall = malloc(strlen(inActionCall) + strlen(recoveryActionCall) + 3);
    sprintf(actionCall,"%s|%s",inActionCall,recoveryActionCall);
    args[0] = NULL;
    args[1] = NULL;
    argc = 0;
  }

  i = packReiAndArg (rei->rsComm, rei, args, argc, &packedReiAndArgBBuf);
  if (i < 0) {
    if (actionCall != inActionCall) 
      free (actionCall);
    return(i);
  }

  ruleSubmitInfo = mallocAndZero(sizeof(ruleExecSubmitInp_t));
  i  = fillSubmitConditions (actionCall, delayCondition, packedReiAndArgBBuf, ruleSubmitInfo, rei);
  strncpy(ruleSubmitInfo->exeAddress,hostName,NAME_LEN);
  if (actionCall != inActionCall) 
    free (actionCall);
  if (i < 0) {
    free(ruleSubmitInfo);
    return(i);
  }
  
  i = rsRemoteRuleExecSubmit(rei->rsComm, ruleSubmitInfo, &ruleExecId);
  if (packedReiAndArgBBuf != NULL) {
    clearBBuf (packedReiAndArgBBuf);
    free (packedReiAndArgBBuf);
  }
  
  free(ruleSubmitInfo);
  if (i < 0) 
    return(i);
  free (ruleExecId);
  snprintf(tmpStr,NAME_LEN, "%d",i);
  i = pushStack(&delayStack,tmpStr);
  return(i);
}
******/
int recover_remoteExec(msParam_t *actionCall, msParam_t *delayCondition, char *hostName, ruleExecInfo_t *rei)
{

  int i;
  ruleExecDelInp_t ruleExecDelInp;

  RE_TEST_MACRO ("    Calling recover_delayExec");

  i  = popStack(&delayStack,ruleExecDelInp.ruleExecId);
  if (i < 0)
    return(i);
  /***
  i = rsRemoteRuleExecDel(rei->rsComm, &ruleExecDelInp);
  ***/
  return(i);

}


int
doForkExec(char *prog, char *arg1)
{
   int pid, i;

   i = checkFilePerms(prog);
   if (i) return(i);

   i = checkFilePerms(arg1);
   if (i) return(i);

#ifndef windows_platform
   pid = fork();
   if (pid == -1) return -1;

   if (pid) {
      /*  This is still the parent.  */
   }
   else {
      /* child */
      for (i=0;i<100;i++) {
	 close(i);
      }
      i = execl(prog, prog, arg1, (char *) 0);
      printf("execl failed %d\n",i);
      return(0);
   }
#else
   /* windows platform */
   if(_spawnl(_P_NOWAIT, prog, prog, arg1) == -1)
   {
	   return -1;
   }
#endif

   return(0);
}


int
msiGoodFailure(ruleExecInfo_t *rei)
{
  
  /**** This is Just a Test Stub  ****/
  if (reTestFlag > 0) {
    if (reTestFlag == LOG_TEST_1)
      rodsLog (LOG_NOTICE,"   Calling msiGoodFailure So that It will Retry Other Rules Without Recovery\n");
    return(RETRY_WITHOUT_RECOVERY_ERR);
  }
  /**** This is Just a Test Stub  ****/
  return(RETRY_WITHOUT_RECOVERY_ERR);
}



/* check that a file exists and is not writable by group or other */ 
int
checkFilePerms(char *fileName) {
   struct stat buf;
   if (stat (fileName, &buf) == -1) {
      printf ("Stat failed for file %s\n", 
              fileName);
      return(-1);
   }

   if (buf.st_mode & 022) {
      printf ("File is writable by group or other: %s.\n",
              fileName);
      return(-2);
   }
   return(0);
}

int 
msiFreeBuffer(msParam_t* memoryParam, ruleExecInfo_t *rei)
{


  RE_TEST_MACRO ("Loopback on msiFreeBuffer");


  if (memoryParam->inpOutBuf != NULL)
    free(memoryParam->inpOutBuf);
  memoryParam->inpOutBuf = NULL;
  return(0);

}

int
msiSleep(msParam_t* secPtr, msParam_t* microsecPtr,  ruleExecInfo_t *rei)
{

  int sec, microsec;

  sec = atoi(secPtr->inOutStruct);
  microsec = atoi(microsecPtr->inOutStruct);

  rodsSleep (sec, microsec);
  return(0);
}

int
msiApplyAllRules(msParam_t *actionParam, msParam_t* reiSaveFlagParam, 
		 msParam_t* allRuleExecFlagParam, ruleExecInfo_t *rei)
{
  int i;
  char *action;
  int reiSaveFlag;
  int allRuleExecFlag;

  action = actionParam->inOutStruct;
  reiSaveFlag = atoi(reiSaveFlagParam->inOutStruct);
  allRuleExecFlag = atoi(allRuleExecFlagParam->inOutStruct);
  i = applyAllRules(action, rei->msParamArray, rei, reiSaveFlag,allRuleExecFlag);
  return(i);

}


/**
 * \fn msiGetDiffTime
 * \author  Antoine de Torcy
 * \date   2007-09-19
 * \brief This microservice returns the difference between two system times
 * \note If we have arithmetic MSs in the future we should use DOUBLE_MS_T instead of strings
 *       Default output format is in seconds, use 'human' as 3d input param for human readable format.
 * \param[in] 
 *    inpParam1 - Required - a STR_MS_T containing the start date (system time in seconds)
 *    inpParam2 - Required - a STR_MS_T containing the end date (system time in seconds)
 *    inpParam3 - Optional - a STR_MS_T containing the desired output format
 * \param[out] 
 *    outParam - a STR_MS_T containing the time elapsed between the two dates
 * \return integer
 * \retval 0 on success
 * \sa
 * \post
 * \pre
 * \bug  no known bugs
**/
int
msiGetDiffTime(msParam_t* inpParam1, msParam_t* inpParam2, msParam_t* inpParam3, msParam_t* outParam, ruleExecInfo_t *rei)
{
	long hours, minutes, seconds;
	char timeStr[TIME_LEN];
	char *format;
	int status;

	
	/* For testing mode when used with irule --test */
	RE_TEST_MACRO ("    Calling msiGetDiffTime")


	if (rei == NULL || rei->rsComm == NULL) {
		rodsLog (LOG_ERROR, "msiGetDiffTime: input rei or rsComm is NULL");
		return (SYS_INTERNAL_NULL_INPUT_ERR);
	}

	
	/* Check for proper input */
	if ((parseMspForStr(inpParam1) == NULL)) {
		rodsLog (LOG_ERROR, "msiGetDiffTime: input inpParam1 is NULL");
		return (USER__NULL_INPUT_ERR);
	}

	if ((parseMspForStr (inpParam2) == NULL)) {
		rodsLog (LOG_ERROR, "msiGetDiffTime: input inpParam2 is NULL");
		return (USER__NULL_INPUT_ERR);
	}
	
	
	/* get time values from strings */
	seconds = atol(inpParam2->inOutStruct) - atol(inpParam1->inOutStruct);
	
	/* get desired output format */
	format = inpParam3->inOutStruct;

	
	/* did they ask for human readable format? */
	if (format && !strcmp(format, "human")) {
		/* get hours-minutes-seconds */
		hours = seconds / 3600;
		seconds = seconds % 3600;
		minutes = seconds / 60;
		seconds = seconds % 60;
		
		snprintf(timeStr, TIME_LEN, "%ldh %ldm %lds", hours, minutes, seconds);
	}
	else {
		snprintf(timeStr, TIME_LEN, "%011ld", seconds);
	}
	
	
	/* result goes to outParam */
	status = fillStrInMsParam (outParam, timeStr);	
	
	return(status);
}


/**
 * \fn msiGetSystemTime
 * \author  Antoine de Torcy
 * \date   2007-09-19
 * \brief This microservice returns the local system time
 * \note Default output format is system time in seconds, use 'human' as input param for human readable format.
 * \param[in] 
 *    inpParam - Optional - a STR_MS_T containing the desired output format
 * \param[out] 
 *    outParam - a STR_MS_T containing the time
 * \return integer
 * \retval 0 on success
 * \sa
 * \post
 * \pre
 * \bug  no known bugs
**/
int
msiGetSystemTime(msParam_t* outParam, msParam_t* inpParam, ruleExecInfo_t *rei)
{
	char *format;
	char tStr0[TIME_LEN],tStr[TIME_LEN];
	int status;
	
	/* For testing mode when used with irule --test */
	RE_TEST_MACRO ("    Calling msiGetSystemTime")


	if (rei == NULL || rei->rsComm == NULL) {
		rodsLog (LOG_ERROR, "msiGetSystemTime: input rei or rsComm is NULL");
		return (SYS_INTERNAL_NULL_INPUT_ERR);
	}
	

	format = inpParam->inOutStruct;
	
	if (!format || strcmp(format, "human")) {
		getNowStr(tStr);
	}
	else {
		getNowStr(tStr0);
		getLocalTimeFromRodsTime(tStr0,tStr);
	}
	
	status = fillStrInMsParam (outParam,tStr);

	return(status);
}


/**
 * \fn msiHumanToSystemTime
 * \author  Antoine de Torcy
 * \date   2008-03-11
 * \brief Converts a human readable date to a system timestamp
 * \note Expects an input date in the form: YYYY-MM-DD-hh.mm.ss
 * \param[in] 
 *    inpParam - a STR_MS_T containing the input date
 * \param[out] 
 *    outParam - a STR_MS_T containing the timestamp
 * \return integer
 * \retval 0 on success
 * \sa
 * \post
 * \pre
 * \bug  no known bugs
**/
int
msiHumanToSystemTime(msParam_t* inpParam, msParam_t* outParam, ruleExecInfo_t *rei)
{
	char sys_time[TIME_LEN], *hr_time;
	int status;
	
	/* For testing mode when used with irule --test */
	RE_TEST_MACRO ("    Calling msiHumanToSystemTime")


	/* microservice check */
	if (rei == NULL || rei->rsComm == NULL) {
		rodsLog (LOG_ERROR, "msiHumanToSystemTime: input rei or rsComm is NULL");
		return (SYS_INTERNAL_NULL_INPUT_ERR);
	}


	/* parse inpParam (date input string) */
	if ((hr_time = parseMspForStr (inpParam)) == NULL) {
		rodsLog (LOG_ERROR, "msiHumanToSystemTime: parseMspForStr error for input param.");
		return (rei->status);
	}


	/* left padding with a zero for DB format. Might change that later */
	memset(sys_time, '\0', TIME_LEN);
	sys_time[0] = '0';


	/* conversion */
	if ((status = localToUnixTime (hr_time, &sys_time[1]))  == 0) {
		status = fillStrInMsParam (outParam, sys_time);
	}


	return(status);
}



/**
 * \fn msiStrToByteBuf
 * \author  Antoine de Torcy
 * \date   2008-09-16
 * \brief Converts a string to a bytesBuf_t
 * \note Following discussion on iRODS_Chat. For easily passing parameters to
 *	microservices that require a BUF_LEN_MS_T
 * \param[in] 
 *    inpParam - a STR_MS_T
 * \param[out] 
 *    outParam - a BUF_LEN_MS_T
 * \return integer
 * \retval 0 on success
 * \sa
 * \post
 * \pre
 * \bug  no known bugs
**/
int
msiStrToBytesBuf(msParam_t* str_msp, msParam_t* buf_msp, ruleExecInfo_t *rei)
{
	char *inStr;
	bytesBuf_t *outBuf;

	/* parse str_msp */
	if ((inStr = parseMspForStr(str_msp)) == NULL)  
	{
		rodsLog (LOG_ERROR, "msiStrToBytesBuf: input str_msp is NULL.");
		return (USER__NULL_INPUT_ERR);
	}

	/* buffer init */
	outBuf = (bytesBuf_t *)malloc(sizeof(bytesBuf_t));
	memset (outBuf, 0, sizeof (bytesBuf_t));

	/* fill string in buffer */
	outBuf->len = strlen(inStr);
	outBuf->buf = inStr;

	/* fill buffer in buf_msp */
	fillBufLenInMsParam (buf_msp, outBuf->len, outBuf);
	
	return 0;
}



/**
 * \fn msiListEnabledMS
 * \author  Antoine de Torcy
 * \date   2009-02-12
 * \brief Returns the list of compiled microservices on the local iRODS server
 * \note This microservice looks at reAction.h and returns the list of compiled 
 *  microservices on the local iRODS server.
 *      The results are written to a KeyValPair_MS_T. For each pair the keyword is the MS name 
 *  while the value is the module where the microservice belongs. 
 *  Standard non-module microservices are listed as "core".
 * \param[in] 
 *    None.
 * \param[out] 
 *    outKVPairs - A KeyValPair_MS_T containing the results.
 * \return integer
 * \retval 0 on success
 * \sa
 * \post
 * \pre
 * \bug  no known bugs
**/
int
msiListEnabledMS(msParam_t *outKVPairs, ruleExecInfo_t *rei)
{
        FILE *radh;						/* reAction(dot)h */

        keyValPair_t *results;			/* the output data structure */

        char lineStr[LONG_NAME_LEN];	/* for line and string parsing */
        char modName[NAME_LEN];
        char *begPtr, *endPtr;


        /* For testing mode when used with irule --test */
        RE_TEST_MACRO ("    Calling msiEnabledMS")


        /* Sanity test */
        if (rei == NULL || rei->rsComm == NULL) {
                rodsLog (LOG_ERROR, "msiListEnabledMS: input rei or rsComm is NULL.");
                return (SYS_INTERNAL_NULL_INPUT_ERR);
        }


        /* Open reAction.h for reading */
        radh = fopen ("../re/include/reAction.h", "r");
        if(!radh) {
                rodsLog (LOG_ERROR, "msiListEnabledMS: unable to open reAction.h for reading.");
                return (UNIX_FILE_READ_ERR);
        }


        /* Skip the first part of the file */
        while (fgets(lineStr, LONG_NAME_LEN, radh) != NULL)
        {
                if (strstr(lineStr, "microsdef_t MicrosTable[]") == lineStr)
                {
                        break;
                }
        }


        /* Default microservices come first, will be listed are "core" */
        strncpy(modName, "core", NAME_LEN); /* Pad with null chars in the process */


        /* Allocate memory for our result struct */
        results = (keyValPair_t*)malloc(sizeof(keyValPair_t));
        memset (results, 0, sizeof (keyValPair_t));


        /* Scan microservice table one line at a time*/
        while (fgets(lineStr, LONG_NAME_LEN, radh) != NULL)
        {
                /* End of the table? */
                if (strstr(lineStr, "};") == lineStr)
                {
                        break;
                }

                /* Get microservice name */
                if ( (begPtr = strchr(lineStr, '\"')) && (endPtr = strrchr(lineStr, '\"')) )
                {
                        endPtr[0] = '\0';
                        addKeyVal(results, &begPtr[1], modName);
                }
                else 
                {
                        /* New Module? */
                        if (strstr(lineStr, "module microservices") )
                        {
                                /* Get name of module (between the first two spaces) */
                                begPtr = strchr(lineStr, ' ');
                                endPtr = strchr(++begPtr, ' ');
                                endPtr[0] = '\0';

                                strncpy(modName, begPtr, NAME_LEN-1);
                        }
                }
        }


        /* Done */
        fclose(radh);


        /* Send results out to outKVPairs */
        fillMsParam (outKVPairs, NULL, KeyValPair_MS_T, results, NULL);


        return 0;
}




