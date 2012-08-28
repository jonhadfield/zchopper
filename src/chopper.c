#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "mongo.h"
#include "chopper.h"

const char chopper_usage_string[] =
    "chopper [-s|--search_string] [-o|--outfile <path>] [-t|--type] [-b|--batchsize <value>] [-i|--ip <value>]\n"
    "           [-p|--port <value>|-s|--search_string <value>] [-v|--verbose]\n"
    "           [-v|--verbose] [-h|--help]\n"
    "           <command> [<args>]";

typedef struct {
    char req_ip[MAX_IP];
    char req_ident[MAX_IDENT];
    char req_user[MAX_USER];
    char req_datetime[MAX_DATETIME];
    char req_method[MAX_METHOD];
    char req_uri[MAX_LINE_LENGTH];
    char req_proto[MAX_PROTO];
    int resp_code;
    char resp_bytes[MAX_RESP_BYTES];
    char req_referer[MAX_REFERER];
    char req_agent[MAX_AGENT];
} st_http_request;

struct globalArgs_t {
    const char *outFileName;	/* -o option */
    FILE *outFile;
    char *type;			/* -t option */
    char *batch_size;		/* -b option */
    char *ip;			/* -i option */
    char *port;			/* -p option */
    char *search_string;	/* -s option */
    int verbose;		/* verbosity */
    char **inputFiles;		/* input files */
    int numInputFiles;		/* # of input files */
} globalArgs;

static const char *optString = "o:t:b:i:p:s:vh?";

static const struct option longOpts[] = {
    {"outFileName", required_argument, NULL, 'o'},
    {"type", required_argument, NULL, 't'},
    {"batch_size", required_argument, NULL, 'b'},
    {"ip", required_argument, NULL, 'i'},
    {"port", required_argument, NULL, 'p'},
    {"search_string", required_argument, NULL, 's'},
    {"verbose", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {NULL, no_argument, NULL, 0}
};

void display_usage(void)
{
    puts(chopper_usage_string);
    exit(EXIT_FAILURE);
}

int flush_to_disk(st_http_request * p, int counter)
{
    FILE *pWrite;
    pWrite = fopen(globalArgs.outFileName, "a+");
    //printf("flushing %d lines to disk\n", counter+1);
    int flush_count;
    for (flush_count = 0; flush_count < counter; flush_count++) {
	fprintf(pWrite, "%s %d %s\n", p[flush_count].req_uri,
		p[flush_count].resp_code, p[flush_count].resp_bytes);
    }
    fclose(pWrite);
    return (0);
}

int flush_to_mongo(st_http_request * p, int counter)
{

    int flush_count;
    bson b;
    mongo conn;

    /* Now make a connection to MongoDB. */
    if( mongo_connect( &conn, globalArgs.ip, atoi(globalArgs.port) ) != MONGO_OK ) {
      switch( conn.err ) {
        case MONGO_CONN_SUCCESS:
          printf("Connected to mongo\n"); 
        case MONGO_CONN_NO_SOCKET:
          printf( "FAIL: Could not create a socket!\n" );
          break;
        case MONGO_CONN_ADDR_FAIL:
          printf( "FAIL: MONGO_CONN_ADDR_FAIL: %s\n",globalArgs.ip);
          break;
        case MONGO_CONN_NOT_MASTER:
          printf( "FAIL: MONGO_CONN_NOT_MASTER\n");
          break;
        case MONGO_CONN_BAD_SET_NAME:
          printf( "FAIL: MONGO_CONN_BAD_SET_NAME\n");
          break;
        case MONGO_CONN_NO_PRIMARY:
          printf( "FAIL: MONGO_CONN_NO_PRIMARY\n");
          break;
        case MONGO_IO_ERROR:
          printf( "FAIL: MONGO_IO_ERROR\n");
          break;
        case MONGO_SOCKET_ERROR:
          printf( "FAIL: MONGO_SOCKET_ERROR\n");
          break;
        case MONGO_READ_SIZE_ERROR:
          printf( "FAIL: MONGO_READ_SIZE_ERROR\n");
          break;
        case MONGO_COMMAND_FAILED:
          printf( "FAIL: MONGO_COMMAND_FAILED\n");
          break;
        case MONGO_WRITE_ERROR:
          printf( "FAIL: MONGO_WRITE_ERROR\n");
          break;
        case MONGO_NS_INVALID:
          printf( "FAIL: MONGO_NS_INVALID\n");
          break;
        case MONGO_BSON_INVALID:
          printf( "FAIL: MONGO_BSON_INVALIDr\n");
          break;
        case MONGO_BSON_NOT_FINISHED:
          printf( "FAIL: MONGO_BSON_NOT_FINISHED\n");
          break;
        case MONGO_BSON_TOO_LARGE:
          printf( "FAIL: MONGO_BSON_TOO_LARGEr\n");
          break;
        case MONGO_WRITE_CONCERN_INVALID:
          printf( "FAIL: Mongo write concern invalid\n");
          break;
        case MONGO_CONN_FAIL:
          printf( "FAIL: Mongo connection fail. Make sure it's listening at %s:%s\n",globalArgs.ip,globalArgs.port);
          break;
      }

      exit( 1 );
    }

    for (flush_count = 0; flush_count < counter; flush_count++) {

    bson_init( &b );
    //printf("req_ip: %s\n",p[flush_count].req_ip);
    //printf("req_ident: %s\n",p[flush_count].req_ident);
    //printf("req_user: %s\n",p[flush_count].req_user);
    //printf("req_datetime: %s\n",p[flush_count].req_datetime);
    //printf("req_method: %s\n",p[flush_count].req_method);
    //printf("req_uri: %s\n",p[flush_count].req_uri);
    //printf("req_proto: %s\n",p[flush_count].req_proto);
    //printf("resp_code: %d\n",p[flush_count].resp_code);
    //printf("resp_bytes: %s\n",p[flush_count].resp_bytes);
    //printf("req_referer: %s\n",p[flush_count].req_referer);
    //printf("req_agent: %s\n",p[flush_count].req_agent);
    bson_append_string( &b, "req_ip", p[flush_count].req_ip );
    bson_append_string( &b, "req_ident", p[flush_count].req_ident );
    bson_append_string( &b, "req_user", p[flush_count].req_user );
    bson_append_string( &b, "req_datetime", p[flush_count].req_datetime );
    bson_append_string( &b, "req_method", p[flush_count].req_method );
    bson_append_string( &b, "req_uri", p[flush_count].req_uri );
    bson_append_string( &b, "req_proto", p[flush_count].req_proto );
    bson_append_int( &b, "resp_code", p[flush_count].resp_code );
    bson_append_int( &b, "resp_bytes", atoi(p[flush_count].resp_bytes) );
    bson_append_string( &b, "req_referer", p[flush_count].req_referer );
    bson_append_string( &b, "req_agent", p[flush_count].req_agent );
    bson_append_finish_object( &b );

    bson_finish( &b );
    //bson_print( &b );
    if( mongo_insert( &conn, "logs.records", &b, NULL ) != MONGO_OK ) {
      printf( "FAIL: Failed to insert document with error %d\n", conn.err );
      exit( 1 );
    }
    bson_destroy( &b );
    }
    mongo_destroy( &conn );
    return (0);
}

int flush_to_stdout(st_http_request * p, int counter)
{
    int flush_count;
    for (flush_count = 0; flush_count < counter; flush_count++) {
	printf("%s %d %s\n", p[flush_count].req_uri,
	       p[flush_count].resp_code, p[flush_count].resp_bytes);
    }
    return (0);
}

int chop(void)
{
    printf("outFileName: %s\n", globalArgs.outFileName);
    printf("type: %s\n", globalArgs.type);
    printf("batch_size: %s\n", globalArgs.batch_size);
    printf("ip: %s\n", globalArgs.ip);
    printf("port: %s\n", globalArgs.port);
    printf("search_string: %s\n", globalArgs.search_string);
    printf("verbose: %d\n", globalArgs.verbose);
    printf("numInputFiles: %d\n", globalArgs.numInputFiles);

    //FILE *pRead;
    int running_total = 0;

    int f_count;
    st_http_request *p;
    p = (st_http_request *) calloc(BATCH_SIZE, sizeof(st_http_request));
    printf("Size of st_http_request: %lu\n", sizeof(st_http_request));
    for (f_count = 0; f_count < globalArgs.numInputFiles; f_count++) {
	//printf("Scanning file: %s \t", globalArgs.inputFiles[f_count]);
	gzFile pRead = gzopen(globalArgs.inputFiles[f_count], "r");
	char log_line[MAX_LINE_LENGTH];
	int counter = 0;
	int line_length = 0;
	int use_batch_size;
	if (globalArgs.batch_size != '\0') {
	    use_batch_size = atoi(globalArgs.batch_size);
	} else {
	    use_batch_size = BATCH_SIZE;
	}

	while (gzgets(pRead,log_line,8192) != NULL) {
	    line_length = strlen(log_line);
	    if (line_length > MAX_LINE_LENGTH - 1) {
		printf
		    ("Encountered a stupidly long line of over 8KB\nLog file must be poop. Exiting.");
		exit(1);
	    } else {
		if ((globalArgs.search_string != NULL)
		    && (strstr(log_line, globalArgs.search_string) ==
			NULL))
		    continue;
		if (strstr(log_line, "\"EOF\""))
		    continue;
		sscanf(log_line,
		       "%s %s %s [%[^]]] \"%s %s %[^\"]\" %d %s \"%[^\"]\" \"%[^\"]\"",
		       p[counter].req_ip, p[counter].req_ident,
		       p[counter].req_user, p[counter].req_datetime,
		       p[counter].req_method, p[counter].req_uri,
		       p[counter].req_proto, &p[counter].resp_code,
		       p[counter].resp_bytes, p[counter].req_referer,
		       p[counter].req_agent);
		running_total++;
                //printf("req_ip: %sX\n",p[counter].req_ip);
                //printf("req_datetime: %sX\n",p[counter].req_datetime);
                //printf("req_method: %sX\n",p[counter].req_method);
	    }
	    if (counter == (use_batch_size)) {
		if (globalArgs.outFileName != NULL)
		    flush_to_disk(p, counter);
		if (globalArgs.ip != NULL && globalArgs.port != NULL)
		    flush_to_mongo(p, counter);
		if (globalArgs.outFileName == NULL && globalArgs.ip == NULL && globalArgs.port == NULL)
		    flush_to_stdout(p, counter);
		counter = 0;
	    } else {
		counter++;
	    }
	}
	if (globalArgs.outFileName != NULL)
	    flush_to_disk(p, counter);
	if (globalArgs.ip != NULL && globalArgs.port != NULL)
	    flush_to_mongo(p, counter);
	if (globalArgs.outFileName == NULL && globalArgs.ip == NULL && globalArgs.port == NULL)
	    flush_to_stdout(p, counter);

        printf("Scanned a total of: %d lines.\n", running_total);
        gzclose(pRead);

    }
    
    free(p);
    return (0);
}

int main(int argc, char *argv[])
{
    int opt = 0;
    int longIndex = 0;
    globalArgs.outFileName = NULL;
    globalArgs.outFile = NULL;
    globalArgs.type = NULL;
    globalArgs.batch_size = NULL;
    globalArgs.ip = NULL;
    globalArgs.port = NULL;
    globalArgs.search_string = NULL;
    globalArgs.verbose = 0;
    globalArgs.inputFiles = NULL;
    globalArgs.numInputFiles = 0;

    opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
    while (opt != -1) {
	switch (opt) {
	case 'o':
	    globalArgs.outFileName = optarg;
	    break;
	case 't':
	    globalArgs.type = optarg;
	    break;
	case 'b':
	    globalArgs.batch_size = optarg;
	    break;
	case 'i':
	    globalArgs.ip = optarg;
	    break;
	case 'p':
	    globalArgs.port = optarg;
	    break;
	case 's':
	    globalArgs.search_string = optarg;
	    break;
	case 'v':
	    globalArgs.verbose = 1;
	    break;
	case 'h':
	case '?':
	    display_usage();
	    break;
	case 0:
	    display_usage();
	    break;
	default:
	    display_usage();
	    break;
	}

	opt = getopt_long(argc, argv, optString, longOpts, &longIndex);
    }

    globalArgs.inputFiles = argv + optind;
    globalArgs.numInputFiles = argc - optind;
    if (globalArgs.numInputFiles <= 0)
	display_usage();

    chop();
    exit(0);
}
