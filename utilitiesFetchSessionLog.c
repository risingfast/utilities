/*  utilitiesFetchSessionLog.c - CGI to fetch the web session log
 *  Author: Geoffrey Jarman
 *  Started: 24-Jul-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      24-Jul-2022 - started. by copying from bookCharacters.c 
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
char *sgUsername = "gjarman";                                                              // mysqlSerer logon username
char *sgPassword = "Mpa4egu$";                                                    // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
MYSQL_FIELD *fields;

int  iTitleID = 0;
char *sParams = NULL;
char *sSubstring = NULL;
char *sFilter =  NULL;
char caDelimiter[] = "&";
char caFilterTemp[MAXLEN] = {'\0'};
char caFilter[MAXLEN + 2] = {'\0'};

void fPrintResult(char *);

int main(void) {

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block -----------------------------------------------------------------------

    printf("Content-type: text/html\n\n");

// Initialize a connection and connect to the database ----------------------------------------------------------------

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

    sprintf(caSQL, "SELECT WS.`Session ID`, "
                    "       WU.`User Short Name`, "
                    "       WU.`User Full Name`, "
                    "       WS.`Session Datetime` "
                    " FROM risingfast.`Web Sessions` WS "
                    " LEFT JOIN risingfast.`Web Users` WU ON WS.`User ID` = WU.`User ID` "
                    " ORDER BY WS.`Session ID` DESC");

    fPrintResult(caSQL);
    
    return 0;
}

void fPrintResult(char *caSQL)
{
    int iColCount = 0;

    if(mysql_query(conn, caSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return;
    }

// store the result of the query

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);
        return;
    }
    
// fetch the number of fields in the result
    
    iColCount = mysql_num_fields(res);
    fields = mysql_fetch_fields(res);
    
    mysql_data_seek(res, 0);
    
// print the column headings

    for(int i = 0; i < iColCount; i++)
    {
        if (i - iColCount + 1 == 0) {
            printf("             %s", fields[i].name);
        } else {
            printf("%s  ", fields[i].name);
        }
    }
    printf("\n");

// print each row of results

    while((row = mysql_fetch_row(res)) != NULL)
    {
        for(int i = 0; i < iColCount; i++)
        {
            if (i == 0) {
                printf("%-10s  ", row[i]);
            } else if (i == 1) {
                printf("%-15s  ", row[i]);
            } else if (i == 2) {
                printf("%-22s       ", row[i]);
            } else if (i - iColCount + 1 == 0) {
                printf("%s (GMT - 5)", row[i]);
                printf("\n");
            } else {
                printf("%s ", row[i]);
            }
        }
    }

    mysql_free_result(res);
}
