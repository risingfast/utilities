# -*- makefile -*-
# makefile for mysql-c-library project
# Created: 15-Jun-2021
# Geoffrey Jarman
# Log
#       13-May-2022 environment
#       13-May-2022 serverProfile
#       20-Sep-2022 add .cgi targets

# $@ Target file
# $^ Dependency files
# $(CC) Compiler executable
# $(CFLAGS) Compiler options
# $@ Souce file
# $^ Dependent files (dependencies)

# Variables

CC=gcc
CFLAGS=-Wall -g -o
SQL1FLAGS=-I/usr/include/mysql
SQL2FLAGS=-L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -lssl -lcrypto -ldl -lresolv

all: authenticateUser authenticateUser.cgi setCornerImage setCornerImage.cgi environment environment.cgi serverProfile serverProfile.cgi utilitiesFetchOptions utilitiesFetchOptions.cgi utilitiesUpdateOptions utilitiesUpdateOptions.cgi saveCornerImageNumber saveCornerImageNumber.cgi fetchCornerImageNumber fetchCornerImageNumber.cgi utilitiesFetchAccessLog utilitiesFetchAccessLog.cgi authenticateUser authenticateUser.cgi utilitiesFetchSessionLog utilitiesFetchSessionLog.cgi file_stats checkAuthentication checkAuthentication.cgi

authenticateUer: authenticateUser.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

authenticateUer.cgi: authenticateUser.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

environment: environment.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

environment.cgi: environment.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

serverProfile: serverProfile.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

serverProfile.cgi: serverProfile.c
	$(CC) $(CFLAGS) $@ $^ $(SQL2FLAGS)

utilitiesFetchOptions: utilitiesFetchOptions.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

utilitiesFetchOptions.cgi: utilitiesFetchOptions.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

utilitiesUpdateOptions: utilitiesUpdateOptions.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

utilitiesUpdateOptions.cgi: utilitiesUpdateOptions.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

saveCornerImageNumber: saveCornerImageNumber.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

saveCornerImageNumber.cgi: saveCornerImageNumber.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

setCornerImage: setCornerImage.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

setCornerImage.cgi: setCornerImage.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

fetchCornerImageNumber: fetchCornerImageNumber.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

fetchCornerImageNumber.cgi: fetchCornerImageNumber.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

utilitiesFetchAccessLog: utilitiesFetchAccessLog.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

utilitiesFetchAccessLog.cgi: utilitiesFetchAccessLog.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

utilitiesFetchSessionLog: utilitiesFetchSessionLog.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

utilitiesFetchSessionLog.cgi: utilitiesFetchSessionLog.c ../shared/rf50.c ../shared/cs50.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS)

authenticateUser: authenticateUser.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS) -luuid

authenticateUser.cgi: authenticateUser.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS) -luuid

checkAuthentication: checkAuthentication.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS) -luuid

checkAuthentication.cgi: checkAuthentication.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS) -luuid

file_stats: file_stats.c
	$(CC) $(CFLAGS) $@ $(SQL1FLAGS) $^ $(SQL2FLAGS) -luuid

clean:
	rm -f *.o *.s *.i authenticateUser authenticateUser.cgi setCornerImage setCornerImage.cgi environment environment.cgi serverProfile serverProfile.cgi utilitiesFetchOptions utilitiesFetchOptions.cgi utilitiesUpdateOptions utilitiesUpdateOptions.cgi saveCornerImageNumber saveCornerImageNumber.cgi fetchCornerImageNumber fetchCornerImageNumber.cgi utilitiesFetchAccessLog utilitiesFetchAccessLog.cgi authenticateUser authenticateUser.cgi utilitiesFetchSessionLog utilitiesFetchSessionLog.cgi file_stats checkAuthentication checkAuthentication.cgi
