# Modelarea și vizualizarea sistemelor neliniare. Mulțimea lui Mandelbrot #

Mulțimea lui Mandelbrot este un fractal care a devenit cunoscut în afara matematicii atât pentru estetica sa, cât și 
pentru structura complicată, definindu-se ca fiind mulțimea acelor puncte c din planul complex pentru care aplicând 
în mod repetat polinomul complex z2 + c (pornind de la z = 0) rezultatul rămâne în interiorul unui disc de rază finită.
*Mulțimea lui Mandelbrot* este un soft educațional performant ce facilitează o explorare amănunțită a acestui fractal.

## Control ##

Camera poate fi controlată folosind tastele 'W', 'A', 'S', 'D' pentru a se deplasa în direcțiile nord, vest, sud, est.
Tastele 'Q', 'E' pot fi folosite pentru a controla nivelul zoom-ului. Pe prima pagină a ferestrei de setări se află 
câmpurile 'x', 'y' și 'zoom' care afișează poziția curentă și care pot fi totodată modificate. Tot din această pagină
utilizatorul poate modifica precizia programului.

## Manager de teme ##

Programul cuprinde un manager de teme ce permite utilizatorului să își personalizeze fractalul. Aceasta poate să aleagă
o temă din cele implicite, să o modifice pe aceasta sau chiar să creeze una nouă. Pe langă culori, utilizatorul are
opțiunea să modifice si funcțiile trigonometrice aplicate unui punct, astfel aplicația oferind un grad ridicat de 
personalizare.

## Manager de locații ##

Utilizatorul poate să își gestioneze cu ușurință locațiile de interes salvându-le coordonata și nivelul de zoom.
Printr-o singură apăsare de buton acesta poate ajunge direct la structura studiată, nefiind nevoie să o caute iar.

## Capturi de ecran ##

Aplicația oferă posibilitatea de a salva capturi de ecran la rezoluție maximă, nefiind nevoie de programe externe.

## Descriere tehnică ##

Softul a fost realizat integral în C++ pentru a asigura o perfomanță cât mai ridicată. Am folosit modulul de grafici
și fereastră din cadrul bibliotecii *SFML*, iar interfața a fost realizată folosing biblioteca *ImGui*. Biblioteca 
*nlohmann-json* a fost necesară pentru a putea folosi formatul *json*. Aplicația se folosește de multithreading pentru
a împărți fereastra în mai multe fâșii verticale pe care se aplică, în paralel, un algoritm eficient de calculare a 
fractalului, astfel maximizând performanța. Randarea se face eficient, fiind nevoie de o singură operațiune de desenare
pentru tot fractalul. Codul este abstractizat si modularizat, întrucât am folosit programare orientată pe obiecte.

![overview](https://github.com/tMario2111/MandelbrotSet/blob/master/screenshots/overview.png)
![JuliaIsland](https://github.com/tMario2111/MandelbrotSet/blob/master/screenshots/JuliaIsland.png)
![Seahorse](https://github.com/tMario2111/MandelbrotSet/blob/master/screenshots/Seahorse.png)