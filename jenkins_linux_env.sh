#!/bin/bash

# The environment for building and testing on Linux under Jenkins

set -e

export JENKINS_PGDATA=${JENKINS_PGDATA:?Jenkins PostgreSQL data variable was not set}

export BUILD_ROOT=${BUILD_ROOT:=$WORKSPACE}
source $BUILD_ROOT/linux_env.sh

unset PGHOME
unset PGCONF
unset PGCTL

tempdir=`mktemp -d 2> /dev/null || mktemp -d -t 'jenkins'`
if [ $? -ne 0 ]; then
    echo "$0: failed to create temporary directory, exiting ..."
    exit 1
fi

export PGDATA=$JENKINS_PGDATA
export IRODS_VAULT=$tempdir
