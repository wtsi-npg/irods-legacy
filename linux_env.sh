#!/bin/bash

export PGVERSION=${PGVERSION:?PostgreSQL version variable was not set}

export PGHOME=/usr/lib/postgresql/$PGVERSION
export PGDATA=/var/lib/postgresql/$PGVERSION/main
export PGCONF=/etc/postgresql/$PGVERSION/main/postgresql.conf
export PGCTL=$PGHOME/bin/pg_ctl
export PGPASSWORD=postgres
export ODBCINI=$BUILD_ROOT/odbc.ini
export IRODS_HOME=$BUILD_ROOT/iRODS
export IRODS_AUTH=$BUILD_ROOT/irods_auth.dat
export ICAT_CLEAN=$BUILD_ROOT/icat_dump.sql
export irodsEnvFile=$BUILD_ROOT/irods_env.conf
