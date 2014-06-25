#!/bin/bash

BUILD_ROOT=${BUILD_ROOT:=$TRAVIS_BUILD_DIR}

source $BUILD_ROOT/linux_env.sh

PGDATA=/var/ramfs/postgresql/$PGVERSION/main
