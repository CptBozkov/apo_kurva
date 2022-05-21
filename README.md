# Achtung, die Kurve!

## Kód

Aplikace je rozdělená do více souborů, podle následujícího diagramu. O chod aplikace se starají 4 vlákna běžící současně. Data se mezi jednotlivými vlákny předávají pomocí struktury `data_passer`. 

![application_architecture](https://user-images.githubusercontent.com/28506955/169646234-d086118d-f98d-4d9c-8a69-25cbebce23b3.png)

#### Threads (červená)

###### gameLoop
- Hlavní herní smyčka. Zpracovává inputy z klávesnice, updateuje pozici hráčů, detekuje kolize a edituje aktivní pixel buffer. Pixel buffery jsou uloženy v `data_passer`, přičemž samotná hra a menu používají každá jiný.

###### draw
- Pokaždé když jí to gameLoop přikáže (pomocí změny `draw` v `data_passer` na true), vykresluje aktivní pixel buffer na displej. Po dokončení vykreslování nastaví `done_draw` v `data_passer` na true.

###### pause
- Pause je thread, který může být spuštěn pomocí nastavení proměnné `pause` v `data_passer` na true. Předem musí bý nastavena délka pauzy pomocí nastavení `pause_length`(v sekundách) v `data_passer`. Po vypršení této doby nastaví pause thread `pause` v `data_passer` na false.

###### keyboard
- Načítá vstupy z klávesnice a ukádá je do `data_passer`.

#### Player (fialová)
- Obstarává funkcionalitu hráče. Od jeho vytvoření (`createPlayer()`) a updatování - změny polohy (`updatePlayer()`) až po detekování kolizí (`detectCollision()`).

#### Utils (zelená)

###### colors
- Obsahuje funkce, které slouží k rychlému vytváření pixelů. Také jsou zde funkce na vytvoření bufferů s přednastavenýma barvama.

###### buffer
- Jsou v něm uloženy funkce na jendodušší editaci pixelů v pixel bufferech a také funkce na jejich vyčištění.

###### font
- Obsahuje funkce, které slouží k převedení charů a arrayů charů na text na displeji. Všechny funkce jsou zde v několika podobách pro modifikaci velikosti téhož fontu.

###### hardware
- Načítá data z otočných voličů, obstaráva rozsvěcení ledline.

###### draw_objects
- Obsahuje funkce které jsou používány k zapisování určitých 2D tvarů do pixel bufferů.

#### Funkce pro práci s přípravkem (modrá)

- **font_types** - definice typu použitého pro popis fontu
- **mzapo_parlcd** - funkce pro nízkoúrovňový přístup k LCD display
- **mzapo_phys** - funkce pro mapování fyzického rozsahu adres do aplikačního procesu
- **mzapo_regs** - definice bázových adres a registrů výukového návrhu MZ_APO

#### Konstanty, struktury (žlutá)
- Zde jsou uložené všechny konstanty a struktury pro celou aplikaci.



## Spuštění

Pro spuštění naší hry budete potřebovat přípravek MicroZed APO, ke kterému připojíte přes USB klávesnici. Hodí se k němu také mít připojený ethernetový kabel.

Více informací o tomto přípravku můžete zjistit např. [zde](https://cw.fel.cvut.cz/wiki/courses/b35apo/documentation/mz_apo/start), návod na jeho obsluhu je zdokumentovaný [zde](https://cw.fel.cvut.cz/wiki/courses/b35apo/documentation/mz_apo-howto/start).

#### Kompilace

Můžete využít náš přiložený `Makefile`. Pomocí příkazu `make send` odešlete data data na specifikovanou IP adresu přípravku, kde se následně program zkompiluje a spustí.

#### Uživatelský manuál ke hře

Ke stažení [zde](https://github.com/CptBozkov/apo_kurva/blob/00a1760de1186f372be8deeda98b31b10d059c45/USER_MANUAL.pdf).
