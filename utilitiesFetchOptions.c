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

char *sgServer = "192.168.0.13";                                                           // mysqlServer LCL IP address
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
    return EXIT_SUCCESS;
}

void fPrintResult(char *strSQL)
{
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

    mysql_free_result(res);
    free(strSQL);
}
