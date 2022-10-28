//  setCornerImage.c - CGI to select a cornerImage and caption text from a text file
//  Author: Geoffey Jarman
//  Started: 30-May-2022
//  Refererences:
//  Log:
//      30-May-2022 started by copying showText.c
//      31-May-2022 change datafile from cornerImage.txt to cornerImageList.txt
//      16-Sep-2022 add Access-Control-Allow-Origin: * http CORS header
//      25-Oct-2022 clean up comments
//      25-Oct-2022 add a guard to fopen to check for NULL
//  Enhancements(0):

// includes and defines ------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#define DATAFILE "cornerImageList.txt"
#define MAXLEN 200

int main() {

    FILE *f;
    if ((f = fopen (DATAFILE, "r")) == NULL) {
        return EXIT_FAILURE;
    }
    char cyText[MAXLEN];

// print the html content-type header and CORS head block --------------------------------------------------------------

    printf("Content-type: text/html\n");
    printf("Access-Control-Allow-Origin: *\n\n");

// read text from the file and print it --------------------------------------------------------------------------------

    while (fgets(cyText, MAXLEN, f) != NULL) {
        printf("%s", cyText);
    };

    fclose(f);

    return 0;
}
