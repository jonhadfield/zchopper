#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char ip[150];
	char ident[100];
} st_http_request;

int main (int argc, char *argv[]) {

  FILE *pRead;
  pRead = fopen("log.log", "r");

  st_http_request *p,*tmp;
  p = (st_http_request *)malloc(sizeof(st_http_request)*10);
  printf("initial size of temp_struct: %d\n",sizeof(st_http_request));

  char log_line[600];
  int counter = 0;
  while (fgets(log_line, 600, pRead) != NULL) {
    //Sanity check log line
    sscanf(log_line, "%s %s", p[counter].ip, p[counter].ident);
    printf("log_line %d: ip:%s ident:%s\n",counter,p[counter].ip,p[counter].ident);
    counter++;
    tmp = (st_http_request *) realloc ( p, (counter+4) * sizeof(st_http_request) );
    if (tmp == NULL) { 
      printf("pants!!!");
    } else {
      p = tmp; 
    }
  }

  free(p);
  return(0);
}
