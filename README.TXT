
Kandidatnr 223

1)Highscoreserver kompileres slik "gcc serv.c score.c" + portnr 4 siffer, kompiler highscore serveren f�r c filene kj�res.

2)Filen score.c tar av seg oppretting av "board", sortering, henting av spillere og minnefrig�ring, alle disse metodene besto testene til header fila "score.h"

3)Det er laget en makefil for selve spillet.

4)Highscoreserveren h�ndterer flere innkommende foresp�rsler fra flere spillere/terminaler

5) Terminalen vil vise ditt spiller navn og poeng med engang et spill er over, senere vil de 5 beste bli vist i "main screen" sortert fra h�yest til minst.

6)ctrl-c i terminalen vil vise en liste p� 25 personer bli vist sortert etter poeng fra h�yst til minst


Jeg ble ikke ferdig med multiplayeren delen, deler av den er kommentert bort og de evt metoder for styre motstanderen fungerer ikke.
I fila score.c ligger alle metodene som highscoreserven benytter seg. Strukturen og metodene og utformet etter testene fra fila score.h. Jeg bestemte meg for � mallokere kun
en gang i i programmet, slik at det skulle opprettes en kontinuerlig minne som senere kunne enkelt frigj�res, dette ble gjort i fila score.c  
