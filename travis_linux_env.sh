#!/bin/bash

set -e

export BUILD_ROOT=${BUILD_ROOT:=$TRAVIS_BUILD_DIR}

source $BUILD_ROOT/linux_env.sh

export PGDATA=/var/ramfs/postgresql/$PGVERSION/main
