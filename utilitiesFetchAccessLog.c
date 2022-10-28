//  utilitiesFetchAccessLog.c - CGI to concatenate and fetch the Apache2 access logs
//  Author: Geoffey Jarman
//  Started: 29-Jun-2022
//  Refererences:
//  Log:
//      29-Jul-2022 started by copying showText.c
//      30-Jun-2022 disable system command which is blocked due to insufficient privileges
//      01-Jul-2022 remove the system command to build the file (moved to a crontab on the OS)
//      15-Sep-2022 add Access-Control-Allow-Origin: * CORS http header
//      25-Oct-2022 clean up comments
//      25-Oct-2022 add a guard for a missing file around fopen
//  Enhancements(0):

// includes and defines ------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#define DATAFILE "apacheAccessLog.txt"
#define MAXLEN 200

int main() {

    FILE *f;
    if((f = fopen (DATAFILE, "r+")) == NULL) {
        return EXIT_FAILURE;
    }
    char cyText[MAXLEN];

// print the html content-type header and CORS header block ------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// read text from the file and print it --------------------------------------------------------------------------------

    while (fgets(cyText, MAXLEN, f) != NULL) {
        printf("%s", cyText);
   }

    fclose(f);

    return 0;
}
