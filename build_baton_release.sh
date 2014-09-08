#!/bin/bash

# Build baton against irods-legacy libraries

set -e

# Already set by .travis.yml...
IRODS_HOME=$IRODS_HOME source ./scripts/set_irods_home.sh
echo "iRODS \$HOME is currently set to: $IRODS_HOME"

# Generate the configure script
autoreconf -i

# Generate the makefiles (see INSTALL for arguments to configure)
./configure

# Compile
make

# Install, including HTML manual and manpage.
make clean install

./baton
