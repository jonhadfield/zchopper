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
#define  BATCH_SIZE       1000

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
int running_total = 0;
  if(argc < 2) {
    printf( "usage: %s filename\n", argv[0] );
    exit(1);
  } else { 
   //LOOP THROUGH FILES
    FILE *pRead;
    int f_count;  
    for (f_count = 1; f_count < argc; f_count++) {
      printf("File %d: %s\n",f_count,argv[f_count]);
      pRead = fopen(argv[f_count], "r");

      st_http_request *p,*tmp;
      p = (st_http_request *)malloc(sizeof(st_http_request)*2);

      char log_line[MAX_LINE_LENGTH];
      int counter = 0;
      int line_length = 0;

      while (fgets(log_line, 8192, pRead) != NULL) {
	line_length = strlen(log_line);
	if (line_length > MAX_LINE_LENGTH-1) {
	  printf("Encountered a stupidly long line of over 8KB\nLog file must be poop. Exiting.");
	  exit(1);
	} else {
	  sscanf(log_line, "%s %s %s [%[^]]] \"%s %s %[^\"]\" %s %s \"%[^\"]\" \"%[^\"]\"", \
	    p[counter].req_ip, p[counter].req_ident, \
	    p[counter].req_user, p[counter].req_datetime, \
	    p[counter].req_method, p[counter].req_uri, \
	    p[counter].req_proto, \
	    p[counter].resp_code, p[counter].resp_bytes, \
	    p[counter].req_referer, p[counter].req_agent);

	  tmp = (st_http_request *) realloc ( p, (counter+2) * sizeof(st_http_request) );
	  if (tmp == NULL) { 
	    printf("Failed to increase memory allocation.");
	    exit(1);
	  } else {
	    p = tmp; 
	  }
	  running_total++;
	}
	if (counter == (BATCH_SIZE-1)) {
	  //printf("flushing %d\n",BATCH_SIZE);
	  counter = 0;
	}else{
	  counter++;
	}
      }
    //END LOOP THROUGH FILES
    //printf("flushing remaining %d\n",counter);
    printf("Scanned: %d lines.\n",running_total);
    fclose(pRead);
    free(p);
    }
  }
    return(0);
  }
