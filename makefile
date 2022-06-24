# -*- makefile -*-
# makefile for mysql-c-library project
# Created: 15-Jun-2021
# Geoffrey Jarman
# Log
#       13-May-2022 environment
#       13-May-2022 serverProfile
# $@ Target file
# $^ Dependency files
# $(CC) Compiler executable
# $(CFLAGS) Compiler options
# $@ Souce file
# $^ Dependent files (dependencies)

# Variables

CC=gcc
CFLAGS=-g -o
SQL1FLAGS=-I/usr/include/mysql
# SQL2FLAGS=-L/usr/lib/x86_64-linux-gnu
SQL2FLAGS=-L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl -lresolv

all: environment serverProfile utilitiesFetchOptions utilitiesUpdateOptions

environment: environment.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

serverProfile: serverProfile.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

utilitiesFetchOptions: utilitiesFetchOptions.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

utilitiesUpdateOptions: utilitiesUpdateOptions.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

clean:
	rm -f *.o *.s *.i environment serverProfile utilitiesFetchOptions utilitiesUpdateOptions
