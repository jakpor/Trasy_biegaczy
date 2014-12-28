#include "graph.h"
#include <string>

Graph::Graph()
{
    lista_wierzcholkow = NULL ;
    macierz_przyleglosci = NULL ;
    lista_krawedzi = NULL ;
    liczba_wierzcholkow = 0;
    liczba_krawedzi = 0;
    szerokosc_grafu = 0;
    wysokosc_grafu = 0;
}

Graph::~Graph()
{
    delete lista_wierzcholkow;
    //fajne usuwanie
    for (int i = 0; i<(liczba_wierzcholkow); i++){
        delete macierz_przyleglosci[i];
    }
    delete macierz_przyleglosci;

    delete lista_krawedzi;
    //ustawiam wszystkie wartości w razie gdyby destrutor był wywoływany ręcznie
    lista_wierzcholkow = NULL ;
    macierz_przyleglosci = NULL ;
    lista_krawedzi = NULL ;
    liczba_wierzcholkow = 0;
    liczba_krawedzi = 0;
    szerokosc_grafu = 0;
    wysokosc_grafu = 0;

}

Graph Graph::copy_graph(){
    Graph a;
    a.liczba_krawedzi = this->liczba_krawedzi;
    a.liczba_wierzcholkow = this->liczba_wierzcholkow;
    a.szerokosc_grafu = this->szerokosc_grafu;
    a.wysokosc_grafu = this->wysokosc_grafu;


    QPoint * wierz = new QPoint[(a.liczba_wierzcholkow)];
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        wierz[i] = this->lista_wierzcholkow[i];
    }
    a.lista_wierzcholkow = wierz;

    QLine * kraw = new QLine[a.liczba_krawedzi];
    for (int i=0 ; i<a.liczba_krawedzi; i++){
        kraw[i] = this->lista_krawedzi[i];
    }

    a.lista_krawedzi = kraw;

    unsigned int ** macierz = new unsigned int * [a.liczba_wierzcholkow];
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        macierz[i] = new unsigned int [(a.liczba_wierzcholkow)];
    }

    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        for (int j=0 ; j<a.liczba_wierzcholkow; j++){
            macierz[i][j] = this->macierz_przyleglosci[i][j];
        }
    }

    a.macierz_przyleglosci = macierz;

    return a;

}


Graph Graph::load_graph(string filename){
    Graph a;
    ifstream in;
    string name;

    //macierz przyleglosci

    name = filename +".txt";
    in.open(name.c_str());
    in>>a.liczba_wierzcholkow;

    unsigned int ** macierz = new unsigned int * [a.liczba_wierzcholkow];
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        macierz[i] = new unsigned int [(a.liczba_wierzcholkow)];
    }


    // Trzeba uwzględnić spacje i znaki nowej linii
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        for (int j=0 ; j<a.liczba_wierzcholkow; j++){
            in>>macierz[i][j];
        }
    }
    a.macierz_przyleglosci = macierz;

    in.close();

    //wierzcholki
    /*
    name = filename +'.xy';
    in.open(name.c_str());

    unsigned int ** macierz = new unsigned int * [a.liczba_wierzcholkow];
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        macierz[i] = new unsigned int [(a.liczba_wierzcholkow)];
    }

    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        for (int j=0 ; j<a.liczba_wierzcholkow; j++){
            inFile>>macierz[i][j];
        }
    }
    a.macierz_przyleglosci = macierz;

    outFile.close();
    */




}
