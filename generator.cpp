#include "generator.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <vector>

using namespace std;

#define NIESKONCZONOSC 0
#define HEIGHT         4
#define WIDTH          5
#define MAX_ODLEGLOSC  100
#define MAX_WIDTH      800
#define MAX_HEIGHT     600

generator::generator()
{
    ofstream outFile;

    //string outFileName;

    outFileName="out.txt";

    outFile.open(outFileName.c_str());

    h = HEIGHT;
    w = WIDTH;

    //etykietki wierzcholkow
    macierz_wierzcholkow = new int *[h];
    for (int i = 0; i<h; i++){
        macierz_wierzcholkow[i] = new int [w];
    }

    int numer_wierzcholka = 0;
    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            macierz_wierzcholkow[i][j] = numer_wierzcholka;
            numer_wierzcholka++;
        }
    }


    // wypisz etykietki wierzcholkow
    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            //printf("%4d", macierz_wierzcholkow[i][j]);
            cout.width(4);
            cout << macierz_wierzcholkow[i][j];
        }
        cout<< endl;
    }

    //wypisz_macierz(w,h,&macierz_wierzcholkow[0][0]);

    //wspolrzedne wierzcholkow

    srand(time(NULL));

    //wspolrzedneX = new int [h][w];
    //wspolrzedneY = new int [h][w];

    //poczatek ukladu wspolrzendych
    int x0 =0;
    int y0 = 0;
    wspolrzedneX[0][0] = x0;
    wspolrzedneY[0][0] = y0;

    //krawedzie prostokata
    for (int i=1 ; i<h; i++){
        wspolrzedneX[i][0] = wspolrzedneX[i-1][0];       // X zostaje taki sam
        wspolrzedneY[i][0] = wspolrzedneY[i-1][0] + rand() % MAX_ODLEGLOSC +1;    //do Y sie cos dodaje
    }

    for (int j=1 ; j<w; j++){
        wspolrzedneY[0][j] = wspolrzedneY[0][j-1]; //Y bez zmian
        wspolrzedneX[0][j] = wspolrzedneX[0][j-1] + rand() % MAX_ODLEGLOSC +1; // X rosnie
    }

    for (int i=1 ; i<h; i++){
        for(int j =1; j<w; j++){
            wspolrzedneX[i][j] = wspolrzedneX[0][j];
            wspolrzedneY[i][j] = wspolrzedneY[i][0];
        }
    }

    cout<< "X"<< endl;
    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            cout.width(4);
            cout << wspolrzedneX[i][j];
        }
        cout<< endl;
    }

    cout<< "Y"<< endl;
    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            cout.width(4);
            cout << wspolrzedneY[i][j];
        }
        cout<< endl;
    }

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


     macierz ma wymiar (h*x) x (h*w) - calkiem spora sie robi, ale wypelniamy tylko g�rna pol�wk�
     wartosci - rzeczywiste odleglosci miedzy punktami (zeby cos sie dzialo)
    */

    //macierz_przyleglosci = new unsigned int[h*w][h*w];


    //zerowanie - ona w calosci idzie do pliku pozniej
    for (int i=0 ; i<(h*w); i++){
        for(int j =0; j<(h*w); j++){
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
                        if((((i)*w+j+1)<(w*h))&&((j+1)<w)){
                            macierz_przyleglosci[i*w+j][i*w+(j+1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i][j+1], wspolrzedneY[i][j+1]);
                        }
                        break;
                    case 1:
                        //jeden w dol
                        if((((i+1)*w+j)<(w*h))&&((i+1)<h)){
                            macierz_przyleglosci[i*w+j][(i+1)*w+(j)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j], wspolrzedneY[i+1][j]);
                        }
                        break;
                    case 2:
                        //przekatna na +
                        if((((i+1)*w+j+1)<(w*h))&&((j+1)<w)&&((i+1)<h)){
                            macierz_przyleglosci[i*w+j][(i+1)*w+ (j+1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j+1], wspolrzedneY[i+1][j+1]);
                        }
                        break;
                    case 3:
                        //przekatna na -
                        if(((i*w+j+1)%w!=1)&&((i+1)*w+ (j-1)<h*w)){
                            macierz_przyleglosci[i*w+j][(i+1)*w+ (j-1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j-1], wspolrzedneY[i+1][j-1]);
                        }
                        break;
                    default:
                        break;
                }
                //cout<<i*w+j+1;
            }
        }

    }

    cout<< "Macierz przyleglosci"<< endl;
    for (int i=0 ; i<(h*w); i++){
        cout.width(5);
        cout<< i+1;
        for(int j =0; j<(h*w); j++){
            cout.width(5);
            cout << macierz_przyleglosci[i][j];
        }
        cout<< endl;
    }

    //plik
    outFile<< h*w<< endl;
    for (int i=0 ; i<(h*w); i++){
        for(int j =0; j<(h*w); j++){
            outFile.width(5);
            outFile << macierz_przyleglosci[i][j];
        }
        outFile<< endl;
    }

    outFile.close();

    //lista krawedzi

    cout<< "Lista krawedzi"<< endl;
    //opis :)
    cout<<" X1  Y1  X2  Y2  WAGA NR1 NR2";
    cout<< endl;


    for (int i=0 ; i<(h*w); i++){
        for(int j =0; j<(h*w); j++){
            if(macierz_przyleglosci[i][j]!=0){
                //j to numer wierzcholka -1, ktory lezy na j%w wierszu i j - h*(j%w) kolumnie
                cout.width(4);
                cout<<wspolrzedneX[(i -(i%w))/w][i%w];
                cout.width(4);
                cout<<wspolrzedneY[(i -(i%w))/w][i%w];
                cout.width(4);
                cout<<wspolrzedneX[(j -(j%w))/w][j%w];
                cout.width(4);
                cout<<wspolrzedneY[(j -(j%w))/w][j%w];
                cout.width(4);
                cout<<macierz_przyleglosci[i][j];    //waga
                cout.width(4);
                cout<<i;                           //numer i tego w
                cout.width(4);
                cout<<j; //numer polaczonego do niego
                cout<< endl;
            }
        }
        //cout<< endl;
    }



    // droga w grafie

    int start = 0;
    int meta = 5;

    if (start>meta){
        int temp = start;
        start = meta;
        meta = temp;
    }

    vector <int> trasa;
    trasa.push_back(start);

    while(trasa[trasa.size()-1]!=meta){
        for (int i=(h*w) ; i>0; i--){
            if(macierz_przyleglosci[trasa[trasa.size()-1]][i]!=0){
                if(i<=meta){
                    trasa.push_back(i);
                    //cout<<trasa[trasa.size()-1];
                    i = 0;

                }
            }
        }
    }

    //wypisz trase
        for(unsigned int i = 0; i < trasa.size(); i++ )
        {
            cout.width(4);
            cout << trasa[ i ];
        }

    /*
    int i =0;
    string dane;
    while (inFile){
        inFile>>dane;
        if ((dane[0] == '2')||!(dane.find(':') == std::string::npos)){
             dane = "";
        }
        if (!(dane.find('/n') == std::string::npos)){
            outFile<<dane<<endl;
        }
        else
            outFile<<dane<<' ';

        i++;
        dane = "";
    }

    outFile.close();
    */


}

generator::~generator()
{

}

//zapomnia�� jak pisa� funkcje na strumieniach
 void generator::wypisz_macierz(int w, int h, int *macierz){
    //ostream out;
    for (int i=0 ; i<h; i++){
        for(int j =0; j<w; j++){
            //printf("%4d", macierz_wierzcholkow[i][j]);
            cout.width(4);
            cout << macierz[i*w + j];
        }
        cout<< endl;
    }
}

unsigned int generator :: distance(int ax,int ay,int bx,int by){
    return (unsigned int)(sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by)));
}
