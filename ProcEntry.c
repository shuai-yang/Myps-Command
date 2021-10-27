/* 
 * Author: Shuai Yang
 * Date: April 23 2021
 * Description: Stuct and functions for working with ProcEntr 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> //#define _SC_CLK_TCK
#include "ProcEntry.h"

ProcEntry* CreateProcEntry(void){
    /* Allocate space for ProcEntry in heap and validate */
    ProcEntry* newProcEntry = (ProcEntry*) malloc(sizeof(ProcEntry));
    if (newProcEntry == NULL) {
        return NULL;
    }
    
    /* Initialize the newProcEntry to zero or NULL based upon data type */
    newProcEntry->pid = 0;
    newProcEntry->ppid = 0;
    newProcEntry->state = 0;
    newProcEntry->utime = 0;
    newProcEntry->stime = 0;
    newProcEntry->num_threads = 0;
    newProcEntry->comm = NULL;
    newProcEntry->path = NULL;
    return newProcEntry;
}

void DestroyProcEntry(ProcEntry* thisProcEntry) {
    /* Validate Parameters */
    if (thisProcEntry == NULL) {
        return;
    }
    /* Free the individual fields that were allocated
     *   with malloc in CreateProcEntryFromFile */
    free(thisProcEntry->comm);
    free(thisProcEntry->path);
    free(thisProcEntry);
}

ProcEntry* CreateProcEntryFromFile(const char statFile[]){
    /* Validate Parameters */
    if(statFile == NULL) {
        return NULL;
    }
   
    /* Open and validate file */
    FILE* dataFile = fopen(statFile,"r");
    if(dataFile == NULL){
        perror("fopen");
        return NULL;
    }
    
    /* Allocate space for ProcEntry in heap, initialize and validate */
    ProcEntry* newProcEntry = CreateProcEntry();
    if (newProcEntry == NULL){
        return NULL;
    }

    /* Allocate space for path in heap and validate */
    newProcEntry->path = (char*)malloc(sizeof(char) * (strlen(statFile) + 1));
    if (newProcEntry->path == NULL){
        return NULL;
    }
    strcpy(newProcEntry->path, statFile);
    
    /* Extract the required fields to build a ProcEntry struct */
    const int MAX_FIELD_SIZE = 20;
    char commField[MAX_FIELD_SIZE];

    int dummy;
    long ldummy;
    int fieldsRead = 0;
    //https://man7.org/linux/man-pages/man5/proc.5.html
    fieldsRead = fscanf(dataFile,
        "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld",
		&(newProcEntry->pid),commField,&(newProcEntry->state),
        &(newProcEntry->ppid),&dummy,&dummy,&dummy,&dummy,&dummy,&ldummy,
		&ldummy,&ldummy,&ldummy,&(newProcEntry->utime),&(newProcEntry->stime),
        &ldummy,&ldummy,&ldummy,&ldummy,&(newProcEntry->num_threads));
    if(fieldsRead != 20){
        perror("fscanf");
        return NULL;
    }

    /* Allocate space for comm (the filename of the executable) in heap and validate */
    newProcEntry->comm = (char*) malloc(sizeof(char) * (strlen(commField) + 1));
    if (newProcEntry->comm == NULL) {
        return NULL;
    } 
    strcpy(newProcEntry->comm,commField);
    
    fclose(dataFile);
    return newProcEntry;
}

void PrintProcEntry(ProcEntry* entry){                                                                                                                                                      
     unsigned long int utime = entry->utime / sysconf(_SC_CLK_TCK);
     unsigned long int stime = entry->stime / sysconf(_SC_CLK_TCK);
     fprintf(stdout, "%7d %7d %5c %5lu %5lu %7ld %-25s %-20s\n",
             entry->pid,
             entry->ppid,
             entry->state,
             utime,
             stime,
             entry->num_threads,
             entry->comm,
             entry->path);
}
