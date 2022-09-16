//  setCornerImage.c - CGI to select a cornerImage and caption text from a text file
//  Author: Geoffey Jarman
//  Started: 30-May-2022
//  Refererences:
//  Log:
//      30-May-2022 started by copying showText.c
//      31-May-2022 change datafile from cornerImage.txt to cornerImageList.txt
//  Enhancements(0):

// includes and defines

#include <stdio.h>
#include <stdlib.h>
#define DATAFILE "cornerImageList.txt"
#define MAXLEN 200

int main() {

    FILE *f;
    f = fopen (DATAFILE, "r");
    char cyText[MAXLEN];

// print the html content type and <head> block

    printf("Content-type: text/html\n\n");

// read text from the file and print it

    while (fgets(cyText, MAXLEN, f) != NULL) {
        printf("%s", cyText);
    };

    fclose(f);

    return 0;
}
