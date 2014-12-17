#ifndef GENERATOR_H
#define GENERATOR_H
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <vector>
#include "graph.h"
using namespace std;

class generator
{
private:
    string outFileName;
    int h; //ile wierzcholkow w pionie
    int w;  //ile wierzcholkow poziomo

    int** macierz_wierzcholkow;
    int** wspolrzedneX;
    int** wspolrzedneY;
    unsigned int** macierz_przyleglosci;

    int marginesX;
    int marginesY;
    int wysokosc_okna;
    int szerokosc_okna;

    bool czy_kwadrat;   //czy powstały graf ma być kwadratem
    int roznorodnosc; //różnrodność długości krawędzi

    bool poziomo;
    bool pionowo;
    bool skos1;
    bool skos2;

    int **etykietki_wierzcholkow(int h, int w, int start);
    void wspolrzedne(int x0, int y0, int wysokosc, int szerokosc, int max_odleglosc, bool stale_odleglosci);
    void wypisz_macierz(int w, int h, int ** macierz, int size);
    void wypisz_macierz_do_pliku(int w, int h, int ** macierz, int size, string name);
    unsigned int distance(int ax,int ay,int bx,int by);

public:
    generator(string filename, int height, int width, int margX, int margY,
               int oknoX, int oknoY, bool kwadrat, int rozn,
               bool pion, bool poz, bool sk1, bool sk2);
    ~generator();
    Graph create_graph();
};

#endif // GENERATOR_H
