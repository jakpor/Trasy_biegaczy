#include "generator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <vector>
#include "graph.h"
#include <QtGui>

using namespace std;

#define NIESKONCZONOSC 0
//#define MAX_ODLEGLOSC  100
//#define MAX_WIDTH      800
//#define MAX_HEIGHT     600


//można by stworzyć jakąś strukturę by skrócić ta linijkę (ale szkoda poprawiać kod pewnie)
generator::generator(string filename, int height, int width, int margX, int margY,
                     int oknoX, int oknoY, bool kwadrat, int rozn,
                     bool pion, bool poz, bool sk1, bool sk2):
    outFileName(filename), h(height), w(width), marginesX(margX), marginesY(margY),wysokosc_okna(oknoY), szerokosc_okna(oknoX),
    czy_kwadrat(kwadrat), roznorodnosc(rozn), pionowo(pion), poziomo(poz), skos1(sk1), skos2(sk2)
{
    ofstream out;
    string name;

    //etykietki wierzcholkow
    macierz_wierzcholkow = etykietki_wierzcholkow(h,w,0);

    // wypisz etykietki wierzcholkow
    //wypisz_macierz(w,h,macierz_wierzcholkow, 4);

    //wspolrzedne wierzcholkow
    /*
    marginesX = 50;
    marginesY = 20;
    wysokosc_okna = 600;
    szerokosc_okna = 800;
    roznorodnosc = 100;
    czy_kwadrat = true;
    */

    //funkcja generująca współrzedne
    wspolrzedne(marginesX,marginesY,wysokosc_okna, szerokosc_okna, roznorodnosc, czy_kwadrat);

    cout<< "X"<< endl;
    //wypisz_macierz(w,h,wspolrzedneX, 4);

    cout<< "Y"<< endl;
    //wypisz_macierz(w,h,wspolrzedneY, 4);

    // zapisanie współrzednych do pliku
    name = outFileName+".xy";
    out.open(name.c_str());
    out.width(4);
    l_wierzcholkow = w*h;
    out << l_wierzcholkow <<endl;

    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            out.width(4);
            out << wspolrzedneX[i][j];
            out.width(4);
            out << wspolrzedneY[i][j];
            out<< endl;
        }
    }

    out.close();

    // macierz przyleglosci
    /* wierzcholek moze laczyc sie tylko z wierzcholkami sasiednimi, czyli majac polozenie x, y kandydatami sa:
       x,   y,  numer
       x+1, y,  numer + w      - jeden w dol
       x+1, y+1 numer +w+1     - jeden w prawo i jeden w dol (przekatna)
       x,   y+1 numer+1        - jeden w prawo
       x+1, y-1 numer+w -1     -2nd przekatna  - nie dziala dla y==0, czyli numer%w==1

       (4 mozliwosci)
     w=3, h=4 (width, height)
       1   2   3
       4   5   6
       7   8   9
      10  11  12


     macierz ma wymiar (h*x) x (l_wierzcholkow) - calkiem spora sie robi, ale wypelniamy tylko g�rna pol�wk�
     wartosci - rzeczywiste odleglosci miedzy punktami (zeby cos sie dzialo)
    */

    //macierz_przyleglosci

    macierz_przyleglosci = new unsigned int *[l_wierzcholkow];
    for (int i = 0; i< l_wierzcholkow; i++){
        macierz_przyleglosci[i] = new unsigned int [l_wierzcholkow];
    }
    //zerowanie - ona w calosci idzie do pliku pozniej
    for (int i=0 ; i< l_wierzcholkow; i++){
        for(int j =0; j< l_wierzcholkow; j++){
            macierz_przyleglosci[i][j] = 0;
        }
    }


    for (int i=0 ; i<h; i++){
        for(int j=0; j<w; j++){
            int szansa = 4; //4;//rand() % 5+2; // ile krawedzi
            int ktora = -1;
            for(int m =0; m<szansa; m++){
                ktora = ktora+1; //ktora+1; //rand() % 4; //ktora krawedz
                switch (ktora){
                    case 0:
                        // jeden w prawo
                        if(poziomo){
                            if((((i)*w+j+1)< l_wierzcholkow)&&((j+1)<w)){
                                macierz_przyleglosci[i*w+j][i*w+(j+1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i][j+1], wspolrzedneY[i][j+1]);
                            }
                        }
                        break;
                    case 1:
                        //jeden w dol
                        if(pionowo){
                            if((((i+1)*w+j)< l_wierzcholkow)&&((i+1)<h)){
                                macierz_przyleglosci[i*w+j][(i+1)*w+(j)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j], wspolrzedneY[i+1][j]);
                            }
                        }
                        break;
                    case 2:
                        //przekatna na +
                        if(skos1){
                            if((((i+1)*w+j+1)<l_wierzcholkow)&&((j+1)<w)&&((i+1)<h)){
                                macierz_przyleglosci[i*w+j][(i+1)*w+ (j+1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j+1], wspolrzedneY[i+1][j+1]);
                            }
                        }
                        break;
                    case 3:
                        //przekatna na -
                        if (skos2){
                            if(((i*w+j+1)%w!=1)&&((i+1)*w+ (j-1)<l_wierzcholkow)){
                                macierz_przyleglosci[i*w+j][(i+1)*w+ (j-1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j-1], wspolrzedneY[i+1][j-1]);
                            }
                        }
                        break;
                    default:
                        break;
                }
                //cout<<i*w+j+1;
            }
        }

    }

    //Symetryczność macierzy!!!
    for (int i = 0;i<l_wierzcholkow;i++){
        for(int j=0; j<i; j++){
            macierz_przyleglosci[i][j] =  macierz_przyleglosci[j][i];
        }
    }

    cout<< "Macierz przyleglosci"<< endl;
    //wypisz_macierz(l_wierzcholkow,l_wierzcholkow,(int **)macierz_przyleglosci, 4);

    //zapis macierzy przyległości do pliku
    name = outFileName+".txt";
    out.open(name.c_str());

    for (int i=0 ; i<l_wierzcholkow; i++){
        for(int j =0; j<l_wierzcholkow; j++){
            out.width(4);
            out << macierz_przyleglosci[i][j];
        }
        out<< endl;
    }

    out.close();



    //lista krawedzi

    cout<< "Lista krawedzi"<< endl;
    //cout<<" X1  Y1  X2  Y2  WAGA NR1 NR2";
    //cout<< endl;

    name = outFileName+".kr";
    out.open(name.c_str());

    int licznik_krawedzi = 0;

    for (int i=0 ; i< l_wierzcholkow; i++){
        for(int j =i; j< l_wierzcholkow; j++){
            if(macierz_przyleglosci[i][j]!=0){
                licznik_krawedzi++;
            }
        }
    }
    out<<licznik_krawedzi<<endl; //pierwsza linijka pliku

    for (int i=0 ; i< l_wierzcholkow; i++){
        for(int j =i; j< l_wierzcholkow; j++){
            if(macierz_przyleglosci[i][j]!=0){
                //j to numer wierzcholka -1, ktory lezy na j%w wierszu i j - h*(j%w) kolumnie
                out.width(4);
                out<<wspolrzedneX[(i -(i%w))/w][i%w];
                out.width(4);
                out<<wspolrzedneY[(i -(i%w))/w][i%w];
                out.width(4);
                out<<wspolrzedneX[(j -(j%w))/w][j%w];
                out.width(4);
                out<<wspolrzedneY[(j -(j%w))/w][j%w];
                //out.width(4);
                //cout<<macierz_przyleglosci[i][j];    //waga
                //cout.width(4);
                //cout<<i;                           //numer i tego w
                //cout.width(4);
                //cout<<j; //numer polaczonego do niego
                out<< endl;
            }
        }
        //cout<< endl;
    }


}

generator::~generator()
{
    for (int i = 0; i<l_wierzcholkow; i++){
        delete macierz_przyleglosci[i];
    }
    delete macierz_przyleglosci;

    for (int i = 0; i<(h); i++){
        delete wspolrzedneX[i];
    }
    delete wspolrzedneX;
    for (int i = 0; i<(h); i++){
        delete wspolrzedneY[i];
    }
    delete wspolrzedneY;
    for (int i = 0; i<(h); i++){
        delete macierz_wierzcholkow[i];
    }
    delete macierz_wierzcholkow;
}

 void generator::wypisz_macierz(int w, int h, int **macierz, int size){
    //ostream out;
    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            //printf("%4d", macierz_wierzcholkow[i][j]);
            cout.width(size);
            cout << macierz[i][j];
        }
        cout<< endl;
    }

}

 void generator::wypisz_macierz_do_pliku(int w, int h, int **macierz, int size, string name){
    ofstream out;
    out.open(name.c_str());

    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            //printf("%4d", macierz_wierzcholkow[i][j]);
            out.width(size);
            out << macierz[i][j];
        }
        out<< endl;
    }

    out.close();

}

unsigned int generator :: distance(int ax,int ay,int bx,int by){
    return (unsigned int)(sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by)));
}

int** generator ::etykietki_wierzcholkow(int h, int w, int start){
    int ** etwierz = new int *[h];
    for (int i = 0; i<h; i++){
        etwierz[i] = new int [w];
    }

    int numer_wierzcholka = start;
    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            etwierz[i][j] = numer_wierzcholka;
            numer_wierzcholka++;
        }
    }
    return etwierz;
}
void generator::wspolrzedne(int x0, int y0, int wysokosc, int szerokosc, int max_odleglosc, bool stale_odleglosci){
    srand(time(NULL));

    wspolrzedneX = new int *[h];
    for (int i = 0; i<h; i++){
        wspolrzedneX[i] = new int [w];
    }
    wspolrzedneY = new int *[h];
    for (int i = 0; i<h; i++){
        wspolrzedneY[i] = new int [w];
    }

    //poczatek ukladu wspolrzendych

    wspolrzedneX[0][0] = x0;
    wspolrzedneY[0][0] = y0;

    if(!stale_odleglosci){
        //krawedzie prostokata
        for (int i=1 ; i<h; i++){
            wspolrzedneX[i][0] = wspolrzedneX[i-1][0];       // X zostaje taki sam
            wspolrzedneY[i][0] = wspolrzedneY[i-1][0] + rand() % max_odleglosc +1;    //do Y sie cos dodaje
        }
        for (int j=1 ; j<w; j++){
            wspolrzedneY[0][j] = wspolrzedneY[0][j-1]; //Y bez zmian
            wspolrzedneX[0][j] = wspolrzedneX[0][j-1] + rand() % max_odleglosc +1; // X rosnie
        }
        //skalowanie
        for (int i=1 ; i<h; i++){
            wspolrzedneY[i][0] = y0 + (wspolrzedneY[i][0]-y0) * (wysokosc-2*y0) / wspolrzedneY[h-1][0] ;    //do Y sie cos dodaje
        }
        for (int j=1 ; j<w; j++){
            wspolrzedneX[0][j] = x0+ (wspolrzedneX[0][j]-x0)* (szerokosc-2*x0) / wspolrzedneX[0][w-1] ; // X rosnie
        }

    }
    else{
        //kwadrat - pseudo kwadrat...
        for (int i=1 ; i<h; i++){
            wspolrzedneX[i][0] = wspolrzedneX[i-1][0];       // X zostaje taki sam
            wspolrzedneY[i][0] = wspolrzedneY[i-1][0] + (wysokosc-2*y0)/h;    //do Y sie cos dodaje
        }
        for (int j=1 ; j<w; j++){
            wspolrzedneY[0][j] = wspolrzedneY[0][j-1]; //Y bez zmian
            wspolrzedneX[0][j] = wspolrzedneX[0][j-1] + (szerokosc-2*y0)/w; // X rosnie
        }
    }



    for (int i=1 ; i<h; i++){
        for(int j =1; j<w; j++){
            wspolrzedneX[i][j] = wspolrzedneX[0][j];
            wspolrzedneY[i][j] = wspolrzedneY[i][0];
        }
    }
}


Graph generator::create_graph(){
    Graph a;

    unsigned int** macierz;
    macierz = new unsigned int *[l_wierzcholkow];
    for (int i = 0; i<(l_wierzcholkow); i++){
        macierz[i] = new unsigned int [l_wierzcholkow];
    }
    for (int i=0 ; i<(l_wierzcholkow); i++){
        for(int j =0; j<(l_wierzcholkow); j++){
            macierz[i][j] = macierz_przyleglosci[i][j];
        }
    }

    a.macierz_przyleglosci = macierz;

    //lista krawedzi
    int licznik_krawedzi = 0;

    for (int i=0 ; i<(l_wierzcholkow); i++){
        for(int j =i; j<(l_wierzcholkow); j++){
            if(macierz_przyleglosci[i][j]!=0){
                licznik_krawedzi++;
            }
        }
    }

    a.liczba_krawedzi = licznik_krawedzi;
    a.liczba_wierzcholkow = l_wierzcholkow;
    a.szerokosc_grafu = w;
    a.wysokosc_grafu = h;

    QLine *lista = new QLine[a.liczba_krawedzi];

    int u = 0;
    for (int i=0 ; i<(l_wierzcholkow); i++){
        for(int j = i; j<(l_wierzcholkow); j++){
            if(macierz_przyleglosci[i][j]!=0){
                //j to numer wierzcholka -1, ktory lezy na j%w wierszu i j - h*(j%w) kolumnie
                lista[u] = QLine((int)wspolrzedneX[(i -(i%w))/w][i%w],(int)wspolrzedneY[(i -(i%w))/w][i%w],(int)wspolrzedneX[(j -(j%w))/w][j%w],(int)wspolrzedneY[(j -(j%w))/w][j%w]);
                u++;
            }
        }
    }
    a.lista_krawedzi = lista;

    //lista wierzchołków
    QPoint *wierz = new QPoint[l_wierzcholkow];

    u = 0;
    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            wierz[u] = QPoint(wspolrzedneX[i][j],wspolrzedneY[i][j]);
            u++;
        }
    }
    a.lista_wierzcholkow = wierz;

    return a;
}
