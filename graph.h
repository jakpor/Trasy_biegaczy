#ifndef GRAPH_H
#define GRAPH_H

#include <QtGui>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include <ctime>
#include <cctype>


using namespace std;
class Graph
{
public:
    Graph();
    ~Graph();
    unsigned int** macierz_przyleglosci;
    int** macierz_betonu;
    int * lista_betonu; //1-beton, 0 - nie beton
    int** macierz_wysokosci;
    int * lista_wysokosci;
    int max_wysokosc;
    int min_wysokosc;
    QPoint * lista_wierzcholkow;
    QLine * lista_krawedzi;
    int liczba_wierzcholkow;
    int liczba_krawedzi;
    int szerokosc_grafu;
    int wysokosc_grafu;
    //13 zmiennych (jak dodajesz to tu dopisz, więcej - do konstuktorów)
    Graph copy_graph();
    void create_graph(string outFileName, int h, int w, int marginesX, int marginesY,
                         int szerokosc, int wysokosc, bool czy_kwadrat, int roznorodnosc,
                         bool pionowo, bool poziomo, bool skos1, bool skos2, int betonowosc, int profil, bool losowosc);
    unsigned int distance(int ax,int ay,int bx,int by);

    void load_graph(string filename);
    void save_graph(string name);
    void debug();
    void destroy(); //bo zwykłe destruktory są często niejawnie wywoływane
};

#endif // GRAPH_H
