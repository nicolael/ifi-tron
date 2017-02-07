

#ifndef PLAYER_DEFINED
#define PLAYER_DEFINED

/**	The structure of each segmentation of the player. */
struct player_pieces_t
{
	/** The x position of this piece */
	int x;
	/** The y position of this piece */
	int y;
};


/** A simple enum to make the directions of the player easier to read.
 */
enum directions { UP = 0, LEFT, DOWN, RIGHT };


/**	Represent the snake itself.
 */
struct player_t
{
	/** Indicates if the player is alive */
	int is_alive;

	/** How many chars will the player move by frame */
	int	speed;

	/** The current size of the snake.pieces's body */
	int	size;

	/** The score :D */
	int score;

	/** Which direction shoud the player go */
	int direction;

	/** All pieces of the snake (including the head) */
	struct player_pieces_t* body;
};


/** The player himself */
extern struct player_t tron;
extern struct player_t tron2;

void player2_change_direction(int new_direction);
void player_change_direction (int new_direction);
void player_exit ();
int  player_hit_borders ();
int  player_hit_self ();
void player_increase_score (int add);
void player_increase_size (int size);
void player_init ();
void player_update ();


#endif
