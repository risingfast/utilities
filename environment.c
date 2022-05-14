/*  environment.c - CGI to show getenv() variables on a webpage
 *  Author: Geoffrey Jarman
 *  Started: 09/22/2021
 *  References:
 *      http://www6.uniovi.es/cscene/topics/web/cs2-12.xml.html
 *  Log:
 *      09/24/2020 copy code from numVars.c
 *      09/21/2021 remove html formatting on output
 *      09/21/2021 fetch values specified by action
 *      10/16/2021 change formatting on outut lines
 *  Enhancements:
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

char evars[20][80]={"SERVER_SOFTWARE", "SERVER_NAME", "SERVER_PROTOCOL", 
                    "SERVER_PORT",
                    "GATEWAY_INTERFACE", "REQUEST_METHOD", 
                    "PATH_INFO", "PATH_TRANSLATED", "SCRIPT_NAME", 
                    "QUERY_STRING", 
                    "REMOTE_HOST", "REMOTE_ADDR", "REMOTE_USER", 
                    "REMOTE_IDENT",
                    "AUTH_TYPE", "CONTENT_TYPE", "CONTENT_LENGTH", 
                    "HTTP_ACCEPT", "HTTP_USER_AGENT", "HTTP_REFERER"};

bool bShowAllValues = false;
bool bShowNonNullsOnly = false;
bool bShowNullsOnly = false;
bool bShowQueryStringOnly = false;
char *sQueryString = '\0';

int main(void) {

    const int numvars=20;
    int i;

//  setenv("QUERY_STRING","QueryString",1);

// test if Null or All or non-Null values should be shown ------------------------------------------------------------

    if (getenv("QUERY_STRING") == NULL) {
        printf("\n\n");
        printf("No parameter string passed");
        printf("\n\n");
    }
    else if (strcmp(getenv("QUERY_STRING"), "ShowAllValues") == 0) {
        bShowAllValues = true;
        bShowNullsOnly = false;
        bShowNonNullsOnly = false;
        bShowQueryStringOnly = false;
        printf("\n");
        printf("All environment variables shown");
        printf("\n\n");
    }
    else if (strcmp(getenv("QUERY_STRING"), "ShowNullsOnly") == 0) {
        bShowNullsOnly = true;
        bShowAllValues = false;
        bShowNonNullsOnly = false;
        bShowQueryStringOnly = false;
        printf("\n");
        printf("Null value environment variables shown");
        printf(" (non-null environment values suppressed)");
        printf("\n\n");
    }
    else if (strcmp(getenv("QUERY_STRING"), "ShowNonNullsOnly") == 0) {
        bShowNullsOnly = false;
        bShowAllValues = false;
        bShowNonNullsOnly = true;
        bShowQueryStringOnly = false;
        printf("\n");
        printf("Non-null environment values shown");
        printf(" (null value environment variables suppressed)");
        printf("\n\n");
    }

    else if (strcmp(getenv("QUERY_STRING"), "QueryString") == 0) {
        bShowNullsOnly = false;
        bShowAllValues = false;
        bShowNonNullsOnly = false;
        bShowQueryStringOnly = true;
        printf("\n");
        printf("Query string value shown");
        printf(" (all other environment variables suppressed)");
        printf("\n\n");
    }

// show the environment values and suppress nulls if requested

    if (bShowNonNullsOnly == true) {
        for (i=0; i < numvars; i++) {
            if (getenv(evars[i]) != NULL) {
                printf("%s = %s\n", evars[i], getenv(evars[i]));
            }
        }
        printf("\n\n");
    }
    else if (bShowNullsOnly == true) {
        for (i=0; i < numvars; i++) {
            if (getenv(evars[i]) == NULL) {
                printf("%s = %s\n", evars[i], getenv(evars[i]));
            }
        }
        printf("\n\n");
    }
    else if (bShowAllValues == true) {
        for (i=0; i<numvars; i++) {
            printf("%s = %s\n", evars[i], getenv(evars[i]));
        }
    }
    else if (bShowQueryStringOnly == true) {
        printf("%s = %s\n", "QUERY_STRING", getenv("QUERY_STRING"));
        printf("\n\n");
    }

    return 0;
}
