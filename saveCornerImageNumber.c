/*  saveCornerImageNumber.c - CGI to update the corner image number on mySQL
 *  Author: Geoffrey Jarman
 *  Started: 25-Jun-2022
 *  References:
 *  Log:
 *      25-Jun-2022 start by copying utilitiesUpdateOptions.c and modifying
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
int  iNumber = 0;
char caDelimiter[] = "&";

int main(void) {

    int i;
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

//    setenv("QUERY_STRING", "number=35", 1);

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("\n");
        printf("Query string is empty. No parameters passed. Terminating program");
        printf("\n\n");
        return 1;
    }

//    printf("Test QUERY_STRING: %s", getenv("QUERY_STRING"));
//    printf("\n\n");

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sscanf(sParam, "number=%d", &iNumber);

//    printf("Test Result: number=%d\n\n", iNumber);

// test for an empty QUERY_STRING -------------------------------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
        return 0;
    }

// set a SQL query and update corner image number ---------------------------------------------------------------------

    sprintf(caSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = %d "
                   "WHERE `Option Name` = 'Corner Image Number';", iNumber);

    printf("SQL used: %s\n\n", caSQL);

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
