#include <stdio.h>
#include <string.h>

int main () {
   FILE *fp, *fp2;
   char c[] = "this is tutorialspoint";
   char buffer[100];

   /* Open file for both reading and writing */
   fp = fopen("/dev/vsp_dev", "rw");

   if(fp == NULL){
      printf("open drirver Error \n");
      return 1;
   }

      /* Open file for both reading and writing */
   fp2 = fopen("/dev/vsp_dev", "rw");

   if(fp2 == NULL){
      printf("open drirver Error 222 \n");
      fclose(fp);
      return 1;
   }

   fclose(fp);
   fclose(fp2);
   
   return(0);
}