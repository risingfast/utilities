/* file_stat.c -- get file statistics for a file path: file permission, size, creation and last modification date
 * Author: Geoffrey Jarman
 * Started: 14-Oct-2022
 * References: 
 *     https://codeforwin.org/2018/03/c-program-find-file-properties-using-stat-function.html
 * Log:
 *     14-Oct-2022 started
 *     15-Oct-2022 add addtional comparisons to match webpage list of cheatsheets
 *     15-Oct-2022 add a heading to the report
 *     15-Oct-2022 add a count of newer working files to the report
 *     19-May-2023 add S62 and S82 for Docker cheatSheet
 *     27-Jun-2023 add S63 and S83 for tmux cheatSheet
 *     20-Aug-2023 add S64 and S84 for Apple cheatSheet
 *     10-Sep-2023 add S65 and S85 for Chrome cheatSheet
 *     26-Jan-2024 add S66 and S86 for Blender cheatSheet
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>

void compareFiles(char *, char *);

struct stat stats1, stats2;
int iOldFileCnt = 0;

int main()
{

// create variables for source files -----------------------------------------------------------------------------------

    char *s01 = "/mnt/c/Users/gjarm/OneDrive/Computing/Arduino/Notes/cheatSheet.Arduino.txt";
    char *s02 = "/mnt/c/Users/gjarm/OneDrive/Computing/GCP/Notes/cheatSheet.GCP.txt";
    char *s03 = "/mnt/c/Users/gjarm/OneDrive/Computing/Git and GitHub/Notes/cheatSheet.Git.txt";
    char *s04 = "/mnt/c/Users/gjarm/OneDrive/Computing/JSON/Notes/cheatSheet.JSON.txt";
    char *s05 = "/mnt/c/Users/gjarm/OneDrive/Computing/Languages/Assembly/Notes/cheatSheet.Assembly.txt";
    char *s06 = "/mnt/c/Users/gjarm/OneDrive/Computing/Languages/cLang/Notes/cheatSheet.Clang.txt";
    char *s07 = "/mnt/c/Users/gjarm/OneDrive/Computing/Languages/cpp/Notes/cheatSheet.Cpp.txt";
    char *s08 = "/mnt/c/Users/gjarm/OneDrive/Computing/Languages/Perl/Notes/cheatSheet.Perl.txt";
    char *s09 = "/mnt/c/Users/gjarm/OneDrive/Computing/Languages/Python/Notes/cheatSheet.Python.txt";
    char *s10 = "/mnt/c/Users/gjarm/OneDrive/Computing/Languages/RegEx/Notes/cheatSheet.Regex.txt";
    char *s11 = "/mnt/c/Users/gjarm/OneDrive/Computing/Languages/Web/Notes/cheatSheet.Web.txt";
    char *s12 = "/mnt/c/Users/gjarm/OneDrive/Computing/Linux/Notes/cheatSheet.Linux.txt";
    char *s13 = "/mnt/c/Users/gjarm/OneDrive/Computing/Markdown/Notes/cheatSheet.Markdown.txt";
    char *s14 = "/mnt/c/Users/gjarm/OneDrive/Computing/Microsoft/Notes/cheatSheet.Microsoft.txt";
    char *s15 = "/mnt/c/Users/gjarm/OneDrive/Computing/Networks/Notes/cheatSheet.Networks.txt";
    char *s16 = "/mnt/c/Users/gjarm/OneDrive/Computing/Oracle/Notes/cheatSheet.Oracle.txt";
    char *s17 = "/mnt/c/Users/gjarm/OneDrive/Computing/Yaml/Notes/cheatSheet.Yaml.txt";
    char *s18 = "/mnt/c/Users/gjarm/OneDrive/Computing/Oracle/mySQL/Notes/cheatSheet.MySQL.txt";
    char *s19 = "/mnt/c/Users/gjarm/OneDrive/Computing/Linux/Vi/Notes/cheatSheet.Vi.txt";
    char *s20 = "/mnt/c/Users/gjarm/OneDrive/Computing/Apache2/Notes/cheatSheet.Apache2.txt";
    char *s61 = "/mnt/c/Users/gjarm/OneDrive/Hobbies/Electronics/Notes/cheatSheet.Electronics.txt";
    char *s62 = "/mnt/c/Users/gjarm/OneDrive/Computing/Docker/Notes/cheatSheet.Docker.txt";
    char *s63 = "/mnt/c/Users/gjarm/OneDrive/Computing/Linux/tmux/Notes/cheatSheet.tmux.txt";
    char *s64 = "/mnt/c/Users/gjarm/OneDrive/Computing/Apple/Notes/cheatSheet.Apple.txt";
    char *s65 = "/mnt/c/Users/gjarm/OneDrive/Computing/Google/Chrome/Notes/cheatSheet.Chrome.txt";
    char *s66 = "/mnt/c/Users/gjarm/OneDrive/Computing/Blender/Notes/cheatsheet.Blender.txt";

// create variables for corresponding project files --------------------------------------------------------------------

    char *s21 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Arduino.txt";
    char *s22 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.GCP.txt";
    char *s23 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Git.txt";
    char *s24 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.JSON.txt";
    char *s25 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Assembly.txt";
    char *s26 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Clang.txt";
    char *s27 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Cpp.txt";
    char *s28 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Perl.txt";
    char *s29 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Python.txt";
    char *s30 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Regex.txt";
    char *s31 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Web.txt";
    char *s32 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Linux.txt";
    char *s33 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Markdown.txt";
    char *s34 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Microsoft.txt";
    char *s35 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Networks.txt";
    char *s36 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Oracle.txt";
    char *s37 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Yaml.txt";
    char *s38 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.MySQL.txt";
    char *s39 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Vi.txt";
    char *s40 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Apache2.txt";
    char *s81 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Electronics.txt";
    char *s82 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Docker.txt";
    char *s83 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.tmux.txt";
    char *s84 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Apple.txt";
    char *s85 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Chrome.txt";
    char *s86 = "/mnt/c/Users/gjarm/OneDrive/Projects/Open/cheatsheets/cheatSheet.Blender.txt";

    system("clear");
    printf("file_stats.c - file statistics on cheatsheets to identify old versions in web project folder");
    printf("\n\n");


    compareFiles(s01, s21);
    compareFiles(s02, s22);
    compareFiles(s03, s23);
    compareFiles(s04, s24);
    compareFiles(s05, s25);
    compareFiles(s06, s26);
    compareFiles(s07, s27);
    compareFiles(s08, s28);
    compareFiles(s09, s29);
    compareFiles(s10, s30);
    compareFiles(s11, s31);
    compareFiles(s12, s32);
    compareFiles(s13, s33);
    compareFiles(s14, s34);
    compareFiles(s15, s35);
    compareFiles(s16, s36);
    compareFiles(s17, s37);
    compareFiles(s18, s38);
    compareFiles(s19, s39);
    compareFiles(s20, s40);
    compareFiles(s61, s81);
    compareFiles(s62, s82);
    compareFiles(s63, s83);
    compareFiles(s64, s84);
    compareFiles(s65, s85);
    compareFiles(s66, s86);

    printf("\n");
    printf("Count of NEWER working files: %d", iOldFileCnt);
    printf("\n\n");

    return 0;
}

void compareFiles(char *file1, char *file2) {
    if (stat(file1, &stats1) != 0) {
        printf("File %s is missing\n", file1);
    }
    if (stat(file2, &stats2) != 0) {
        printf("File %s is missing\n", file2);
    }
    if ((stats1.st_mtime - stats2.st_mtime) > 0) {
        printf("Working version %s is NEWER %12s\n", file1, "<-**");
        iOldFileCnt++;
    } else {
        printf("Project version %s is current\n", file2);
    }

    return;

}
