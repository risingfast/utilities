//  authenticateUser.c - CGI to authenticate a user from their user_id and password and create a sesion cookie if none exists
//  Author: Geoff Jarman
//  Started: 08/06/2020
//  References:
//    https://www.codingame.com/playgrounds/14213/how-to-play-with-strings-in-c/string-split
//  Log:
//      07/26/2020 initial authentication with string comparison
//      08/07/2020 migrate to the web and tested out
//      08/07/2020 streamline exception checking in web code for http-posted data
//      08/07/2020 add string comparison logic
//      09/08/2020 add menu bar links
//      10/11/2020 center navigation links in main heading
//      10/24/2020 center the body of the html on the webpage
//      11/14/2020 reviewed all
//      04-Jan-2021 webcode
//      04-Jan-2021 remove HTML tags and re-write for grid layout
//      24-Sep-2021 tokenize GET string and check each value
//      25-Sep-2021 return username check and password check
//      10-Jun-2021 change the test username and test password
//      10-Jun-2021 remove the datafile definition as unused
//      17-Jul-2022 add a UUID cookie to the header and generate the UUID using a library package
//      17-Jul-2022 add a check for an existing cookie before authenticating
//      17-Jul-2022 add database check for a valid user and password
//      18-Jul-2022 add session table insert
//      18-Jul-2022 add cookie UUID to session record
//      29-Jul-2022 fix bug inserting new session record if no value user authenticated
//      16-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
//  Enhancements (0):
//

#include <mysql.h>
#include <stdio.h>
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
#define MAXLEN 1024
#define SQL_LEN 5000                                                      // max length of SQL query

// global declarations

char *sgServer = "192.168.0.13";                                        //mysqlServer LCL IP address
char *sgUsername = "gjarman";                                       // mysqlSerer LCL logon username
char *sgPassword = "Mpa4egu$";                                 // password to connect to mysqlserver
char *sgDatabase = "risingfast";                             // default database name on mysqlserver

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;

int main(void) {
    int    iUserID = 0;                                              // user id from web users table
    char   *sParams;                                                  // pointer to the QUERY_STRING
    char   *sSubstring;                                                  // pointer to the substring
    char   caDelimiter[] = "&";                                                   // token delimiter
    char   caUsername[60] = "\0";                                                  // username array
    char   caPassword[60] = "\0";                                                  // password array
    char   caSQL1[SQL_LEN] = {'\0'};                                // array to hold SQL query string
    char   caSQL2[SQL_LEN] = {'\0'};                                // array to hold SQL query string
    char   caCookieVal[UUIDTEXTSIZE];                                                 // UUID cookie
    bool   bCookieExists = false;                        // flag to indicate a session cookie exists
    bool   bUserIsAuthenticated = false;                   // flag to indicate user is authenticated
    uuid_t myUuid;                                               // raw or binary UUID type variable


//  setenv("QUERY_STRING", "Username=scook&Password=Sssp4atta", 1);
//  setenv("QUERY_STRING", "Username=gjarman&Password=Mpa4egu9", 1);

// check for an existing oookie named 'gj2020InstanceID' --------------------------------------------------------------

    if (getenv("HTTP_COOKIE") == NULL) {
        bCookieExists = false;
    } else if (strstr(getenv("HTTP_COOKIE"), "gj2020InstanceID") == NULL) {
        bCookieExists = false;
    } else {
        bCookieExists = true;
    }

//  get the content from QUERY_STRING and tokenize the username and password based on '&' character---------------------

    if (bCookieExists == false) {
        sParams = getenv("QUERY_STRING");

        sSubstring = strtok(sParams, caDelimiter);
        sscanf(sSubstring, "Username=%s", caUsername);

        sSubstring = strtok(NULL, caDelimiter);
        sscanf(sSubstring, "Password=%s", caPassword);
    }


// Initialize a connection and connect to the database if no cookie exists

    if (bCookieExists == false) {
    
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
    
        sprintf(caSQL1, "SELECT WU.`User ID`, WU.`User Password` "
                       "FROM risingfast.`Web Users` WU "
                       "WHERE WU.`User Short Name` = '%s'", caUsername);
    
        if(mysql_query(conn, caSQL1) != 0)
        {
            printf("\n");
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            return -1;
        }
    }

    // store the result of the query and test for no valid result

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
        if (row[1] == NULL) {
            bUserIsAuthenticated = false;
        }
        else if (strcmp(row[1], caPassword) == 0) {
            bUserIsAuthenticated = true;
            iUserID = atoi(row[0]);
        } else {
            bUserIsAuthenticated = false;
        }
    }
    
    //  create a new UUID if user is authenticated and the cookie gj2020InstanceID  does not exist ------------------------

    if ((bCookieExists == false) && (bUserIsAuthenticated == true)) {
        uuid_generate_random(myUuid);
        uuid_unparse(myUuid, caCookieVal);

    //  create a session record in web sessions table  with the new UUID (cookie value)

        sprintf(caSQL2, "INSERT INTO risingfast.`Web Sessions` "
                        "(`User ID`, `Session Cookie`) VALUES (%d, '%s')", iUserID, caCookieVal);

        if(mysql_query(conn, caSQL2) != 0)
        {
            printf("\n");
            printf("mysql_query() error in function %s():\n\n%s", __func__, mysql_error(conn));
            printf("\n\n");
            return -1;
        }
    }

//  create the HTTP headers for the cookie and the body text content-type and CORS

    if ((bCookieExists == false) && (bUserIsAuthenticated == true)) {
        printf("Set-Cookie: gj2020InstanceID=%s; HttpOnly\n", caCookieVal);
    }
    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

//  build response and output back to requesting javascript ------------------------------------------------------------

    if (bCookieExists == true) {
        printf("Already Authenticated");
    } 
    else if (bUserIsAuthenticated == true) {
        printf("Success - authenticated");
    }
    else if (bUserIsAuthenticated == false) {
        printf("Authentication failed");
    }

    return 0;
}
