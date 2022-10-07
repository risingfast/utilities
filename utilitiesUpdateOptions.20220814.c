/*  utilitiesUpdateOptions.c - CGI to update webpage options
 *  Author: Geoffrey Jarman
 *  Started: 21-Jun-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      21-Jun-2022 start by copying bookAddBook.c and modifying
 *  Enhancements:
*/

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

#define SQL_LEN 5000

#define MAXLEN 1024

// global declarations

char *sgServer = "192.168.0.13";                                                               //mysqlServer IP address
// char *sgServer = "35.188.123.150"                                                              // mysqlServer GCP IP address
char *sgUsername = "gjarman";                                                              // mysqlSerer logon username
// char *sgUserName = "root";                                                                     // mysqlServer GCP logon name
char *sgPassword = "Mpa4egu$";                                                    // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char *sParam = NULL;
char *sCharacter = NULL;
char caBehaviour[MAXLEN] = {'\0'};
char caBackground[MAXLEN] = {'\0'};
char caShowUser[MAXLEN] = {'\0'};
char caShowLog[MAXLEN] = {'\0'};
char caStringBuf[MAXLEN] = {'\0'};
char *sTemp = NULL;
char caDelimiter[] = "&";

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block -----------------------------------------------------------------------

    printf("Content-type: text/html\n\n");

// Initialize a connection and connect to the database

    conn = mysql_init(NULL);

    if (!mysql_real_connect(conn, sgServer, sgUsername, sgPassword, sgDatabase, 0, NULL, 0))
    {
        printf("\n");
        printf("Failed to connect to MySQL Server %s in module %s()", sgServer, __func__);
        printf("\n\n");
        printf("Error: %s\n", mysql_error(conn));
        printf("\n");
        return  EXIT_FAILURE;
    }

// check for a NULL query string -------------------------------------------------------------------------------------=

//    setenv("QUERY_STRING", "behaviour=Randomized&background=Test%20Flowers&showuser=Suppressed&&showlog=No", 1);

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("\n");
        printf("Query string is empty. No parameters passed. Terminating program");
        printf("\n\n");
        return 1;
    }

//    printf("QUERY_STRING: %s", getenv("QUERY_STRING"));
//    printf("\n\n");

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sTemp = strtok(sParam, caDelimiter);
    sscanf(sTemp, "behaviour=%[^\n]s", caStringBuf);
    strcpy(caBehaviour, fUrlDecode(caStringBuf));

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "background=%[^\n]s", caStringBuf);
    strcpy(caBackground, fUrlDecode(caStringBuf));

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "showuser=%[^\n]s", caStringBuf);
    strcpy(caShowUser, fUrlDecode(caStringBuf));

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "showlog=%[^\n]s", caStringBuf);
    strcpy(caShowLog, fUrlDecode(caStringBuf));

//    printf("Behavior=%s, Background=%s, ShowUser=%s, ShowLog=%s\n\n", caBehaviour, caBackground, caShowUser, caShowLog);

// test for an empty QUERY_STRING -------------------------------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
        return 0;
    }

// set a SQL query and update corner image behaviour ------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Corner Image Choice';", caBehaviour);

//    printf("SQL: %s\n\n", caSQL);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

// set a SQL query and update background image choice------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Background Choice';", caBackground);

//    printf("SQL: %s\n\n", caSQL);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

// set a SQL query and update show current user -----------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Show Current User';", caShowUser);

//    printf("SQL: %s\n\n", caSQL);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

// set a SQL query and update show access log -------------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Show Access Log';", caShowLog);

//    printf("SQL: %s\n\n", caSQL);

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return -1;
    }

    mysql_free_result(res);

    return 0;
}
