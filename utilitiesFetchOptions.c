/*  utilitiesFetchOptions.c - CGI to fetch options from mySQL
 *  Author: Geoffrey Jarman
 *  Started: 20-Jan-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      04-Jan-2022 copied from bookInquiry2.c
 *      04-Oct-2022 add Acces-Control-Allow-Origin: * CORS header
 *      06-Nov-2022 changes sprintf() to as asprintf()
 *      07-Nov-2022 clean up comments and return arguments
 *      07-Nov-2022 remove unused variable declarations
 *      20-Nov-2022 free resources used by mySQL library
 *      16-Apr-2023 change sgServer from 192.168.0.13 to 192.168.1.64
 *      20-Apr-2023 change sgServer from 192.168.1.64 to 35.212.209.127
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

#define HDG_LEN 1000
#define MAXLEN 1024

// global declarations -------------------------------------------------------------------------------------------------

char *sgServer = "35.212.209.127";                                                         // mysqlServer LCL IP address
char *sgUsername = "gjarman";                                                          // mysqlServer LCL logon username
char *sgPassword = "Mpa4egu$";                                                     // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                 // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

void fPrintResult(char *);

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

    asprintf(&strSQL, "SELECT WO.`Option ID` as 'ID' "
                   ", WO.`Option Name` as 'Name' "
                   ", WO.`Option Setting` as 'Setting' "
                   "FROM risingfast.`Web Options` WO ");

    fPrintResult(strSQL);

// free rosources used by strSQL ---------------------------------------------------------------------------------------4dd

    free(strSQL);

// * free resources used by the 'res' array ----------------------------------------------------------------------------

    mysql_free_result(res);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

    return EXIT_SUCCESS;
}

void fPrintResult(char *strSQL) {
    int iColCount = 0;

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
    
    iColCount = mysql_num_fields(res);
    
    mysql_data_seek(res, 0);
    
// print each row of results -------------------------------------------------------------------------------------------

     while((row = mysql_fetch_row(res)) != NULL)
    {
        for (int i = 0; i < iColCount; i++)
        {
            if(i == 0)
            {
                printf("%4s,", row[i]);
            }
            else if (i == iColCount - 1)
            {
                if (row[i] != NULL) {
                    printf("%s", row[i]);
                } else {
                    ;
                }
            }
            else {
                if (row[i] != NULL) {
                    printf("%s,", row[i]);
                } else {
                    ;
                }
            }
        }
        printf("\n");
    }
    return;
}
