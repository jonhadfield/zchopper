#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct http_request {
	char ip[15];
	char ident[1];
};


int main (int argc, char *argv[]) {

  FILE *pRead;
  char log_line[5000];
  pRead = fopen("log.log", "r");

  int count1=0;
  struct http_request *example=malloc(sizeof(*example));  

  while (fgets(log_line, 300, pRead) != NULL) {
    sscanf(log_line, "%s %s", example[count1].ip, example[count1].ident);
    printf("log_line %d: ip:%s ident:%s\n",count1,example[count1].ip,example[count1].ident);
    if (count1>0) example=realloc(example,(count1+1)*sizeof(*example));
    count1++;
  }

  close(pRead);
  free(example);
  return 0;  
}








