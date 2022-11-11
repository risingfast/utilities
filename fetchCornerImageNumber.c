/*  fetchCornerImageNumber.c - CGI to fetch the corner image number from mySQL
 *  Author: Geoffrey Jarman
 *  Started: 25-Jun-2022
 *  References:
 *  Log:
 *      25-Jun-2022 copied from utilitiesFetchOptions.c
 *      15-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
 *      05-Nov-2022 use asprintf() instead of sprintf to define strSQL
 *  Enhancements:
*/

#define _GNU_SOURCE

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

#define SQL_LEN 5000
#define HDG_LEN 1000

#define MAXLEN 1024

// global declarations -------------------------------------------------------------------------------------------------

char *sgServer = "192.168.0.13";                                                            //mysqlServer LCL IP address
char *sgUsername = "gjarman";                                                           // mysqlSerer LCL logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

char caText[MAXLEN] = {'\0'};
char caTopic[MAXLEN] = {'\0'};
char *sTopic = NULL;
char caFilterTemp[MAXLEN] = {'\0'};
char caFilter[MAXLEN + 2] = {'\0'};
char *sFilter = NULL;
char *sParams = NULL;
char *sSubstring = NULL;
char caDelimiter[] = "&";

void fPrintResult(char *, char *, char *);

int main(void) {

    char *strSQL = NULL;

// print the html content type header and CORS header block ------------------------------------------------------------

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

// set a SQL query to select the corner image number from the options table --------------------------------------------

    asprintf(&strSQL, "SELECT WO.`Option Setting` "
                    "FROM risingfast.`Web Options` WO "
                    "WHERE `Option Name` = 'Corner Image Number';");

    fPrintResult(sTopic, sFilter, strSQL);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

    free(strSQL);
    return 0;
}

// function to execcute the sql and print the corner image number result -----------------------------------------------

void fPrintResult(char *caTopic, char *caFilter, char *strSQL)
{
    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return;
    }

// store the result of the query ---------------------------------------------------------------------------------------

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);
        return;
    }
    
// fetch the number of fields in the result ----------------------------------------------------------------------------
    
    mysql_data_seek(res, 0);
    
// print each row of results -------------------------------------------------------------------------------------------

     row = mysql_fetch_row(res);
     printf("%4s", row[0]);
     printf("\n");

     mysql_free_result(res);
}
