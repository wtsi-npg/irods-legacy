/*** Copyright (c), The Regents of the University of California            ***
 *** For more information please refer to files in the COPYRIGHT directory ***/

/* rodsGenQueryNames.h - common header file for the generalized query names.
 * Maps the user specified strings to the corresponding #define values.
 */

#ifndef IRODS_GEN_QUERY_NAMES_H
#define IRODS_GEN_QUERY_NAMES_H

#include "rodsGenQuery.h"

typedef struct columnName {
   int columnId;
   char *columnName;
} columnName_t;

columnName_t columnNames[] = {
   COL_ZONE_ID,          "ZONE_ID", 
   COL_ZONE_NAME,        "ZONE_NAME",
   COL_USER_ID,          "USER_ID",
   COL_USER_NAME,        "USER_NAME",
   COL_USER_TYPE,        "USER_TYPE",
   COL_USER_ZONE,        "USER_ZONE",
   COL_USER_DN,          "USER_DN",
   COL_USER_INFO,        "USER_INFO",
   COL_USER_COMMENT,     "USER_COMMENT",
   COL_USER_CREATE_TIME, "USER_CREATE_TIME",
   COL_USER_MODIFY_TIME, "USER_MODIFY_TIME",

   COL_R_RESC_ID,     "RESC_ID",
   COL_R_RESC_NAME,   "RESC_NAME",
   COL_R_ZONE_NAME,   "RESC_ZONE_NAME",
   COL_R_TYPE_NAME,   "RESC_TYPE_NAME",
   COL_R_CLASS_NAME,  "RESC_CLASS_NAME",
   COL_R_LOC,         "RESC_LOC",
   COL_R_VAULT_PATH,  "RESC_VAULT_PATH",
   COL_R_FREE_SPACE,  "RESC_FREE_SPACE",
   COL_R_RESC_INFO,   "RESC_INFO",
   COL_R_RESC_COMMENT,"RESC_COMMENT",
   COL_R_CREATE_TIME, "RESC_CREATE_TIME",
   COL_R_MODIFY_TIME, "RESC_MODIFY_TIME",

   COL_D_DATA_ID,        "DATA_ID",
   COL_D_COLL_ID,        "DATA_COLL_ID",
   COL_DATA_NAME,        "DATA_NAME",
   COL_DATA_REPL_NUM,    "DATA_REPL_NUM",
   COL_DATA_VERSION,     "DATA_VERSION",
   COL_DATA_TYPE_NAME,   "DATA_TYPE_NAME",
   COL_DATA_SIZE,        "DATA_SIZE",
   COL_D_RESC_GROUP_NAME,"DATA_RESC_GROUP_NAME",
   COL_D_RESC_NAME,      "DATA_RESC_NAME",
   COL_D_DATA_PATH,      "DATA_PATH",
   COL_D_OWNER_NAME,     "DATA_OWNER_NAME",
   COL_D_OWNER_ZONE,     "DATA_OWNER_ZONE",
   COL_D_REPL_STATUS,    "DATA_REPL_STATUS",
   COL_D_DATA_STATUS,    "DATA_STATUS",
   COL_D_DATA_CHECKSUM,  "DATA_CHECKSUM",
   COL_D_EXPIRY,         "DATA_EXPIRY",
   COL_D_MAP_ID,         "DATA_MAP_ID",
   COL_D_COMMENTS,       "DATA_COMMENTS",
   COL_D_CREATE_TIME,    "DATA_CREATE_TIME",
   COL_D_MODIFY_TIME,    "DATA_MODIFY_TIME",

   COL_DATA_ACCESS_TYPE,     "DATA_ACCESS_TYPE",
   COL_DATA_ACCESS_NAME,     "DATA_ACCESS_NAME",
   COL_DATA_TOKEN_NAMESPACE, "DATA_TOKEN_NAMESPACE",
   COL_DATA_ACCESS_USER_ID,  "DATA_ACCESS_USER_ID",
   COL_DATA_ACCESS_DATA_ID,  "DATA_ACCESS_DATA_ID",

   COL_COLL_ID,            "COLL_ID",
   COL_COLL_NAME,          "COLL_NAME",
   COL_COLL_PARENT_NAME,   "COLL_PARENT_NAME",
   COL_COLL_OWNER_NAME,    "COLL_OWNER_NAME",
   COL_COLL_OWNER_ZONE,    "COLL_OWNER_ZONE",
   COL_COLL_MAP_ID,        "COLL_MAP_ID",
   COL_COLL_INHERITANCE,   "COLL_INHERITANCE",
   COL_COLL_COMMENTS,      "COLL_COMMENTS",
   COL_COLL_CREATE_TIME,   "COLL_CREATE_TIME",
   COL_COLL_MODIFY_TIME,   "COLL_MODIFY_TIME",

   COL_META_DATA_ATTR_NAME,  "META_DATA_ATTR_NAME",
   COL_META_DATA_ATTR_VALUE, "META_DATA_ATTR_VALUE",
   COL_META_DATA_ATTR_UNITS, "META_DATA_ATTR_UNITS",
   COL_META_DATA_ATTR_ID,    "META_DATA_ATTR_ID",
 
   COL_META_COLL_ATTR_NAME,  "META_COLL_ATTR_NAME",
   COL_META_COLL_ATTR_VALUE, "META_COLL_ATTR_VALUE",
   COL_META_COLL_ATTR_UNITS, "META_COLL_ATTR_UNITS",
   COL_META_COLL_ATTR_ID,    "META_COLL_ATTR_ID",

   COL_META_NAMESPACE_COLL,  "META_NAMESPACE_COLL",
   COL_META_NAMESPACE_DATA,  "META_NAMESPACE_DATA",
   COL_META_NAMESPACE_RESC,  "META_NAMESPACE_RESC",
   COL_META_NAMESPACE_USER,  "META_NAMESPACE_USER",

   COL_META_RESC_ATTR_NAME,  "META_RESC_ATTR_NAME",
   COL_META_RESC_ATTR_VALUE, "META_RESC_ATTR_VALUE",
   COL_META_RESC_ATTR_UNITS, "META_RESC_ATTR_UNITS",
   COL_META_RESC_ATTR_ID,    "META_RESC_ATTR_ID",

   COL_META_USER_ATTR_NAME,  "META_USER_ATTR_NAME",
   COL_META_USER_ATTR_VALUE, "META_USER_ATTR_VALUE",
   COL_META_USER_ATTR_UNITS, "META_USER_ATTR_UNITS",
   COL_META_USER_ATTR_ID,    "META_USER_ATTR_ID",

   COL_RESC_GROUP_RESC_ID,   "RESC_GROUP_RESC_ID",
   COL_RESC_GROUP_NAME,      "RESC_GROUP_NAME",

   COL_USER_GROUP_ID,        "USER_GROUP_ID",
   COL_USER_GROUP_NAME,      "USER_GROUP_NAME",

   COL_RULE_EXEC_ID,                "RULE_EXEC_ID",
   COL_RULE_EXEC_NAME,              "RULE_EXEC_NAME",
   COL_RULE_EXEC_REI_FILE_PATH,     "RULE_EXEC_REI_FILE_PATH",
   COL_RULE_EXEC_USER_NAME,         "RULE_EXEC_USER_NAME",
   COL_RULE_EXEC_ADDRESS,           "RULE_EXEC_ADDRESS",
   COL_RULE_EXEC_TIME,              "RULE_EXEC_TIME",
   COL_RULE_EXEC_FREQUENCY,         "RULE_EXEC_FREQUENCY",
   COL_RULE_EXEC_PRIORITY,          "RULE_EXEC_PRIORITY",
   COL_RULE_EXEC_ESTIMATED_EXE_TIME,"RULE_EXEC_ESTIMATED_EXE_TIME",
   COL_RULE_EXEC_NOTIFICATION_ADDR, "RULE_EXEC_NOTIFICATION_ADDR",
   COL_RULE_EXEC_LAST_EXE_TIME,     "RULE_EXEC_LAST_EXE_TIME",
   COL_RULE_EXEC_STATUS,            "RULE_EXEC_STATUS",

   COL_TOKEN_NAMESPACE, "TOKEN_NAMESPACE",
   COL_TOKEN_ID,        "TOKEN_ID",
   COL_TOKEN_NAME,      "TOKEN_NAME",
   COL_TOKEN_VALUE,     "TOKEN_VALUE",
   COL_TOKEN_VALUE2,    "TOKEN_VALUE2",
   COL_TOKEN_VALUE3,    "TOKEN_VALUE3",
   COL_TOKEN_COMMENT,   "TOKEN_COMMENT",
};

int NumOfColumnNames = sizeof(columnNames) / sizeof(columnName_t);

#endif	/* IRODS_GEN_QUERY_NAMES_H */
