#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

/* short options:
 * 
 * -o outfile name - write to file rather than stdout
 * -t output type 
 * -b batch size - number of lines to process before flushing - debug only
 * -i ip of db server 
 * -p port of db server 
 * -v verbose output
 * additional file names are used as input files
 * 
 *
 * long options:
 *
 * --outfile
 * --type
 * --batchsize
 * --ip
 * --port
 * --verbose
 *
 */

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

struct globalArgs_t {
	const char *outFileName;	        /* -o option */
	FILE *outFile;
	char *type;				/* -t option */
	int  batch_size;			/* -b option */
	char *ip;				/* -i option */
	char *port;				/* -p option */
	int verbose;				/* verbosity */
	char **inputFiles;			/* input files */
	int numInputFiles;			/* # of input files */
} globalArgs;

static const char *optString = "o:t:b:i:p:vh?";

static const struct option longOpts[] = {
	{ "outFileName", no_argument, NULL, 'o' },
	{ "type", required_argument, NULL, 't' },
	{ "batch_size", no_argument, NULL, 'b' },
	{ "ip", no_argument, NULL, 'i' },
	{ "port", no_argument, NULL, 'p' },
	{ "verbose", no_argument, NULL, 'v' },
	{ "help", no_argument, NULL, 'h' },
	{ NULL, no_argument, NULL, 0 }
};

void display_usage( void )
{
	puts( "chopper - reads apache logs and outputs to file or db" );
	exit( EXIT_FAILURE );
}

/* Convert the input files to HTML, governed by globalArgs.
 *  */
int chop( void )
{
	printf( "outFileName: %s\n", globalArgs.outFileName );
	printf( "type: %s\n", globalArgs.type );
	printf( "batch_size: %d\n", globalArgs.batch_size );
	printf( "ip: %s\n", globalArgs.ip);
	printf( "port: %s\n", globalArgs.port);
	printf( "verbose: %d\n", globalArgs.verbose );
	printf( "numInputFiles: %d\n", globalArgs.numInputFiles );
	int running_total = 0;
   //LOOP THROUGH FILES
    FILE *pRead;
    int f_count;  
    for (f_count = 0; f_count < globalArgs.numInputFiles; f_count++) {
      printf("File %d: %s\n",f_count,globalArgs.inputFiles[f_count]);
      pRead = fopen(globalArgs.inputFiles[f_count], "r");
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
  return(0);
}
int main (int argc, char *argv[]) {
	int opt = 0;
	int longIndex = 0;
	globalArgs.outFileName = NULL;
	globalArgs.outFile = NULL;
	globalArgs.type = NULL;
	globalArgs.batch_size = BATCH_SIZE;
	globalArgs.ip = NULL;
	globalArgs.port = NULL;
	globalArgs.verbose = 0;
	globalArgs.inputFiles = NULL;
	globalArgs.numInputFiles = 0;
	
	opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	while( opt != -1 ) {
		switch( opt ) {
			case 'o':
				globalArgs.outFileName = optarg;
				break;
			case 't':
				globalArgs.type = optarg;
				break;
			case 'b':
				globalArgs.batch_size = atoi(optarg);
				break;
			case 'i':
				globalArgs.ip = optarg;
				break;
			case 'p':
				globalArgs.port = optarg;
				break;
			case 'v':
				globalArgs.verbose = 1;
				break;
			case 'h':
			case '?':
				display_usage();
				break;
			case 0:	
				break;
			default:
				break;
		}
		
		opt = getopt_long( argc, argv, optString, longOpts, &longIndex );
	}
	
	globalArgs.inputFiles = argv + optind;
	globalArgs.numInputFiles = argc - optind;
	chop();
        return(0);
}
