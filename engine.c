#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h> // the actual game engine
#include <unistd.h>  // for the usleep () function
#include<netinet/in.h>
#include<sys/socket.h>
#include<netdb.h>
#include "engine.h"
#include "player.h"

static void klient();
static void ip_nr();
static void port_nr();
static void send_dir(int directions);

int sd;
char *spillerid;
/** This SIMPLE formula determines a time (in microseconds) the game
 *  must wait between each screen refresh. It is based on the current
 *  game level, so the higher its value, the lowest is the delay and
 *  faster the game will appear to be. */

#define   REFRESH_DELAY     (50000 + ((9 - game.level) * 10000))


static char PLAYER_HEAD_CHAR = '@'; ///< The 'image' of the snake head
static char PLAYER_CHAR	     = 'o'; ///< The 'image' of the snake body
static char BORDER_CHAR	     = '#'; ///< The 'image' of the border in normal mode
static char MENU_BORDER_CHAR = '*'; ///< The 'image' of the border in the main menu


/*Simple enum to make the colors easier to read: FOREGROUND_BACKGROUND */
enum Colors { BLACK_WHITE = 1, CYAN_BLACK, BLUE_BLACK,
              WHITE_BLACK, GREEN_BLACK, RED_BLACK,  };

struct screen_t screen;
struct game_t game;
//char tast[15];

/**	Just erases everything to black
 */

void draw_background ()
{
  clear();
}

char *paramet;
void sjek_param(char *param){

  strncpy(paramet,param, strlen(param));
}
/* Draws the window border */
void draw_borders ()
{
  int i;

  attron (COLOR_PAIR (WHITE_BLACK));
  for (i = 0; i <= (screen.width-1); i++)	
    {
      mvaddch (1, i, BORDER_CHAR);
      mvaddch ((screen.height-1), i, BORDER_CHAR);
    }
  for (i = 1; i <= (screen.height-1); i++)	
    {
      mvaddch (i, 0, BORDER_CHAR);
      mvaddch (i, (screen.width-1), BORDER_CHAR);
    }

}
//returner score
int score(){
  return tron.score;
}


/* Draws the snake - from the head to the whole body */
void draw_player ()
{
  attron (COLOR_PAIR (GREEN_BLACK));
  mvaddch (tron.body[0].y, tron.body[0].x, PLAYER_HEAD_CHAR);

  int i;
  for (i = 1; i < tron.size; i++)
    mvaddch (tron.body[i].y, tron.body[i].x, PLAYER_CHAR);

  /* 
    attron (COLOR_PAIR (RED_BLACK));    
    mvaddch (tron2.body[0].y, tron2.body[0].x, PLAYER_HEAD_CHAR);

    int j;
    for (j = 1; j < tron2.size; j++)
    mvaddch (tron2.body[j].y, tron2.body[j].x, PLAYER_CHAR);
  */
	
}


/* Prints the current score */
void draw_score ()
{
  attron (COLOR_PAIR (WHITE_BLACK));
  mvprintw (0, 0,  "----------------   Level (speed): %2d", game.level);
  mvprintw (0, 36, " ------------ Score: %4d ------------------", tron.score);
}


/* Exits and dealocates the memory required by ncurses */
void engine_exit ()
{
  clear ();
  refresh ();
  // Effectively ends ncurses mode
  endwin ();
}


//char tast[15];
/**	Get the user input during game and make the right decisions
 */
void engine_get_game_input ()
{
  // The input variable MUST be int to accept non-ascii characters
  int input = getch ();
  
  switch (input)
    {
      
    case ERR:
      // If we get no input
      break;
      
    case KEY_UP:
      
      //  send_dir(input);
      player_change_direction (UP);
      break;
      
    case KEY_LEFT: 
    
      // send_dir(input);
      player_change_direction (LEFT);
      break;
      
    case KEY_DOWN:
      
      //send_dir(input);
      player_change_direction (DOWN);
      break;
      
    case KEY_RIGHT:
      
      //send_dir(input);
      player_change_direction (RIGHT);
      break;
      
    case 'q':	
    case 'Q':
      engine_exit ();
      ifitron_exit ();
      break;

      
    default:
      break;
    }
}
int argu;
void arguments(int antall){
  argu= antall;
}
/**	Starts the game engine. Initializes all the stuff related to ncurses.
 *
 *  @note If some engine-specific initialization fails, the game aborts.
 */
void engine_init ()
{
  screen.width  = 80;
  screen.height = 24;

  // Starts the ncurses mode
  initscr ();

  if (has_colors() == FALSE)
    ifitron_abort ("Your terminal does not support colors.\n");

  // Start support for colors ( Name, Foreground, Background )
  start_color ();
  init_pair (GREEN_BLACK, COLOR_GREEN, COLOR_BLACK);
  init_pair (CYAN_BLACK,  COLOR_CYAN,  COLOR_BLACK);
  init_pair (WHITE_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair (RED_BLACK,   COLOR_RED,   COLOR_BLACK);
  init_pair (BLUE_BLACK,  COLOR_BLUE,  COLOR_BLACK);
  init_pair (BLACK_WHITE, COLOR_BLACK, COLOR_WHITE);

  int current_height, current_width;
  // Gets the current width and height of the terminal
  getmaxyx (stdscr, current_height, current_width);

  if ((current_width < screen.width) || (current_height < screen.height))
    ifitron_abort ("Your console screen is smaller than 80x24\n"
		   "Please resize your window and try again\n\n");

  // Character input doesnt require the <enter> key anymore
  raw ();

  // Makes the cursor invisible
  curs_set (0);

  // Support for extra keys (life F1, F2, ... )
  keypad (stdscr, true);

  // Wont print the input received
  noecho ();

  // Wont wait for input - the game will run instantaneously
  nodelay (stdscr, true);

  // Refresh the screen (prints whats in the buffer)
  refresh ();
}


/**	Draws the Game Over screen.
 *
 *	Besides drawing 'Game Over', it highlights where the player died.
 */
void engine_show_game_over ()
{
  attron (COLOR_PAIR (RED_BLACK));
  mvaddch (tron.body[0].y, tron.body[0].x, 'x');
  
  mvprintw(9, 17, "GAME OVER");
  mvprintw(10, 17, "Username %s", spillerid);
  mvprintw(11, 17, "Poeng %4d", tron.score);
  mvprintw(12, 17, "Returning to main menu");
  draw_score ();
  
  refresh ();
  
}
char buffSc[15];

void send_score(){
  
  sprintf(buffSc,"%d", tron.score);

  //write(sd,"%d",buffSc);
} 

/* Displays the main menu and gets the user input from it.
 *
 * This function blocks the game execution and waits for user input,
 * refreshing the main menu screen according to the options selected. */
char buf[13];
char user[13];

char *Hip = "";
int Hport = 0;

char *Mip = "";
int Mport = 0;
void Hportnr(char *portnr){
  Hport =atoi(portnr);
}
void Hipnr(char *ipnr){
  Hip= ipnr;
}
void Mportnr(char *portnr){
  Mport =atoi(portnr);
}
void Mipnr(char *ipnr){
  Mip= ipnr;
}
void playername(char *username){
  
  spillerid = username;
  
}
//denne henter de top 5 beste fra serveren og viser dem i "main screen"
void get_top5(){
  struct sockaddr_in serveraddr;
  
  char buf[13];
  char user[13];
  
  sd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  
  bzero(&serveraddr,sizeof(struct sockaddr_in));
  
  serveraddr.sin_family = AF_INET;
  
  inet_pton(AF_INET, Hip, &serveraddr.sin_addr);
  
  serveraddr.sin_port = htons(Hport);
  
  connect(sd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr_in));
  
  write(sd,"topp5", 12 );
  
  int teller;
  for(teller=0; teller<5; teller++){
    
    read(sd,buf,12);
    buf[12]='\0';
    read(sd,user,12);  
    
    if(buf[0]=='\0'){
      continue;
    } 		 
    user[12]='\0';
    
    attron (COLOR_PAIR (GREEN_BLACK));
    mvprintw(17+teller,12,"Name : %s Score : %s",buf,user);
    
  }

}
void engine_show_main_menu ()
{
  int wait = TRUE;
  
  int speed_option = 1;
  char speed_options[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  
  int menu_x_padding = 18;
  int option_x_padding = menu_x_padding + 17;
  
  clear ();

  if(argu>3){
    get_top5();
  }

  while (wait == TRUE)
    {
      // The borders
      attron (COLOR_PAIR (WHITE_BLACK));
      int i;
      for (i = 0; i < screen.width; i++)
	{
	  mvaddch (0, i, MENU_BORDER_CHAR);
	  mvaddch (screen.height - 1, i, MENU_BORDER_CHAR);
	}
      for (i = 0; i < screen.height; i++)
	{
	  mvaddch (i, 0, MENU_BORDER_CHAR);
	  mvaddch (i, screen.width - 1, MENU_BORDER_CHAR);
	}
      
      attron (COLOR_PAIR (GREEN_BLACK));
      
      mvprintw (8, 12,  "IFI-TRON");
      mvprintw (9,  12, "Player name : %s",spillerid);

      attron (COLOR_PAIR (BLUE_BLACK));
      mvprintw (10, 12, "+---------------------------------------------------+");
      mvprintw (11, 12, "|                                                   |");
      mvprintw (12, 12, "|                                                   |");
      mvprintw (13, 12, "|                                                   |");
      mvprintw (14, 12, "|                                                   |");
      mvprintw (15, 12, "|                                                   |");
      mvprintw (16, 12, "+---------------------------------------------------+");
      mvprintw (12, menu_x_padding, "Press <enter> or <space> to start game");
      mvprintw (13, menu_x_padding, "Press <q> to quit game");
      
      // And here we draw the level numbers
      attron (COLOR_PAIR (BLUE_BLACK));
      mvprintw (15, menu_x_padding, "Starting speed/level:");
      
      // Tricky, draw the options with the right colors
      int j;
      for (i = 0, j = 0; i < 9; i++)
	{
	  if (i == (speed_option-1))
	    attron (COLOR_PAIR (WHITE_BLACK));
	  else
	    attron (COLOR_PAIR (BLUE_BLACK));
	  
	  mvprintw (15, option_x_padding+j+6, "%c", speed_options [i]);
	  j += 2;
	}
      
      attron (COLOR_PAIR (WHITE_BLACK));
      mvprintw (screen.height-3, 3, "Use --help for guidelines");
      
      // Now we wait for orders
      wait = get_main_menu_input (&speed_option);
      
      // This function is so refreshing...
      refresh ();
    }
  
  game.level = speed_option;
}




/**	Completely draws the screen during game.
 *
 * 	The usleep() function "stops" the program for 'n' microseconds.
 */
void engine_show_screen ()
{
  draw_background ();
  draw_borders ();
  draw_player ();
  draw_score ();

  usleep (REFRESH_DELAY);

  refresh();
}


/* Gets input for the main menu */
int get_main_menu_input (int* speed_option)
{
  int input = getch();

  switch (input)
    {
    case ERR: // no input
      break;

    case '\n': case ' ':
      return FALSE;
      break;

    case 'q': case 'Q':
      engine_exit();
      ifitron_exit();
      break;

    case KEY_LEFT:
      if (*speed_option > 1) (*speed_option)--;
      break;

    case KEY_RIGHT:
      if (*speed_option < 9) (*speed_option)++;
      break;

    case '1':
      *speed_option = 1;
      break;
    case '2':
      *speed_option = 2;
      break;
    case '3':
      *speed_option = 3;
      break;
    case '4':
      *speed_option = 4;
      break;
    case '5':
      *speed_option = 5;
      break;
    case '6':
      *speed_option = 6;
      break;
    case '7':
      *speed_option = 7;
      break;
    case '8':
      *speed_option = 8;
      break;
    case '9':
      *speed_option = 9;
      break;

    default:
      break;
    }

  return TRUE;
}


/* Aborts the game and displays the error message */
void ifitron_abort (char* error_msg)
{
  engine_exit ();
  printf ("%s", error_msg);
  exit (EXIT_FAILURE);
}

/* Interrupts the game and quits to the terminal. */
void ifitron_exit ()
{
  if (tron.body != NULL)
    {
      free (tron.body);
      tron.body = NULL;
    }
  exit (EXIT_SUCCESS);
}




/* Finish the game after haveing lost a life. */
void ifitron_game_over ()
{
  
  if(argu > 3){
    send_score();
    klient();
  }
  engine_show_game_over ();
  usleep (4000000); // Wait 4 seconds before returning...
  engine_show_main_menu ();
  ifitron_init ();
}


/* Starts all the necessairy stuff */
void ifitron_init ()
{
  player_init ();
  engine_show_screen ();
}

//kontakter serveren for å sende highscore
void klient(){

  
  struct sockaddr_in serveraddr;
  
  char buf[13];
  char user[13];

  sd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  
  bzero(&serveraddr,sizeof(struct sockaddr_in));
  
  serveraddr.sin_family = AF_INET;
  
  inet_pton(AF_INET, Hip, &serveraddr.sin_addr);

  
  serveraddr.sin_port = htons(Hport);
  
  if(connect(sd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr_in))<0){
     printf("error");
     exit(0);
  }
  //sender med score til serveren
  write(sd,buffSc , 12 );

  //sender med navn til serveren
  write(sd,spillerid,12);
  
  //lagrer i score i en buf
  read(sd, buf, 12 );

  //lagrer i user
  read(sd, user, 12 );

  buf[12] = '\0';
  user[12]='\0';


  close(sd);

}

char tast[15];// tar var på taste verdiene

//Directons for opponnent, denne ble jeg ikke helt ferdig med
void send_dir(int direction){

  struct sockaddr_in serveraddr;
  
  sd = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
  
  bzero(&serveraddr,sizeof(struct sockaddr_in));
  
  serveraddr.sin_family = AF_INET;
  
  inet_pton(AF_INET, Mip, &serveraddr.sin_addr);

  
  serveraddr.sin_port = htons(Mport);
  
  connect(sd,(struct sockaddr*)&serveraddr,sizeof(struct sockaddr_in));

  //direction = getch();
  sprintf(tast,"%d",direction);
  
  write(sd,tast,12);
  //send(sd,tast,12,MSG_DONTWAIT);
  
  read(sd, buf,12);
  //recv(sd,buf,12,MSG_DONTWAIT);
  
  switch (atoi(buf))
    {
      
    case ERR:
      // If we get no input
      break;
      
    case KEY_UP:
     
      player2_change_direction (UP);
      break;
      
    case KEY_LEFT: 
   
      player2_change_direction (LEFT);
      break;
      
    case KEY_DOWN:
  
      player2_change_direction (DOWN);
      break;
      
    case KEY_RIGHT:
     
      player2_change_direction (RIGHT);
      break;
      
    case 'q':	
    case 'Q':
      engine_exit ();
      ifitron_exit ();
      break;

      
    default:
      break;
    }

  close(sd);

}

	


