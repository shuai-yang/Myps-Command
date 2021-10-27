/* 
 * File: mytests.c
 * Author: Sharon Yang
 * Date: Apirl 24 2021 
 * Description: Unit tests for the functions declared in ProcEntry.h
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ProcEntry.h"

/* 
 * test functions CreateProcEntry() and DestroyStudent() by passing in 
 * a ordinary string
 * return - 0 on success, 1 on error 
 */

int testCreateDestroy(void) {
   char testName[] = "Create/Destroy Test";
   ProcEntry* testProcEntry = CreateProcEntry();
   if (testProcEntry == NULL) {
      fprintf(stderr,"%s failed\n", testName);
      return 1;
   } 
   DestroyProcEntry(testProcEntry);
   fprintf(stderr, "%s passed\n", testName);
   return 0;
}

int testCreateFromFileDestroy(void) {
   char testName[] = "CreateFromFile/Destroy Test";
   ProcEntry* testProcEntry = CreateProcEntryFromFile("/proc/1/stat");
   if (testProcEntry == NULL) {
      fprintf(stderr,"%s failed\n", testName);
      return 1;
   } 
   DestroyProcEntry(testProcEntry);
   fprintf(stderr, "%s passed\n", testName);
   return 0;
}

/* 
 * test function PrintProcEntry() to make sure it prints out the 
 * 8 data points on one line
 * return - 0 on success, 1 on error 
 */
int testPrintProcEntry(void) {

   char testName[] = "CreateFromFile/Print/Destroy Test";

   ProcEntry* testProcEntry = CreateProcEntryFromFile("test_data/onyx_proc/1/stat");
   if (testProcEntry == NULL) {
      fprintf(stderr,"%s failed\n", testName);
      return 1;
   } 
   printf("%s - Test Output: ", testName);
   PrintProcEntry(testProcEntry);
   printf("\n%s - Expected Output:   1       0     S     1     3       1 (systemd)                 test_data/onyx_proc/1/stat\n", testName);

   DestroyProcEntry(testProcEntry);
   return 0;
}

/* 
 * test functions CreateProcEntryFromFile() by passing in a NULL pointer
 * return - 0 on success, 1 on error 
 */
int testCreateFromFileNull(void) {
   char testName[] = "CreateFromFile NULL Test";
   ProcEntry* testProcEntry = CreateProcEntryFromFile(NULL);
   if (testProcEntry == NULL) {
      fprintf(stderr,"%s passed\n", testName);
      return 0;
   } else {
      fprintf(stderr,"%s failed\n", testName);
      return 1;
   }

}
 
int main(void) {
   int status = 0;
   status += testCreateDestroy();
   status += testCreateFromFileDestroy();
   status += testPrintProcEntry();
   status += testCreateFromFileNull();
   
   return status;
}