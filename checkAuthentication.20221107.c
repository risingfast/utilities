//  checkAuthentication.c -  CGI to check authentication of a session user via their session cookie
//  Author: Geoff Jarman
//  Started: 07/18/2022
//  References: 
//      https://computer.howstuffworks.com/cgi3.htm
//  Log:
//      18-Jul-2022 -- started by copying from simplest.c
//      21-Jul-2022 -- add unknown response for non-authenticated user
//      20-Oct-2022 extend MySQL initialization and shutdown operations
//  Enhancements(0)

#include <stdio.h>
#include <mysql.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <uuid/uuid.h>
#include <string.h>
#include <ctype.h>
#include "../shared/rf50.h"

#define HTML_LEN 60                                     // Max length of input string from html form
#define CGI_LEN HTML_LEN + 6                                // 5 for "data=" and 1 for trailing NULL
#define UUIDTEXTSIZE sizeof(uuid_t)*2 + 5                         // size of formatted characer UUID
#define MAX_LEN 1024
#define SQL_LEN 5000                                                      // max length of SQL query

char *sgServer = "192.168.0.13";                                        //mysqlServer LCL IP address
char *sgUsername = "gjarman";                                       // mysqlSerer LCL logon username
char *sgPassword = "Mpa4egu$";                                 // password to connect to mysqlserver
char *sgDatabase = "risingfast";                             // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

int main(void) {

    bool bCookieExists = false;                          // flag to indicate a session cookie exists
    char caEnvVars[MAX_LEN] = {'\0'};
    char caCookieVal[MAX_LEN] = {'\0'};                 // cookie value
    char caSessionID[MAX_LEN] = {'\0'};
    char caUserID[MAX_LEN] = {'\0'};
    char caDateTime[MAX_LEN] = {'\0'};
    char caUserShortName[MAX_LEN] = {'\0'};
    char caUserFullName[MAX_LEN] = {'\0'};
    char caUserEmail[MAX_LEN] = {'\0'};
    char caSQL1[SQL_LEN] = {'\0'};
    char *token;

//    setenv("HTTP_COOKIE", "cookie1=18afes7a7; cookie2=28afes7a8; cookie4=48afes710; gj2020InstanceID=b471bfac-122b-4053-84e3-dddb374619ff; G_ENABLED_IDPS=google", 1);
//    setenv("HTTP_COOKIE", "cookie2=28afes7a8; cookie4=48afes710; gj2020InstanceID=b471bfac-122b-4053-84e3-dddb374619ff; G_ENABLED_IDPS=google; cookie1=18afes7a7", 1);
//    setenv("HTTP_COOKIE", "cookie2=28afes7a8; cookie4=48afes710; G_ENABLED_IDPS=google; cookie1=18afes7a7", 1);

// print the html content type and <head> block -----------------------------------------------------------------------

   printf("Content-type: text/html\n");
   printf("Access-Control-Allow-Origin: *\n\n");

// check for and fetch any existing oookie named 'gj2020InstanceID' ----------------------------------------------------

    if (getenv("HTTP_COOKIE") == NULL) {
        printf("Content-type: text/html\n\n");
        printf("Failure\n");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        return -1;
    } else if (strcpy(caEnvVars, getenv("HTTP_COOKIE")) == NULL) {
        bCookieExists = false;
    } else if (strstr(caEnvVars, "gj2020InstanceID") == NULL) {
        bCookieExists = false;
    } else {
        bCookieExists = true;
        token = strtok(caEnvVars, "; ");
        while (token != NULL) {
            if(strstr(token, "gj2020InstanceID") != NULL) {
                sscanf(token, "gj2020InstanceID=%s", caCookieVal);
            }
            token = strtok(NULL, "; ");
        }
    }

// fetch the username and session profile matching the session cookie

    if (bCookieExists) {

// * initialize the MySQL client library -------------------------------------------------------------------------------

        if (mysql_library_init(0, NULL, NULL)) {
            printf("Cannot initialize MySQL Client library\n");
            return EXIT_FAILURE;
        }

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

        sprintf(caSQL1, "SELECT WS.`Session ID`, "
                        "       WS.`User ID`, "
                        "       WU.`User Short Name`, "
                        "       WU.`User Full Name`, "
                        "       WU.`User Email`, "
                        "       WS.`Session Datetime` "
                        " FROM risingfast.`Web Sessions` WS "
                        " LEFT JOIN risingfast.`Web Users` WU ON WS.`User ID` = WU.`User ID` "
                        " WHERE WS.`Session Cookie` = '%s'", caCookieVal);

        if(mysql_query(conn, caSQL1) != 0)
        {
            printf("\n");
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            return EXIT_FAILURE;
        }

    // store the result of the query

        res = mysql_store_result(conn);
        if(res == NULL)
        {
            printf("%s() -- no results returned", __func__);
            printf("\n");

            mysql_free_result(res);
            return -1;
        }

        mysql_data_seek(res, 0);


    // fetch the password for the user from the database and validate against the password input

        if((row = mysql_fetch_row(res)) != NULL)
        {
            strcpy(caSessionID, row[0]);
            strcpy(caUserID, row[1]);
            strcpy(caUserShortName, row[2]);
            strcpy(caUserFullName, row[3]);
            strcpy(caUserEmail, row[4]);
            strcpy(caDateTime, row[5]);
        }

// * close the database connection created by mysql_init(NULL) ---------------------------------------------------------

        mysql_free_result(res);
        mysql_close(conn);

// * free resources used by the MySQL library --------------------------------------------------------------------------

        mysql_library_end();

    return EXIT_SUCCESS;
    }

// print the html page content type and <head> block -------------------------------------------------------------------

    if (bCookieExists) {
        printf("Success\n");
        printf("%s\n", caSessionID);
        printf("%s\n", caUserShortName);
        printf("%s\n", caUserFullName);
        printf("%s\n", caUserEmail);
        printf("%s\n", caDateTime);
        printf("%s\n", caCookieVal);
    } else {
        printf("Failure\n");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
        printf("%s\n", "Unknown");
    }

    return EXIT_SUCCESS;
}
