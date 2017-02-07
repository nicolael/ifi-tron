#include <stdlib.h>
#include <strings.h>

#include "engine.h"
#include "player.h"

struct player_t	tron;
struct player_t tron2;


void player2_change_direction(int new_direction ){

	
	if      ((new_direction == UP) && (tron2.direction != DOWN))
		tron2.direction = UP;

	else if ((new_direction == LEFT) && (tron2.direction != RIGHT))
		tron2.direction = LEFT;

	else if ((new_direction == DOWN) && (tron2.direction != UP))
		tron2.direction = DOWN;

	else if ((new_direction == RIGHT) && (tron2.direction != LEFT))
		tron2.direction = RIGHT;

}

/* Changes the tron direction based on the input received */
void player_change_direction (int new_direction)
{
	if      ((new_direction == UP) && (tron.direction != DOWN))
		tron.direction = UP;

	else if ((new_direction == LEFT) && (tron.direction != RIGHT))
		tron.direction = LEFT;

	else if ((new_direction == DOWN) && (tron.direction != UP))
		tron.direction = DOWN;

	else if ((new_direction == RIGHT) && (tron.direction != LEFT))
		tron.direction = RIGHT;

}

/* Free memory associated with the player */
void player_exit ()
{
	if (tron.body != NULL)
	{
	  // Clean up - memory may be reused later
	  bzero (tron.body, sizeof(tron.size * sizeof (struct player_pieces_t)));
	  // Free
	  free (tron.body);
	  tron.body = NULL;
	}
	/*
	if (tron2.body != NULL)
	{
	  // Clean up - memory may be reused later
	  bzero (tron2.body, sizeof(tron2.size * sizeof (struct player_pieces_t)));
	  // Free
	  free (tron2.body);
	  tron2.body = NULL;
	}
	*/
}


/* Checks collision between the player and the walls */
int player_hit_borders ()
{
	if ((tron.body[0].x < 1) || (tron.body[0].x > (screen.width-2)) ||
	    (tron.body[0].y < 2) || (tron.body[0].y > (screen.height-2)))
	{
		return TRUE;
	}
	/*
	if ((tron2.body[0].x < 1) || (tron2.body[0].x > (screen.width-2)) ||
	    (tron2.body[0].y < 2) || (tron2.body[0].y > (screen.height-2)))
	{
		return TRUE;
	}
	*/
	return FALSE;
}


/**	Checks if the tron have collided with itself */
int player_hit_self ()
{
	int i;
	for (i = (tron.size-1); i > 1; i--)
	{
		if ((tron.body[0].x == tron.body[i].x) &&
		    (tron.body[0].y == tron.body[i].y))
		{
			return TRUE;
		}
	}
	/*
	int j;
	for (j = (tron2.size-1); j > 1; j--)
	{
		if ((tron2.body[0].x == tron2.body[j].x) &&
		    (tron2.body[0].y == tron2.body[j].y))
		{
			return TRUE;
		}
	}
	*/
	return FALSE;
}


/**	Simply increases the score by the value sent as the parameter.
 */
void player_increase_score (int add)
{
	tron.score += add;
	//tron2.score += add;
}




/**	Increases the tron size.
 *
 * 	Here we have the core function of this file. Each time the tron
 * 	increases its size, we expand the array that represents its body. */

void player_increase_size (int size)
{
	int piece_size = sizeof (struct player_pieces_t);

	tron.size += size;

	tron.body = realloc (tron.body, (tron.size * piece_size));
	if (tron.body == NULL)
		ifitron_abort ("Memory Error!\n");
	
	/*
	int pieze_size = sizeof (struct player_pieces_t);

	tron2.size += size;

	tron2.body = realloc (tron2.body, (tron2.size * pieze_size));
	if (tron2.body == NULL)
		ifitron_abort ("Memory Error!\n");
	*/
}


/**	Starts the player-related variables. */
void player_init ()
{
	tron.body = NULL;

	tron.is_alive  = TRUE;
	tron.speed     = 1;
	tron.score     = 0;
	tron.direction = RIGHT;
	tron.size      = 1;

	tron.body = malloc (tron.size * sizeof (struct player_pieces_t));
	if (tron.body == NULL) ifitron_abort ("Memory Error!\n");

	int i;
	for (i = 0; i < tron.size; i++)
	{
		tron.body[i].x = screen.width/2;
		tron.body[i].y = screen.height/2;
	}

	/*
	tron2.body = NULL;

	tron2.is_alive  = TRUE;
	tron2.speed     = 1;
	tron2.score     = 0;
	tron2.direction = LEFT;
	tron2.size      = 1;

	tron2.body = malloc (tron2.size * sizeof (struct player_pieces_t));
	if (tron2.body == NULL) ifitron_abort ("Memory Error!\n");

	int j;
	for (j = 0; j < tron2.size; j++)
	{
		tron2.body[j].x = screen.width/2;
		tron2.body[j].y = screen.height/2;
	}
	*/

}

/**	Updates the player position, one piece at a time
  */
void player_update ()
{
// body
	int i;
	for (i = (tron.size-1); i > 0; i--)
	{
		tron.body[i].x = tron.body[i-1].x;
		tron.body[i].y = tron.body[i-1].y;
	}

// head
	if      (tron.direction == UP)    tron.body[0].y -= 1;
	else if (tron.direction == LEFT)  tron.body[0].x -= 1;
	else if (tron.direction == DOWN)  tron.body[0].y += 1;
	else if (tron.direction == RIGHT) tron.body[0].x += 1;
	

	/*
	int j;
	for (j = (tron2.size-1); j > 0; j--)
	{
		tron2.body[j].x = tron2.body[j-1].x;
		tron2.body[j].y = tron2.body[j-1].y;
	}

	if      (tron2.direction == UP)    tron2.body[0].y -= 1;
	else if (tron2.direction == LEFT)  tron2.body[0].x -= 1;
	else if (tron2.direction == DOWN)  tron2.body[0].y += 1;
	else if (tron2.direction == RIGHT) tron2.body[0].x += 1;
	*/

}
