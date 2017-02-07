#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>     /* For getopt_long() */

#include "engine.h"
#include "player.h"

void args_handle (int argc, char** argv);
void print_help();

/** Handles all the commandline arguments.
 *
 * This is the main function of this module. It processes
 * an argv[] array of argc elements based on the options
 * specified at the struct option.
 *
 * If you want to add another argument, follow these steps:
 * # Add an option on the struct option.
 * # Add the short equivalent (if it exists) on the getopt_long() call.
 * # Add a case on the switch specifying the action of that option.
 */

void args_handle (int argc, char** argv)
{
  
  
	static struct option options[] =
	{
	  {"help",         no_argument, NULL, 'h'},
	  {"spillerid",         required_argument, NULL, 'u'},
	  {"multiplayerserver",           required_argument,NULL, 'm'},
	  {"highscoreserver",   required_argument,NULL, 's'},
	  /* The last element must be all zeroes */
	  {0, 0, 0, 0}
	};
	/* The index of the current option */
	int option_index;
	/* The character for comparison */
	int c = 0;
	
	
	//sjekker antall arguments
	arguments(argc);


	/* We keep checking the arguments untill they run out (c == -1) */
	while (c != -1)
	{
		c = getopt_long (argc, argv, "h:u:m:", options, &option_index);

	   
		switch (c)
		{
		case -1:
		  // There were no '-' parameters passed
		  // or all the parameters were processed
		  break;
		  
		case 'u':
		  //optind
		  playername(optarg);
		  break;
		case 'm':
		  
		  Mipnr(argv[optind]);
		  Mportnr(argv[optind+1]);
		  
		  break;

		case 'h':
		  
		  if(optarg){
		  Hipnr(argv[optind]);
		  Hportnr(argv[optind+1]);
		  }else{
		  
		   default:
		  print_help();
		  exit(EXIT_SUCCESS);
		  break;
		  }

		}
	}

	/* Just in case the user specified more arguments (not options)
	 * than needed, you decide what to do. Here, we just ignore them */
	while (optind < argc) optind++;
}


/**	Prints Help instructions on standard output.
 */
void print_help(void)
{
  printf("Synopsis:\n");
  printf("\tThe classic TRON game.\n\n");

  printf("Controls:\n");
  printf("\tNumbers (1 ~ 9)   Changes the game speed at the main menu\n");
  printf("\tArrow Keys        Control the directions\n");
  printf("\tq                 Quits the game at any time\n");
  printf("\n\n");

  printf("Usage:\n");
  printf("\tifitron [-h]\n");
  printf("Commandline arguments:\n\n");
  printf("\t-h, --help         Displays the help guidelines.\n");
  printf("\n");
}


// The main loop of the game.

 int main (int argc, char* argv[])
{
 
	if (argc > 1) args_handle (argc, argv);

	engine_init ();
	engine_show_main_menu ();
	ifitron_init ();

	while (TRUE)
	{
	  if (tron.is_alive == FALSE) ifitron_game_over ();
	 
	  engine_get_game_input();
	  
	  player_update();
	  
	  player_increase_size (1);
	  player_increase_score (game.level);
	  
	  if (tron.score % 50 == 0 && game.level < 9) game.level++;

	  if (player_hit_self() == TRUE  || player_hit_borders() == TRUE)
	    tron.is_alive = FALSE;

	  engine_show_screen ();
	}
	return 0;
}
