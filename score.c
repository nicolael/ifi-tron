#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include "score.h"

//definerer board
struct score_board { 

char username[25];
uint32_t score;
size_t size;
size_t name_len;
};

/** 
board_init returnerer et nytt high-score board
**/

//oppretter strukturen til board
struct score_board* board_init(size_t size, size_t name_len) {

  struct score_board *board;
  
  int i=0;

  
  board = malloc(sizeof(struct score_board)*size);//allokering av minne
  
  for(i=0; i<size; i++){
    board[i].score = 0;
    board[i].size = size;
    board[i].name_len = name_len;
  }
  
  return board;
}

/** 
  board_add_score - returverdier:
  -1 = error
  0  = ikke god nok score for Ã¥ fÃ¥ plass i listen
  X  = plassering for nye score i listen
 **/
int board_add_score(struct score_board *board, uint32_t score, const char *name)  {

  int i=0;
  if(board == NULL || score == 0){
    // printf("brettet er tom");
    return -1;
  }

  int tempScore = 0;
  int pos = 0;
  char tmp[25];
  char nytmp[25];

  //sorterer scorene
  for(i = 0; i<5; i++){
    
    if(board[i].score < score){
      
      tempScore = board[i].score; // score i en tmp variabel
      strncpy(tmp, board[i].username, board[i].name_len);//kopierer over til en tmp variabel slik at det ikke overskrives
      
      board[i].score = score;  //setter inn den høyeste verdien i lista
      strncpy(board[i].username, name, board[i].name_len);//kopierer over til list
      
      for(pos = i+1; pos<5; pos++){
	
	
	score = board[pos].score;
	
	board[pos].score = tempScore;
	strncpy(nytmp,board[pos].username, board[pos].name_len);
	
	strncpy(board[pos].username,tmp, board[pos].name_len);
	
	tempScore = score;
	strncpy(tmp, nytmp, strlen(nytmp));
      }
      return i+1;
    }
    
  }
  
return 0;      
}

/** 
    board_get_pos - returverdier:
    -1 = error 
    0 = funnet 
**/
//returnere 0 om en person er funnet -1 hvis error 
int board_get_pos(struct score_board *board, size_t pos, uint32_t *score, char *name_dest, size_t name_size) {
  
 
  // sjekker for NULL verdier 
  if(board == NULL || score == NULL || board[pos-1].score == 0 || pos > 5 ){
    
    // printf("brettet er tomt, no score");
    return -1;
  }

  //setter name_dest til å peke på et spillers navn
  strncpy(name_dest, board[pos-1].username ,board[pos-1].name_len);
  *score = board[pos-1].score;
  
  
  return 0;
}

/** 
  board_destroy - returnerer NULL hvis alt er i orden
  printer ut lista når highscore terminalen avsluttes med ctr-c
**/

struct score_board* board_destroy(struct score_board *board){
  
  int i = 0;
  for(i =0; i<25; i++){
    printf("%d)Name :  %s  Score : %d\n",i+1, board[i].username,board[i].score);
    // printf("Score : %d\n", board[i].score);
  }

  //frigør minne
  free(board);
  board = NULL;
  return 0;

}

