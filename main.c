#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct request_details
{
  char client_ip[15];
  char date[30];
};


int main()
{
struct request_details request1,*request1_ptr=&request1;
strcpy(request1_ptr->client_ip,"8.8.8.8");
strcpy(request1_ptr->date,"28/03/1977");

printf("request1 has ip of: %s\n",request1_ptr->client_ip);
printf("request1 has date of: %s\n",request1_ptr->date);


  FILE *pRead;
  /*req reqs[10]; */
  /*int count;*/
  /*char log_line[500];*/
  pRead = fopen("log.log", "r");
  if ( pRead == NULL )  
  {
    printf("\nFile cannot be opened\n");
    exit(1);
  }

/*
  fscanf(pRead, "%d", &count);
  fscanf(pRead, "%[^\n]", &log_line);
   
  printf("line: %s\n", log_line);
*/
  int count1;
  char aline[500];
  /*char req_ip[15];*/
  while (fgets(aline, 500, pRead) != NULL) {
    /*sscanf(aline, "%s %s", array[count1][0],array[count1][1]);*/
    printf("line %d: %s\n",count1,aline);
    count1++;
  }
  printf("File contains: %d lines\n",count1); 

/*for(k = 0; k < count; k++)
{
  fscanf(pRead,"%s",reqs[k].client_ip);
}
*/    
  fclose(pRead);
  return(0);
}