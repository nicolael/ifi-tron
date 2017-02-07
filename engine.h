#ifndef ENGINE_DEFINED
#define ENGINE_DEFINED

#define TRUE  1
#define FALSE 0

/** Global definitions related to the game screen */
struct screen_t
{
	/** The fixed width of the game area */
	int width;
	/** The fixed height of the game area */
	int height;
	/** The time (in microsseconds) that the game */
	int delay;
};


/* Global definitions related to the game in general. */
struct game_t
{
	int level; /* How fast to move + score */
                   // more?
};


/** The global screen structure */
extern struct screen_t screen;

/** The global game structure */
extern struct game_t game;


void draw_background ();
void draw_borders ();
void draw_player ();
void draw_score ();

void engine_exit ();
void engine_init ();
void engine_show_game_over ();
void engine_show_main_menu ();
void engine_show_pause ();
void engine_show_screen ();

int  get_main_menu_input (int* current_option);
void engine_get_game_input ();
void get_pause_input ();

void ifitron_abort (char* error_msg);
void ifitron_exit ();
void ifitron_game_over ();
void ifitron_init ();



#endif
