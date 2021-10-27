/* 
 * File: mypc.c
 * Author: Sharon Yang
 * Date: April 24 2021 
 * Description: Write a simplified version of the ps command
 * found on Linux/Unix based systems
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> //optarg
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include "ProcEntry.h"

#define MAX_PATH_LENGTH 4096
#define UNUSED(x) (void)x

void usage(char * argv[]);

int noSort(const struct dirent** entryA, const struct dirent** entryB){
    UNUSED(entryA);
    UNUSED(entryB);
    return 0;
}
/*build a filtered array of dirent structs that contains only PID directories*/
static int PID_Directory (const struct dirent* current) {
    /* check if the dirent type is a directory 
    * and if the first character of the name is a number
    */
    if(current->d_type == DT_DIR && (current->d_name[0] >='0' && current->d_name[0] <='9')){  
        return 1;
    }else{
        return 0;
    }
}

/* sort ProcEntry items by process id */
static int pidSort(const void *a, const void *b){
    ProcEntry *f = *(ProcEntry **)a;
    ProcEntry *s = *(ProcEntry **)b;
    int rval = f->pid - s->pid;
    return rval;
}

/* sorting by command (comm) */
static int commSort(const void *a, const void *b){
    ProcEntry *f = *(ProcEntry **)a;
    ProcEntry *s = *(ProcEntry **)b;
    int rval = strcmp(f->comm, s->comm);
    return rval;
}

int main (int argc, char* argv[]) {// pass in command-line arguments
    struct dirent **eps;
    int n; 
    int opt;
    bool commSortFlag = false;
    bool zStateFlag = false;

    /*declare dirPath and set default to /proc*/
    char dirPath[MAX_PATH_LENGTH];
    strcpy(dirPath, "/proc"); 

    /*declare filterFunction pointer*/
    int(*filterFunction)(const struct dirent*);
    filterFunction = PID_Directory;

    /*declare sortFunction pointer*/
    int(*sortFunction)(const struct dirent**, const struct dirent**);
    sortFunction = noSort;
    
    /*use getopt() to process command line arguments*/
    while((opt = getopt(argc, argv, "d:pczh")) != -1){
        switch(opt){
            case 'd':
                strncpy(dirPath, optarg, MAX_PATH_LENGTH);
                break;
            case 'p':
                break;
            case 'c':
                commSortFlag = true;
                break;
            case 'z':
                zStateFlag = true;
                break;
            case 'h': //help
                usage(argv); 
                break;
            default:
                fprintf(stderr, "Error: Invalid option specified\n");
                fprintf(stderr, "Usage: %s [-d <path>] [-p] [-c] [-z] [-h]\n", argv[0]);    
                break;
        }
    }                                               

    /*perform the actual scandir of the dirPath*/
    errno = 0;
    n = scandir (dirPath, &eps, filterFunction, sortFunction);  
    /* validate directory was opened successfully */
    if (n < 0) {
        perror("scandir: ");
        exit(1);
    }
    
    ProcEntry** myprocs = (ProcEntry**) (malloc(sizeof(ProcEntry*) * n));

    int cnt;
    for (cnt = 0; cnt < n; ++cnt) {
        /*use string concatenation to build the full file path 
        * to the stat file located in each PID directory
        */
        char* pathToStat = malloc(strlen(dirPath) + strlen(eps[cnt]->d_name) + 7); 
        //+ 7 because of the '/[pid]/stat' and the terminating 0
        strcpy(pathToStat,dirPath);
        strcat(pathToStat, "/" ); // or "\\" in Windows
        strcat(pathToStat, eps[cnt]->d_name);  // fprintf(stdout,"%s\n", eps[cnt]->d_name);
        strcat(pathToStat, "/stat" );
        ProcEntry* procEntry = CreateProcEntryFromFile(pathToStat);
        myprocs[cnt] = procEntry;
        free(pathToStat);
    }

    /* sort the myprocs */
    if(commSortFlag){
        qsort(myprocs, n, sizeof(ProcEntry*), commSort);
    }else{
        qsort(myprocs, n, sizeof(ProcEntry*), pidSort);
    }
    
    /* write the lines to output stream */
    fprintf(stdout,"%7s %7s %5s %5s %5s %7s %-25s %-20s\n",
        "PID","PPID","STATE","UTIME","STIME","THREADS","CMD","STAT_FILE");
    for (cnt = 0; cnt < n; ++cnt){
        /* filter the myprocs */
        if(zStateFlag){
            if (myprocs[cnt]->state == 'Z'){
                PrintProcEntry(myprocs[cnt]);
            }
        }else{
            PrintProcEntry(myprocs[cnt]);
        }
    }

    /*clean  up memory*/
    for (cnt = 0; cnt < n; ++cnt) {
        free (eps[cnt]);
        DestroyProcEntry(myprocs[cnt]);
    }
    free(eps);
    free(myprocs);

    return 0;
}

/*display help dialog and exit*/
void usage(char * argv[]){
    fprintf(stdout, "Usage: %s [-d <path>] [-p] [-c] [-z] [-h]\n"
                    "       -d <path> Directory containing proc entries (default: /proc)\n"
                    "       -p        Display proc entries sorted by pid (default)\n"
                    "       -c        Display proc entries sorted by command lexicographically\n"
                    "       -z        Display ONLY proc entries in the zombie state\n"
                    "       -h        Display this help message\n", argv[0]);
    exit(0);
}
