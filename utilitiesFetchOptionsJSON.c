/*  utilitiesFetchOptionsJSON.c - CGI to fetch options from mySQL in JSON format
 *  Author: Geoffrey Jarman
 *  Started: 20-Nov-2022
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      20-Nov-2022 copied from utiltiesFetchOptions.c
 *      20-Nov-2022 remove the function fPrintResult() and inline with main()
 *      20-Nov-2022 add the json-c library calls and test the output
 *      22-Jan-2033 set freed pointers to NULL
 *  Enhancements:
*/

#define _GNU_SOURCE
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "json-c/json.h"
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

    int intColCnt = 0;
    int intRowCnt = 0;
    int intCurRow = 0;
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
        return EXIT_FAILURE;
    }

    asprintf(&strSQL, "SELECT WO.`Option ID` as 'ID' "
                   ", WO.`Option Name` as 'Name' "
                   ", WO.`Option Setting` as 'Setting' "
                   "FROM risingfast.`Web Options` WO ");

    if(mysql_query(conn, strSQL) != 0)
    {
        printf("\n");
        printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
        printf("\n\n");
        return EXIT_FAILURE;
    }

// store the result of the query ---------------------------------------------------------------------------------------

    res = mysql_store_result(conn);
    if(res == NULL)
    {
        printf("%s() -- no results returned", __func__);
        printf("\n");

        mysql_free_result(res);
        return EXIT_FAILURE;
    }
    
// fetch the number of fields in the result and seek to the first row of results ---------------------------------------
    
    intColCnt = mysql_num_fields(res);
    intRowCnt = mysql_num_rows(res);
    
    mysql_data_seek(res, 0);
    
// create a json object ------------------------------------------------------------------------------------------------

    json_object *jsonOptions = json_object_new_object();
    json_object *jsonOptionNumber;
    json_object *jsonOptionName;
    json_object *jsonOptionValue;


// print each row of results -------------------------------------------------------------------------------------------

     printf("{\"Options\":[\n");
     while((row = mysql_fetch_row(res)) != NULL) {
        for (int i = 0; i < intColCnt; i++)
        {
            if(i == 0)
            {
                jsonOptionNumber = json_object_new_string(row[i]);
            }
            else if (i == intColCnt - 1)
            {
                if (row[i] != NULL) {
                    jsonOptionName = json_object_new_string(row[i]);
                } else {
                    ;
                }
            }
            else {
                if (row[i] != NULL) {
                    jsonOptionValue = json_object_new_string(row[i]);
                }
            }
        }
        json_object_object_add(jsonOptions, "Number", jsonOptionNumber);
        json_object_object_add(jsonOptions, "Name", jsonOptionName);
        json_object_object_add(jsonOptions, "Value", jsonOptionValue);
        printf("%s", json_object_to_json_string(jsonOptions));
        if (intCurRow == intRowCnt - 1) {
            printf("\n");
        } else {
            printf(",\n");
        }
        intCurRow++;
    }
     printf("]}\n");
    
// free resources used by the JSON datastructure -----------------------------------------------------------------------

    free(jsonOptions);
    jsonOptions = NULL;

// free resources used by strSQL ---------------------------------------------------------------------------------------

    free(strSQL);
    strSQL = NULL;

// * free resources used by the 'res' array ----------------------------------------------------------------------------

    mysql_free_result(res);

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

    mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

    mysql_library_end();

    return EXIT_SUCCESS;
}
