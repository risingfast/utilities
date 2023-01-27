/*  utilitiesUpdateOptions.c - CGI to update webpage options
 *  Author: Geoffrey Jarman
 *  Started: 21-Jun-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      21-Jun-2022 start by copying bookAddBook.c and modifying
 *      14-Aug-2022 add leftlinks setting
 *      15-Sep-2022 add Access-Control-Allow-Origin: * CORS header
 *      03-Oct-2022 change 'Show Current User' to 'Show Session Log'
 *      26-Oct-2022 clean up comments
 *      26-Oct-2022 extend MySQL initialization and shutdown operations to fix memory leaks
 *      07-Nov-2022 change sprintf() to asprintf()
 *      16-Nov-2022 change strcpy() to strncpy()
 *      22-Jan-2023 set freed pointers to NULL
 *  Enhancements:
*/

#define _GNU_SOURCE
#define MAXLEN 1024

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

// global declarations -------------------------------------------------------------------------------------------------

char *sgServer = "192.168.0.13";                                                                //mysqlServer IP address
char *sgUsername = "gjarman";                                                              // mysqlServer logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char *sParam = NULL;
char *sCharacter = NULL;
char caBehaviour[MAXLEN + 1] = {'\0'};
char caBackground[MAXLEN + 1] = {'\0'};
char caShowUser[MAXLEN + 1] = {'\0'};
char caShowLog[MAXLEN + 1] = {'\0'};
char caLeftLinks[MAXLEN + 1] = {'\0'};
char caStringBuf[MAXLEN + 1] = {'\0'};
char *sTemp = NULL;
char caDelimiter[] = "&";

int main(void) {

    char *strSQL = NULL;

// print the html content-type and CORS http header block -------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// check for a NULL query string --------------------------------------------------------------------------------------

//    setenv("QUERY_STRING", "behaviour=Randomized&background=backgroundFlowersOnFabric&showuser=Yes&showlog=No&leftlinks=Long", 1);   // uncomment for testing only

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("Query string is empty. Expecting QUERY_STRING=\"behaviour=<>&background=<>&showUser=<>&showLog=<>&leftLinks=<>\". Terminating utiltiesUpdateOptions.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character----------------------------------------------

    sTemp = strtok(sParam, caDelimiter);
    sscanf(sTemp, "behaviour=%[^\n]s", caStringBuf);
    sTemp = fUrlDecode(caStringBuf);
    strncpy(caBehaviour, sTemp, MAXLEN);
    free(sTemp);
    sTemp = NULL;

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "background=%[^\n]s", caStringBuf);
    sTemp = fUrlDecode(caStringBuf);
    strncpy(caBackground, sTemp, MAXLEN);
    free(sTemp);
    sTemp = NULL;

    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "showuser=%[^\n]s", caStringBuf);
    sTemp = fUrlDecode(caStringBuf);
    strncpy(caShowUser, sTemp, MAXLEN);
    free(sTemp);
    sTemp = NULL;


    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "showlog=%[^\n]s", caStringBuf);
    sTemp = fUrlDecode(caStringBuf);
    strncpy(caShowLog, sTemp, MAXLEN);
    free(sTemp);
    sTemp = NULL;


    sTemp = strtok(NULL, caDelimiter);
    sscanf(sTemp, "leftlinks=%[^\n]s", caStringBuf);
    sTemp = fUrlDecode(caStringBuf);
    strncpy(caLeftLinks, sTemp, MAXLEN);
    free(sTemp);
    sTemp = NULL;


    printf("Behavior=%s, Background=%s, ShowUser=%s, ShowLog=%s\n\n", caBehaviour, caBackground, caShowUser, caShowLog);  // uncomment for testing only

// * initialize the MySQL client library -------------------------------------------------------------------------------

   if (mysql_library_init(0, NULL, NULL)) {
       printf("Cannot initialize MySQL Client library\n");

       return EXIT_FAILURE;
   }

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

// set a SQL query and update corner image behaviour ------------------------------------------------------------------

    asprintf(&strSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Corner Image Choice';", caBehaviour);


    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

// set a SQL query and update background image choice------------------------------------------------------------------

    asprintf(&strSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Background Choice';", caBackground);


    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

// set a SQL query and update show current user -----------------------------------------------------------------------

    asprintf(&strSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Show Session Log';", caShowUser);


    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

// set a SQL query and update show access log -------------------------------------------------------------------------

    asprintf(&strSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Show Server Log';", caShowLog);


    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

// set a SQL query and update left links setting ----------------------------------------------------------------------

    asprintf(&strSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = '%s' "
                   "WHERE `Option Name` = 'Left Links';", caLeftLinks);


    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

// free memory allocated to result set 'res' ---------------------------------------------------------------------------

    mysql_free_result(res);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();
    free(strSQL);
    strSQL = NULL;

    return EXIT_SUCCESS;
}
