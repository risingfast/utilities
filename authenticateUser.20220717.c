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
//      10-Jun-2021 change the test username and test password
//      10-Jun-2021 remove the datafile definition as unused
//      17-Jul-2022 add a UUID cookie to the header and generate the UUID using a library package
//      17-Jul-2022 add a check for an existing cookie before authenticating
//  Enhancements (0):
// 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <uuid/uuid.h>
#include <string.h>
#define HTML_LEN 60                                     // Max length of input string from html form
#define CGI_LEN HTML_LEN + 6                                // 5 for "data=" and 1 for trailing NULL
#define UUIDTEXTSIZE sizeof(uuid_t)*2 + 5                         // size of formatted characer UUID

int main(void) {
    char   *sParams;                                                  // pointer to the QUERY_STRING
    char   *sSubstring;                                                  // pointer to the substring
    char   caDelimiter[] = "&";                                                   // token delimiter
    char   caUsername[60] = "\0";                                                  // username array
    char   caPassword[60] = "\0";                                                  // password array
    char   *sValidUser = "testUser";                                           // the valid username
    char   *sValidPassword = "testPwd$";                                       // the valid password
    char   caCookieVal[UUIDTEXTSIZE];                                                // UUID cookie
    uuid_t myUuid;                                               // raw or binary UUID type variable
    bool   bUserIsValid = false;                                    // flag to indicate a valid user
    bool   bPasswordIsValid = false;                            // flag to indicate a valid password
    bool   bCookieExists = false;                        // flag to indicate a session cookie exists

//    setenv("QUERY_STRING", "Username=testUser&Password=testPwd1$", 1);

/// check for an existing oookie named 'gj2020InstanceID' --------------------------------------------------------------

    if (strstr(getenv("HTTP_COOKIE"), "gj2020InstanceID") == NULL) {
        bCookieExists = false;
    } else {
        bCookieExists = true;
    }

//  create a new UUID if the cookie gj2020InstanceID  does not exist ---------------------------------------------------

    if (bCookieExists == false) {
        uuid_generate_random(myUuid);
        uuid_unparse(myUuid, caCookieVal);
    }

//  get the content from QUERY_STRING and tokenize the username and password based on '&' character---------------------

    if (bCookieExists == false) {
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
    }

// print the html headers including the cookie if the user sign-on is valid --------------------------------------------

    if ((bUserIsValid == true) && (bPasswordIsValid == true)) {
        printf("Set-Cookie: gj2020InstanceID=%s; HttpOnly\n", caCookieVal);
    }
    printf("Content-type: text/html\n\n");

//  build response and output back to requesting javascript ------------------------------------------------------------

    if (bCookieExists == true) {
        printf("Already Authenticated");
    } 
    else if (bUserIsValid == false) {
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
