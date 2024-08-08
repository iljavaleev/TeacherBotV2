#!/bin/bash
psql -U postgres -tc "SELECT 1 FROM pg_database WHERE datname = '${POSTGRES_DB}'" | grep -q 1 || psql -U  ${POSTGRES_USER}  -c "CREATE DATABASE '${POSTGRES_DB}'"
psql -U ${POSTGRES_USER} -d ${POSTGRES_DB} -a -f ./db_setup.sql