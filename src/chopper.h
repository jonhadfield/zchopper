#define  MAX_LINE_LENGTH  8192
#define  MAX_IP             16
#define  MAX_IDENT           8
#define  MAX_USER            8
#define  MAX_DATETIME       27
#define  MAX_METHOD          7
#define  MAX_URI          2083  //Safe limit?
#define  MAX_PROTO           8
#define  MAX_RESP_BYTES     20
#define  MAX_REFERER      2083  //Safe limit?
#define  MAX_AGENT        1024  //Safe limit?
#define  BATCH_SIZE       100

/* short options:
*
* -o outfile name - write to file rather than stdout
* -t output type
* -b batch size - number of lines to process before flushing - debug only
* -i ip of db server
* -p port of db server
* -s search string
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
* --search_string
* --verbose
*
*/
