#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define  MAX_LINE_LENGTH  8192 
#define  MAX_IP             15
#define  MAX_IDENT           8 
#define  MAX_USER            8 
#define  MAX_DATETIME       22  
#define  MAX_METHOD          7 
#define  MAX_URI          2083 //Safe limit?
#define  MAX_PROTO           8
#define  MAX_RESP_CODE       3
#define  MAX_RESP_BYTES     20 
#define  MAX_REFERER      2083 //Safe limit?
#define  MAX_AGENT        1024 //Safe limit?

typedef struct {
	char req_ip[MAX_IP];
	char req_ident[MAX_IDENT];
	char req_user[MAX_USER];
	char req_datetime[MAX_DATETIME];
	char req_method[MAX_METHOD];
	char req_uri[MAX_LINE_LENGTH];
	char req_proto[MAX_PROTO];
	char resp_code[MAX_RESP_CODE];
	char resp_bytes[MAX_RESP_BYTES];
	char req_referer[MAX_REFERER];
	char req_agent[MAX_AGENT];
} st_http_request;


int main (int argc, char *argv[]) {

  FILE *pRead;
  pRead = fopen("log.log", "r");

  st_http_request *p,*tmp;
  p = (st_http_request *)malloc(sizeof(st_http_request)*2);

  char log_line[MAX_LINE_LENGTH];
  int counter = 0;
  int line_length = 0;
  while (fgets(log_line, 8192, pRead) != NULL) {
  //printf("\n\n\nstart of loop\n");

    /*printf("line no.: %d\n",counter);
    printf("logline: %s\n",log_line);
    printf("logline length: %d\n",line_length);
    printf("penult char is: %c\n",log_line[line_length-3]);
    printf("got here 4\n");*/

    line_length = strlen(log_line);
    if (line_length > MAX_LINE_LENGTH-1) {
      printf("Encountered a stupidly long line of over 8KB\nGoodbye!!!");
      exit(1);
    }else{

      sscanf(log_line, "%s %s %s [%[^]]] \"%s %s %[^\"]\" %s %s \"%[^\"]\" \"%[^\"]\"", \
                     p[counter].req_ip, p[counter].req_ident, \
                     p[counter].req_user, p[counter].req_datetime, \
                     p[counter].req_method, p[counter].req_uri, \
                     p[counter].req_proto, \
                     p[counter].resp_code, p[counter].resp_bytes, \
                     p[counter].req_referer, p[counter].req_agent);

      /*printf("scanned line\n");
      printf("ip:%s, ip len:%d\n",p[counter].req_ip,(int)strlen(p[counter].req_ip));
      printf("ident:%s\n",p[counter].req_ident);
      printf("user:%s\n",p[counter].req_user);
      printf("datetime:%s\n",p[counter].req_datetime);
      printf("reqmethod:%s\n",p[counter].req_method);
      printf("requri:%s\n",p[counter].req_uri);
      printf("reqproto:%s\n",p[counter].req_proto);
      printf("reqcode:%s\n",p[counter].resp_code);
      printf("respbytes:%s\n",p[counter].resp_bytes);
      printf("reqreferer:%s\n",p[counter].req_referer);
      printf("reqagent:%s\n",p[counter].req_agent);*/

      tmp = (st_http_request *) realloc ( p, (counter+2) * sizeof(st_http_request) );
      if (tmp == NULL) { 
        printf("pants!!!");
      } else {
        p = tmp; 
      }
    }
    counter++;
  }
  printf("Scanned: %d lines.\n",counter);
  free(p);
  return(0);
}
