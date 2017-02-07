
Kandidatnr 223

1)Highscoreserver kompileres slik "gcc serv.c score.c" + portnr 4 siffer, kompiler highscore serveren før c filene kjøres.

2)Filen score.c tar av seg oppretting av "board", sortering, henting av spillere og minnefrigøring, alle disse metodene besto testene til header fila "score.h"

3)Det er laget en makefil for selve spillet.

4)Highscoreserveren håndterer flere innkommende forespørsler fra flere spillere/terminaler

5) Terminalen vil vise ditt spiller navn og poeng med engang et spill er over, senere vil de 5 beste bli vist i "main screen" sortert fra høyest til minst.

6)ctrl-c i terminalen vil vise en liste på 25 personer bli vist sortert etter poeng fra høyst til minst


Jeg ble ikke ferdig med multiplayeren delen, deler av den er kommentert bort og de evt metoder for styre motstanderen fungerer ikke.
I fila score.c ligger alle metodene som highscoreserven benytter seg. Strukturen og metodene og utformet etter testene fra fila score.h. Jeg bestemte meg for å mallokere kun
en gang i i programmet, slik at det skulle opprettes en kontinuerlig minne som senere kunne enkelt frigjøres, dette ble gjort i fila score.c  
