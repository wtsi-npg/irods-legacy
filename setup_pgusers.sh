#!/bin/bash

# Set up the required PostgreSQL user accounts.

set -e

createuser -D -r -s icat
echo "ALTER USER icat WITH PASSWORD 'icat'" | psql
echo "ALTER USER postgres WITH PASSWORD '$PGPASSWORD'" | psql
