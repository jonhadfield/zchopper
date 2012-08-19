#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  READ_LINE_LENGTH   600
#define  MAX_LINE_LENGTH   500

typedef struct {
	char req_ip[15];
	char req_ident[100];
	char req_user[100];
	char req_date[100];
	char req_time[100];
	char req_line[100];
	char resp_code[100];
	char resp_size[100];
	char req_referer[100];
	char req_user_agent[100];
} st_http_request;


int main (int argc, char *argv[]) {

  FILE *pRead;
  pRead = fopen("log.log", "r");

  st_http_request *p,*tmp;
  p = (st_http_request *)malloc(sizeof(st_http_request)*10);

  char log_line[READ_LINE_LENGTH];
  int counter = 0;
  int line_length = 0;
  while (fgets(log_line, 1024, pRead) != NULL) {

    line_length = strlen(log_line);
    //DEBUG
    printf("line no.: %d\n",counter);
    printf("logline: %s\n",log_line);
    printf("logline length: %d\n",line_length);
    printf("penult char is: %c\n",log_line[line_length-3]);
    printf("last char is: %c\n\n",log_line[line_length-2]);

    if ( line_length > MAX_LINE_LENGTH ) {
      printf("Line: %d exceeds max request length of: %d\n", counter, MAX_LINE_LENGTH);
      continue;
    }

    sscanf(log_line, "%s %s %s %s %s %s %s %s %s %s", \
                     p[counter].req_ip, p[counter].req_ident, \
                     p[counter].req_user, p[counter].req_date, \
                     p[counter].req_time, p[counter].req_line, \
                     p[counter].resp_code, p[counter].resp_size, \
                     p[counter].req_referer,p[counter].req_user_agent);
    printf("log_line %d: ip:%s ident:%s user:%s date:%s\n",counter,p[counter].req_ip,p[counter].req_ident,p[counter].req_user,p[counter].req_date);
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
