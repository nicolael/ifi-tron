all: game

game:
	gcc ifitron.c engine.c player.c -lncurses
debug:
	gcc ifitron.c engine.c player.c -lncurses -g