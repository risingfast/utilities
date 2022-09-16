/*  utilitiesFetchOptions.c - CGI to fetch options from mySQL
 *  Author: Geoffrey Jarman
 *  Started: 20-Jan-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      04-Jan-2022 copied from bookInquiry2.c
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
#define HDG_LEN 1000

#define MAXLEN 1024

// global declarations

char *sgServer = "192.168.0.13";                                                            //mysqlServer LCL IP address 
// char *sgServer = "35.188.123.150";                                                             //mysqlServer GCP IP address
char *sgUsername = "gjarman";                                                           // mysqlSerer LCL logon username
//char *sgUsername = "root";                                                                 // mysqlSerer GCP logon username
char *sgPassword = "Mpa4egu$";                                                    // password to connect to mysqlserver
char *sgDatabase = "risingfast";                                                // default database name on mysqlserver

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

    char caSQL[SQL_LEN] = {'\0'};

// print the html content type and <head> block -----------------------------------------------------------------------

    printf("Content-type: text/html\n\n");

// Initialize a connection and connect to the database$$

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

    sprintf(caSQL, "SELECT WO.`Option ID` as 'ID' "
                   ", WO.`Option Name` as 'Name' "
                   ", WO.`Option Setting` as 'Setting' "
                   "FROM risingfast.`Web Options` WO ");
    fPrintResult(sTopic, sFilter, caSQL);
    return 0;
}

void fPrintResult(char *caTopic, char *caFilter, char *caSQL)
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
    
    mysql_data_seek(res, 0);
    
// print each row of results

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
}
