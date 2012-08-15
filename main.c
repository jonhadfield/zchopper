#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

struct st_http_request {
	char ip[15];
	char ident[10];
};

int main (int argc, char *argv[]) {

  FILE *pRead;
  char log_line[5000];
  pRead = fopen("log.log", "r");

//Create array of 20 of st_http_request
  struct st_http_request **temp_struct;
  temp_struct = malloc(20 * sizeof *temp_struct);

  if (temp_struct == NULL) { printf("Pants"); }

  //int i=0;
  //for (i = 0; i < 20; ++i) {
  //  temp_struct[i] = malloc(sizeof *temp_struct[i]);
  //  strcpy(temp_struct[i]->ip,"1.2.3.4");
  //  printf("log_line %d: ip:%s\n",i,temp_struct[i]->ip);
  //}

  int counter = 0;
  while (fgets(log_line, 500, pRead) != NULL) {
    temp_struct[counter] = malloc(sizeof *temp_struct[counter]);

    //struct st_temp **tmp;
    //tmp = realloc(temp_struct, 30 * sizeof *temp_struct);
    //if (tmp == NULL) { 
	// handle failed realloc, temp_struct is unchanged
    //} else {
	// everything went ok, update the original pointer (temp_struct)
	//temp_struct = tmp; 
    //}

    sscanf(log_line, "%s %s", temp_struct[counter]->ip, temp_struct[counter]->ident);
    printf("log_line %d: ip:%s ident:%s\n",counter,temp_struct[counter]->ip,temp_struct[counter]->ident);
    counter++;
  }


  // We need more space ... remember to use a temporary variable
  //struct st_http_request **tmp;
  //tmp = realloc(temp_struct, 30 * sizeof *temp_struct);
  //if (tmp == NULL) { 
  //  printf("Cannot allocate any more memory. Pants :(");
    // handle failed realloc, temp_struct is unchanged
  //} else {
    // everything went ok, update the original pointer (temp_struct)
  //  temp_struct = tmp; 
  //}
  //int j=0;
  //for (i = 20; i < 30; ++i) { // notice the indexing, [20..30)
    // NOTICE: the realloc allocated more space for pointers
    // we still need to allocate space for each new object
  //  temp_struct[i] = malloc(sizeof *temp_struct[i]);
  //  strcpy(temp_struct[i]->ip,"8.8.8.8");
  //}
  // temp_struct now "holds" 30 temp_struct objects
  // ...
  // and always do remember, in the end
  //int k=0;
  //for (k = 0; i < 30; ++i)
  //  free(temp_struct[i]);
  //  free(temp_struct);

  return(0);
}
