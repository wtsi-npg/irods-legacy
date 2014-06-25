#!/bin/bash

# Smoke test the build by starting the server and connecting a client.

set -e

$TRAVIS_BUILD_DIR/irodscontrol istart
sleep 10

echo irods | script -q -c "$IRODS_HOME/clients/icommands/bin/iinit" > /dev/null
$IRODS_HOME/clients/icommands/bin/ienv
$IRODS_HOME/clients/icommands/bin/ils

$TRAVIS_BUILD_DIR/irodscontrol istop
