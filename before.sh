#!/bin/bash

set -ev

createuser -D -r -s icat
echo "ALTER USER icat WITH PASSWORD 'icat'" | psql
echo "ALTER USER postgres WITH PASSWORD '$PGPASSWORD'" | psql
