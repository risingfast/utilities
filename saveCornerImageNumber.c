/*  saveCornerImageNumber.c - CGI to update the corner image number on mySQL
 *  Author: Geoffrey Jarman
 *  Started: 25-Jun-2022
 *  References:
 *  Log:
 *      25-Jun-2022 start by copying utilitiesUpdateOptions.c and modifying
 *      16-Sep-2022 add Access-Control-Allow-Origin: * http CORS header
 *      07-Nov-2022 replace sprintf(() with asprintf()
 *      07-Nov-2022 clean up return values
 *      22-Jan-2023 set freed pointer to NULL
 *  Enhancements:
*/

#define MAXLEN 1024
#define _GNU_SOURCE

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

// global declarations -------------------------------------------------------------------------------------------------

char *sgServer = "192.168.0.13";                                                                //mysqlServer IP address
char *sgUsername = "gjarman";                                                               // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

char *sParam = NULL;
char *sCharacter = NULL;
int  iNumber = 0;
char caDelimiter[] = "&";

int main(void) {

    char *strSQL = NULL;

// check for a NULL query string ---------------------------------------------------------------------------------------

    sParam = getenv("QUERY_STRING");

    if(sParam == NULL) {
        printf("\n");
        printf("Query string is empty. Expecting QUERY_STRING=\"number=<99>\". Terminating saveCornerImageNumber.cgi");
        printf("\n\n");
        return EXIT_FAILURE;
    }

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sscanf(sParam, "number=%d", &iNumber);

// test for an empty QUERY_STRING --------------------------------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
        return EXIT_FAILURE;
    }

// print the html content-type and CORS header block -------------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// * initialize the MySQL client library -------------------------------------------------------------------------------

   if (mysql_library_init(0, NULL, NULL)) {
       printf("Cannot initialize MySQL Client library\n");
       return EXIT_FAILURE;
   }

// Initialize a connection and connect to the database -----------------------------------------------------------------

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

// set a SQL query and update corner image number ----------------------------------------------------------------------

    asprintf(&strSQL, "UPDATE risingfast.`Web Options` "
                   "SET `Option Setting` = %d "
                   "WHERE `Option Name` = 'Corner Image Number';", iNumber);

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

    mysql_free_result(res);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();
    free(strSQL);
    strSQL = NULL;

    return EXIT_SUCCESS;
}
