//  authenticateUser.c - CGI to authenticate a user from an entered string
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
//  Enhancements (0):
// 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#define HTML_LEN 60                                    // Max length of input string from html form
#define CGI_LEN HTML_LEN + 6                               // 5 for "data=" and 1 for trailing NULL
#define DATAFILE "authenticateUser.dat"

int main(void) {
    char   *sParams;                                                  // pointer to the QUERY_STRING
    char   *sSubstring;                                                  // pointer to the substring
    char   caDelimiter[] = "&";                                                   // token delimiter
    char   caUsername[60] = "\0";                                                  // username array
    char   caPassword[60] = "\0";                                                  // password array
    char   *sValidUser = "gjarman";                                            // the valid username
    char   *sValidPassword = "Mpa4egu9";                                       // the valid password
    bool   bUserIsValid = false;                                    // flag to indicate a valid user
    bool   bPasswordIsValid = false;                            // flag to indicate a valid password

//    setenv("QUERY_STRING", "Username=gjarman&Password=Mpa4egu9", 1);

// print the html page content type ------------------------------------------------------------------------------------

    printf("Content-type: text/html");
    printf("\n\n");

//  get the content from QUERY_STRING and tokenize based on '&' character-----------------------------------------------

    sParams = getenv("QUERY_STRING");

    sSubstring = strtok(sParams, caDelimiter);
    sscanf(sSubstring, "Username=%s", caUsername);
    if (strcmp(caUsername, sValidUser) == 0) {
        bUserIsValid = true;
    }

    sSubstring = strtok(NULL, caDelimiter);
    sscanf(sSubstring, "Password=%s", caPassword);
    if (strcmp(caPassword, sValidPassword) == 0) {
        bPasswordIsValid = true;
    }

//  build response and output back to requesting javascript ------------------------------------------------------------

    if (bUserIsValid == false) {
        printf("Bad username");
    }
    else if (bPasswordIsValid == false) {
        printf("Bad password");
    }
    else {
        printf("Success");
    }

    return 0;
}
