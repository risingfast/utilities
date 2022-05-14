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
SQL2FLAGS=-L/usr/lib/x86_64-linux-gnu

all: environment serverProfile

environment: environment.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

serverProfile: serverProfile.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

clean:
	rm -f *.o *.s *.i environment serverProfile
