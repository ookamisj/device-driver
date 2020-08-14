#include <stdio.h>
#include <string.h>

int main () {
   FILE *fp;
   char c[] = "this is tutorialspoint";
   char buffer[100];

   /* Open file for both reading and writing */
   fp = fopen("/dev/lkm_example", "rw");

   /* Read and display data */
   fread(buffer, strlen(c)+1, 1, fp);
   printf("%s\n", buffer);

   /* Write data to the file */
   fwrite(c, strlen(c) + 1, 1, fp);

   /* Seek to the beginning of the file */
//    fseek(fp, 0, SEEK_SET);

   /* Read and display data */
   fread(buffer, strlen(c)+1, 1, fp);
   printf("\n%s\n", buffer);
   fclose(fp);
   
   return(0);
}