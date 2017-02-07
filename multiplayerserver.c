#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include "score.h"
#define MAX_SIZE 50
#include <signal.h>
#include "player.h"

void args_handler(int argc, char *argv[]);

struct score_board *board;
void signal_callback_handler(int signum)
{
  printf("Systemet avslutter......\n",signum);
 
  exit(signum);
}

int main(int argc, char *argv[]){
  
  board = board_init(25,25);
  
  signal(SIGINT,signal_callback_handler);
  
  
  if(argc!=2){
    printf("need port number, system aborting....\n");
    exit(EXIT_SUCCESS);
  }
  
  while(1){
    char buf[13];
    char user[13];
    
    int port = 0;
    port = atoi(argv[1]);
    
    printf("connection portnr = %d \n",port);
    
    int listen_desc, conn_desc; 
    
    int maxfd; 
    
    int queue;
    
    fd_set tempset, savedset; 
    
    struct sockaddr_in serv_addr, client_addr;
    
    char buff[MAX_SIZE];
    
    listen_desc = socket(AF_INET, SOCK_STREAM, 0);
    
    if(listen_desc < 0)
      
      printf("Failed creating socket\n");
    
    bzero((char *)&serv_addr, sizeof(serv_addr));
    
    serv_addr.sin_family = AF_INET;
    
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    serv_addr.sin_port = htons(port);
    
    
    if (bind(listen_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
      
      printf("Failed to bind\n");
    
    listen(listen_desc, 5);
    
    
    maxfd = listen_desc; 
    
    
    FD_ZERO(&savedset); 
    
    FD_SET(listen_desc, &savedset); 
    
    
    while(1) {
         
      tempset = savedset;
      
      queue = select(maxfd+1, &tempset, NULL, NULL, NULL); 
    
      if(queue) // new client connection
	
	{
	  
	  //printf("new client connection\n");
	  int size = sizeof(client_addr);

	  conn_desc = accept(listen_desc, (struct sockaddr *)&client_addr, &size);
	  //conn_desc = accept(listen_desc, (struct sockaddr *)&client_addr, &size);

	  FD_SET(conn_desc, &savedset); // add new descriptor to set of monitored ones
	 

	  if(conn_desc > maxfd){
	    
	    maxfd = conn_desc; // max for select
	    	    
	    //retninger
	    
	    read(conn_desc, buf, 12);
	    
	    buf[12] = '\0';
	    

	    printf("%d\n", atoi(buf));
	    
	    int j;
	    for(j = 0; j < maxfd;j++){
	      if(FD_ISSET(j, &savedset)){
		if(j != conn_desc)
		
	      }
	    }
	    write(conn_desc, buf, 12);
	    
	    
	  }
	  
	} // End main listening loop
      
    }
    close(conn_desc);
    close(listen_desc);
    sleep(1);
    
  }
  
  return EXIT_SUCCESS;
}
