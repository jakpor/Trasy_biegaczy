#ifndef TRASA
#define TRASA

#include "graph.h"
#include <vector>

#define INF 0
enum kryterium{distances=1, attractiveness=2, profile=3};
/************ Clasa rozwiązania***********/
class Trasa{
/************ Pola ************/

//lista wynik;
// zbior krawędzi dołączonych do rozwiązania
int wierzcholek_poczatkowy; //czy będzie to zrobione w tej postaci??
int wierzcholek_koncowy;
int f_distance;
// ocena trasy wg poszczególnych kryteriów
int f_attractiveness ;
int f_profile;
int funkcja_celu;
// ocena wg kryteriów zbiorowych
//vector<??> wykluczenia // krótko, średnio i długoterminowe

// enum umożliwiający słowne wybieranie ocenianego
// kryterium w metodach
public:
/************ Metody ************/
Trasa();
Trasa(int start, int end);
~Trasa();
// tworzenie pustego rozwiązania
    //trasa(wierzcholek);
// tworzenie rozwiązania przechodzącego przez dany wierzchołek
    //trasa(trasa);
// nie wiem czy będzie potrzebna, można stworzyć coś o podobnej konstrukcji
// do wykorzystania przy implementacji wykluczeń
    //lista/trasa rozwizanie_poczatkowe(){
//generowanie rozwiązania począktkowego przy założeniu losowego rozwiązania
    //wierzcholek= rand();
    //trasa::trasa(wiercholek)

// inna wersja rozważa wywołanie tej funkcji z argumentem mówiącym
//czy generujemy randomową trasę, czy trasę minimalną

int dijkstra(int wierzcholek_poczatkowy, int wierzcholek_koncowy, Graph graf, kryterium type);
// uzywana w kontruktorze
 vector<int> nastepniki(int x, unsigned int** A, int n);
 int minimum(unsigned int * temp,unsigned int * perm, int size);
     //wyznaczenie nastepnikow wierzcholka x, n -rozmiar macierzy A


/****** Metody algorytmu *******/
    //lista/trasa skroc(kryterium, wykluczenie);
    //lista/trasa wydluz(kryterium, wkluczenie);
    //wykluczenie/ void ocena_trasy(kryterium);
// zmienia tablice wykluczeń lub zwraca wykluczenie, lub wywołuje
// bezposrednio metody skroc, wydluz
    //vector<lista> otoczenie (wykluczenie, rozmiar);
// wyznacza otoczenie rozwiązania, w kierunku którego chcemy
//podążać i wybrać z niego rozwiązanie
    //bool kryterium_stopu( argumenty do kryt stopu);
// do ustalenia dzialanie tej metody
    //wykluczenie wybor_wykluczenia(kryterium, ...); // ...
};




#endif // TRASA

