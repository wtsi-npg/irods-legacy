#!/bin/bash

PGVERSION=${PGVERSION:?PostgreSQL version variable was not set}

PGHOME=/usr/lib/postgresql/$PGVERSION
PGDATA=/var/lib/postgresql/$PGVERSION/main
PGCONF=/etc/postgresql/$PGVERSION/main/postgresql.conf
PGCTL=$PGHOME/bin/pg_ctl
PGPASSWORD=postgres
ODBCINI=$BUILD_ROOT/odbc.ini
IRODS_HOME=$BUILD_ROOT/iRODS
IRODS_AUTH=$BUILD_ROOT/irods_auth.dat
ICAT_CLEAN=$BUILD_ROOT/icat_dump.sql
irodsEnvFile=$BUILD_ROOT/irods_env.conf
