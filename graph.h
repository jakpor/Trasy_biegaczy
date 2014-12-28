#ifndef GRAPH_H
#define GRAPH_H

#include <QtGui>
#include <fstream>
#include <string>
#include <iostream>

using namespace std;
class Graph
{
public:
    Graph();
    ~Graph();
    unsigned int** macierz_przyleglosci;
    QPoint * lista_wierzcholkow;
    QLine * lista_krawedzi;
    int liczba_wierzcholkow;
    int liczba_krawedzi;
    int szerokosc_grafu;
    int wysokosc_grafu;
    Graph copy_graph();

    Graph load_graph(string filename);
};

#endif // GRAPH_H
