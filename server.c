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


struct score_board *board;
//Denne metoden håndterer ctrl-c
void signal_callback_handler(int signum)
{
   printf("Systemet avslutter........printer ut highscore lista %d\n",signum);
   // Cleanup and close up stuff here
   board_destroy(board);
   // Terminate program
   exit(signum);
}

int main(int argc, char *argv[]){
  
  board = board_init(25,25);
  
  signal(SIGINT,signal_callback_handler);
  
  
  if(argc!=2){
    printf("need more port number, system aborting....\n");
    exit(EXIT_SUCCESS);
  }
  
  while(1){
    char buf[13];
    char user[13];
    
    int port = 0;
    port = atoi(argv[1]);
    
    printf("connection portnr = %d \n",port); // skriver ut portnummer
    
    int listen_desc, conn_desc; // lytter
    
    int maxfd; // max verdi dekriptor for array
    
    int queue;
    
    fd_set tempset, savedset;  // descriptor som skal moniteres
    
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
    
    
    maxfd = listen_desc; // Itarte "max descriptor" med en gyldig verdi
    
    
    FD_ZERO(&savedset); // tømmer "descriptor" 
    
    FD_SET(listen_desc, &savedset); // legger til midlertidig "listening descriptor" 
    
    
    while(1) {
     
      // Setter alle descriptorene til en local variabel, dette trenger jeg fordi select vil overskrive og vi vil ikke miste "tråden"
     
      tempset = savedset;
      
      queue = select(maxfd+1, &tempset, NULL, NULL, NULL); // settes til å vente til data kommer
     
      if(queue) // ny klient
	
	{
	  
	  
	  int size = sizeof(client_addr);

	  conn_desc = accept(listen_desc, (struct sockaddr *)&client_addr, &size);
	  

	  FD_SET(conn_desc, &savedset);
	  //legger til descriptor
	  if(conn_desc > maxfd){
	    
	    maxfd = conn_desc; // max for select
	   
	  
	    read(conn_desc, buf, 12);
	    buf[12] = '\0';
	    
	    //brukes til å hente top 5

	    if(strcmp(buf,"topp5")==0){
	      int teller;
	      char navn[15];
	      uint32_t skore;
	      char bufScore[15];
	      for(teller=1; teller<=5; teller++){
		
		//vi sender "" tom hvis den ikke finnes
		if(board_get_pos(board,teller,&skore,navn,15)!=0){
		
		  write(conn_desc,"",12);
		
		  write(conn_desc,"",12);

		}else{
		  
		  //fra int til char
		  sprintf(bufScore,"%d",skore);
		  
		  //sender tilbake navn
		  write(conn_desc,navn,12);
		  
		  //sender tilbake "score"
		  write(conn_desc,bufScore,12);
		}
	      }
	    }
	    
	    else {
	      
	      //leser fra forbindelsen
	      read(conn_desc,user,12);
	      
	      user[12]='\0';
	      board_add_score(board,atoi(buf),user);
	      
	      //printer ut til terminalen
	      printf("name : %s | score : %s\n",user, buf);
	      //printf("usename : %s\n",user);
	      
	      write(conn_desc, buf, 12);
	      
	      write(conn_desc, user,12);
	    }
	  }
	  
	} // End main listening loop
      
    }
    close(conn_desc); //lukker socketen
    close(listen_desc); 
    sleep(1);
    
  }
  
  return EXIT_SUCCESS;
}




