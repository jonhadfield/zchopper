#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "chopper.h"

/* short options:
 * *
 * * -o outfile name - write to file rather than stdout
 * * -t output type
 * * -b batch size - number of lines to process before flushing - debug only
 * * -h db host
 * * -p port of db server
 * * -c mongo db and collection
 * * -s search string
 * * -v verbose output
 * * additional file names are used as input files
 * *
 * *
 * * long options:
 * *
 * * --outfile
 * * --type
 * * --batchsize
 * * --host
 * * --port
 * * --collection
 * * --search_string
 * * --verbose
 * *
 * */

const char chopper_usage_string[] =
    "chopper [-s|--search_string] [-o|--outfile <path>] [-t|--type] [-b|--batchsize <value>] [-h|--host <value>]\n"
    "           [-p|--port <value>] [-c|--collection <db.collection>][-s|--search_string <value>] [-v|--verbose]\n"
    "           [-v|--verbose] [-h|--help]\n"
    "           <command> [<args>]";

static const char *optString = "o:t:b:h:p:c:s:v?";

static const struct option longOpts[] = {
    {"outFileName", required_argument, NULL, 'o'},
    {"type", required_argument, NULL, 't'},
    {"batch_size", required_argument, NULL, 'b'},
    {"host", required_argument, NULL, 'h'},
    {"port", required_argument, NULL, 'p'},
    {"collection", required_argument, NULL, 'c'},
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

int main(int argc, char *argv[])
{
    int opt = 0;
    int longIndex = 0;
    globalArgs.outFileName = NULL;
    globalArgs.outFile = NULL;
    globalArgs.type = NULL;
    globalArgs.batch_size = NULL;
    globalArgs.host = NULL;
    globalArgs.port = 27017;
    globalArgs.collection = NULL;
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
	case 'h':
	    globalArgs.host = optarg;
	    break;
	case 'p':
	    globalArgs.port = atoi(optarg);
	    break;
	case 'c':
	    globalArgs.collection = optarg;
	    break;
	case 's':
	    globalArgs.search_string = optarg;
	    break;
	case 'v':
	    globalArgs.verbose = 1;
	    break;
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
