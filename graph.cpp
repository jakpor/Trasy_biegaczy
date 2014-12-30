#include "graph.h"
#include <string>
#include <ctime>
#include <cctype>
#include <fstream>

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
//    delete lista_wierzcholkow;
//    //fajne usuwanie
//    for (int i = 0; i<(liczba_wierzcholkow); i++){
//        delete macierz_przyleglosci[i];
//    }
//    delete macierz_przyleglosci;

//    delete lista_krawedzi;
//    //ustawiam wszystkie wartości w razie gdyby destrutor był wywoływany ręcznie
//    lista_wierzcholkow = NULL ;
//    macierz_przyleglosci = NULL ;
//    lista_krawedzi = NULL ;
//    liczba_wierzcholkow = 0;
//    liczba_krawedzi = 0;
//    szerokosc_grafu = 0;
//    wysokosc_grafu = 0;

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



    return a;
}

Graph Graph::create_graph(string outFileName, int h, int w, int marginesX, int marginesY,
                     int szerokosc, int wysokosc, bool czy_kwadrat, int roznorodnosc,
                     bool pionowo, bool poziomo, bool skos1, bool skos2, int betonowosc, int profil)
{
    ofstream out;
    string name;
    int l_wierzcholkow = h*w; //ile wszystkich

    //int** macierz_wierzcholkow;
    int** wspolrzedneX;
    int** wspolrzedneY;
    unsigned int** macierz_przyleglosci;


    //etykietki wierzcholkow
    //macierz_wierzcholkow = etykietki_wierzcholkow(h,w,0);

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

    wspolrzedneX[0][0] = marginesX;
    wspolrzedneY[0][0] = marginesY;

    if(!czy_kwadrat){
        //krawedzie prostokata
        for (int i=1 ; i<h; i++){
            wspolrzedneX[i][0] = wspolrzedneX[i-1][0];       // X zostaje taki sam
            wspolrzedneY[i][0] = wspolrzedneY[i-1][0] + rand() % roznorodnosc +1;    //do Y sie cos dodaje
        }
        for (int j=1 ; j<w; j++){
            wspolrzedneY[0][j] = wspolrzedneY[0][j-1]; //Y bez zmian
            wspolrzedneX[0][j] = wspolrzedneX[0][j-1] + rand() % roznorodnosc +1; // X rosnie
        }
        //skalowanie
        for (int i=1 ; i<h; i++){
            wspolrzedneY[i][0] = marginesY + (wspolrzedneY[i][0]-marginesY) * (wysokosc-2*marginesY) / wspolrzedneY[h-1][0] ;    //do Y sie cos dodaje
        }
        for (int j=1 ; j<w; j++){
            wspolrzedneX[0][j] = marginesX+ (wspolrzedneX[0][j]-marginesX)* (szerokosc-2*marginesX) / wspolrzedneX[0][w-1] ; // X rosnie
        }

    }
    else{
        //kwadrat - pseudo kwadrat...
        for (int i=1 ; i<h; i++){
            wspolrzedneX[i][0] = wspolrzedneX[i-1][0];       // X zostaje taki sam
            wspolrzedneY[i][0] = wspolrzedneY[i-1][0] + (wysokosc-2*marginesY)/h;    //do Y sie cos dodaje
        }
        for (int j=1 ; j<w; j++){
            wspolrzedneY[0][j] = wspolrzedneY[0][j-1]; //Y bez zmian
            wspolrzedneX[0][j] = wspolrzedneX[0][j-1] + (szerokosc-2*marginesX)/w; // X rosnie
        }
    }



    for (int i=1 ; i<h; i++){
        for(int j =1; j<w; j++){
            wspolrzedneX[i][j] = wspolrzedneX[0][j];
            wspolrzedneY[i][j] = wspolrzedneY[i][0];
        }
    }

    //cout<< "X"<< endl;
    //wypisz_macierz(w,h,wspolrzedneX, 4);

    //cout<< "Y"<< endl;
    //wypisz_macierz(w,h,wspolrzedneY, 4);

    // zapisanie współrzednych do pliku
    name = outFileName+".xy";
    out.open(name.c_str());
    out.width(4);

    l_wierzcholkow = w*h;
    QPoint *wierz = new QPoint[l_wierzcholkow];//do klasy Graph
    int u = 0;

    out << l_wierzcholkow <<endl;

    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            out.width(4);
            out << wspolrzedneX[i][j];
            out.width(4);
            out << wspolrzedneY[i][j];
            out<< endl;
            wierz[u] = QPoint(wspolrzedneX[i][j],wspolrzedneY[i][j]);
            u++; //u to po prostu iterator
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

    //cout<< "Lista krawedzi"<< endl;

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

    QLine *lista = new QLine[licznik_krawedzi];

    int x1, x2, y1, y2;
    u = 0;

    for (int i=0 ; i< l_wierzcholkow; i++){
        for(int j =i; j< l_wierzcholkow; j++){
            if(macierz_przyleglosci[i][j]!=0){
                //j to numer wierzcholka -1, ktory lezy na j%w wierszu i j - h*(j%w) kolumnie
                x1 = wspolrzedneX[(i -(i%w))/w][i%w];
                y1 = wspolrzedneY[(i -(i%w))/w][i%w];
                x2 = wspolrzedneX[(j -(j%w))/w][j%w];
                y2 = wspolrzedneY[(j -(j%w))/w][j%w];

                out.width(4);
                out<<x1;
                out.width(4);
                out<<y1;
                out.width(4);
                out<<x2;
                out.width(4);
                out<<y2;
                //out.width(4);
                //cout<<macierz_przyleglosci[i][j];    //waga
                //cout.width(4);
                //cout<<i;                           //numer i tego w
                //cout.width(4);
                //cout<<j; //numer polaczonego do niego
                out<< endl;

                lista[u] = QLine(x1,y1,x2,y2);
                u++;
            }
        }
    }

    //betonowosc wersja beta
//    macierz_betonu = new unsigned int *[l_wierzcholkow];
//    for (int i = 0; i< l_wierzcholkow; i++){
//        macierz_betonu[i] = new unsigned int [l_wierzcholkow];
//    }

//    if(betonowosc == 0){
//        for (int i=0 ; i<l_wierzcholkow; i++){
//            for(int j =0; j<l_wierzcholkow; j++){
//                if(macierz_przyleglosci!=0){
//                    macierz_betonu[i][j] = rand()% 2; //jest, albo nie jest
//                }
//            }
//        }
//    }

    //od teraz tworzymy Graph

    Graph a;

    a.macierz_przyleglosci = macierz_przyleglosci;

    a.liczba_krawedzi = licznik_krawedzi;
    a.liczba_wierzcholkow = l_wierzcholkow;
    a.szerokosc_grafu = w;
    a.wysokosc_grafu = h;
    a.lista_krawedzi = lista;
    a.lista_wierzcholkow = wierz;
    //cout<<"Stworzono Graph"<<endl;
    //cout<<licznik_krawedzi<<endl;
//    for (int k = 0; k<licznik_krawedzi; k++ ){
//        cout<<a.lista_krawedzi[k].x1()<<" "<<a.lista_krawedzi[k].y1()<<" "<<a.lista_krawedzi[k].x2()<<" "<<a.lista_krawedzi[k].y2()<<" "<<endl;
//    }
    return a;
}


unsigned int Graph :: distance(int ax,int ay,int bx,int by){
    return (unsigned int)(sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by)));
}
