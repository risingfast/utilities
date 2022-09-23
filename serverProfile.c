//  serverProfile.c -- prints javascript variables back to the calling html page serverProfile.html
//  Author -- Geoffrey Jarman
//  Started -- 01-Aug-2020
//  Log:
//      01-Aug-2020 started
//      13-Aug-2020 add system information from the server into returned data
//      13-Feb-2021 reviewed all
//      13-Feb-2021 webcode
//      22-Sep-2021 Copied from fetch.c
//      14-Sep-2022 Changed return of main to int
//      16-Sep-2022 Add Access-Control-Allow-Origin: * CORS http header
//  References:
//      https://c-program-example.com/2012/01/c-program-to-get-the-system-information.html
//      https://www.willmaster.com/library/tutorials/javascript-and-cgi-talking-to-each-other.php
//

// includes and defines

#include <stdio.h>
#include <sys/utsname.h>

int main() 
{

// variable declarations

    struct utsname uname_pointer;
    uname(&uname_pointer);

// print content-type text/javascript and CORS http headers ----------

    printf("Content-type: text/javascript\n");
    printf("Access-Control-Allow-Origin: *\n\n");

    printf("ServerSysName = \"%s\";\n", uname_pointer.sysname);
    printf("ServerNodeName = \"%s\";\n", uname_pointer.nodename);
    printf("ServerRelease = \"%s\";\n", uname_pointer.release);
    printf("ServerVersion = \"%s\";\n", uname_pointer.version);
    printf("ServerMachine = \"%s\";\n", uname_pointer.machine);
    return 0;
}
