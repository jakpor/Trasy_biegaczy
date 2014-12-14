#include "graph.h"
#define NULL 0

using namespace std;
graph::graph()
{
    lista_wierzcholkow = NULL ;
    macierz_przyleglosci = NULL ;
    lista_krawedzi = NULL ;
}

graph::graph(int** wierzcholki,unsigned int**  przyleglosci,int**  krawedzie):
    lista_wierzcholkow(wierzcholki),
    macierz_przyleglosci(przyleglosci),
    lista_krawedzi(krawedzie)
{}

graph::~graph()
{

}

