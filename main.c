#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <malloc.h>

#include "emulate8080.h"


void *get_buffer(char *filename)
{
   int fd, res;
   struct stat statbuf; 
   unsigned char *buf;


   fd = open(filename, O_RDONLY);
   if (fd < 0) {
      fprintf(stderr, "failed to open file\n");
      return NULL;
   }


   if (fstat(fd, &statbuf) < 0 ){
      fprintf(stderr, "stat failed\n");
      close(fd);
      return NULL;
   }

   buf = malloc(statbuf.st_size);

   if (buf == NULL) {
      fprintf(stderr, "malloc failed\n");
      close(fd);
      return NULL;
   }

   res = read(fd, buf, statbuf.st_size);
   if (res < 0){
      fprintf(stderr, "read failed");
      free(buf);
      close(fd);
      return NULL;
   } else {
      printf("read: %d bytes\n", statbuf.st_size);
   }

   close(fd);
   return buf;
}

void hexdump(unsigned char *buf) 
{
   int i;
   int row = 8;
   int first_set = 0;

   size_t buf_size = malloc_usable_size(buf);

   for (i = 0; i < buf_size; i++) {
      printf("%02x ", buf[i]);
      if ((i+1) % row == 0) {
         if (first_set){
            first_set = 0;
            printf("\n");
         } else {
            first_set = 1;
            printf("  ");
         }
      }
   }
   printf("\n");
}




int main(int argc, char **argv)
{

   char *filename = argv[1];
   unsigned char *buf;

   buf = get_buffer(filename);

   if (buf == NULL) {
      return -1;
   }

   State8080 *state = initilizeState(); 

   loadIntoMemory(state, buf);

   disass8080(state->memory);


   cleanUpState(state);


   free(buf);

   
}
