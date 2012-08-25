#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
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
    for (flush_count = 0; flush_count < counter; flush_count++) {
	printf("flush_count: %d\n", flush_count);
    }
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

    FILE *pRead;
    int running_total = 0;

    int f_count;
    st_http_request *p;
    p = (st_http_request *) calloc(BATCH_SIZE, sizeof(st_http_request));
    printf("Size of st_http_request: %lu\n", sizeof(st_http_request));
    for (f_count = 0; f_count < globalArgs.numInputFiles; f_count++) {
	printf("File %d: %s\n", f_count, globalArgs.inputFiles[f_count]);
	pRead = fopen(globalArgs.inputFiles[f_count], "r");
	char log_line[MAX_LINE_LENGTH];
	int counter = 0;
	int line_length = 0;
	int use_batch_size;
	if (globalArgs.batch_size != '\0') {
	    use_batch_size = atoi(globalArgs.batch_size);
	} else {
	    use_batch_size = BATCH_SIZE;
	}

	while (fgets(log_line, 8192, pRead) != NULL) {
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
	    }
	    if (counter == (use_batch_size)) {
		if (globalArgs.outFileName != NULL)
		    flush_to_disk(p, counter);
		if (globalArgs.ip != NULL && globalArgs.port != NULL)
		    flush_to_mongo(p, counter);
		if (globalArgs.outFileName == NULL)
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
	if (globalArgs.outFileName == NULL)
	    flush_to_stdout(p, counter);

	printf("Scanned a total of: %d lines.\n", running_total);

	fclose(pRead);
    }
    free(p);
//if (globalArgs.outFileName != NULL)
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
