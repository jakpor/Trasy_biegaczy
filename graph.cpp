#include "graph.h"
#include <string>
#include <ctime>
#include <cctype>
#include <fstream>

Graph::Graph()
{
    //konstruktor na 13 zmiennych
    lista_wierzcholkow = NULL ;
    macierz_przyleglosci = NULL ;
    macierz_betonu = NULL ;
    macierz_wysokosci = NULL ;
    lista_betonu = NULL;
    lista_wysokosci = NULL;
    lista_krawedzi = NULL ;
    liczba_wierzcholkow = 0;
    liczba_krawedzi = 0;
    szerokosc_grafu = 0;
    wysokosc_grafu = 0;
    min_wysokosc = 0;
    max_wysokosc = 0;
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
    //13 zmiennych
    Graph a;
    a.liczba_krawedzi = this->liczba_krawedzi;
    a.liczba_wierzcholkow = this->liczba_wierzcholkow;
    a.szerokosc_grafu = this->szerokosc_grafu;
    a.wysokosc_grafu = this->wysokosc_grafu;
    a.min_wysokosc = this->min_wysokosc;
    a.max_wysokosc = this->max_wysokosc;

    //lista wierzcholkow
    QPoint * wierz = new QPoint[(a.liczba_wierzcholkow)];
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        wierz[i] = this->lista_wierzcholkow[i];
    }
    a.lista_wierzcholkow = wierz;

    //lista krawedzi
    QLine * kraw = new QLine[a.liczba_krawedzi];
    for (int i=0 ; i<a.liczba_krawedzi; i++){
        kraw[i] = this->lista_krawedzi[i];
    }
    a.lista_krawedzi = kraw;

    //macierz przyleglosci
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

    //macierz betonu
    int ** beton = new int * [a.liczba_wierzcholkow];
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        beton[i] = new int [(a.liczba_wierzcholkow)];
    }
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        for (int j=0 ; j<a.liczba_wierzcholkow; j++){
            beton[i][j] = this->macierz_betonu[i][j];
        }
    }
    a.macierz_betonu = beton;

    //macierz wysokosci
    int ** wysokosci = new int * [a.liczba_wierzcholkow];
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        wysokosci[i] = new int [(a.liczba_wierzcholkow)];
    }
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        for (int j=0 ; j<a.liczba_wierzcholkow; j++){
            wysokosci[i][j] = this->macierz_wysokosci[i][j];
        }
    }
    a.macierz_wysokosci = wysokosci;

    //lista betonu
    int * lista_b = new int [a.liczba_krawedzi];
    for (int i=0 ; i<a.liczba_krawedzi; i++){
        lista_b[i] = this->lista_betonu[i];
    }
    a.lista_betonu = lista_b;

    //lista wysokosci
    int * lista_w = new int [a.liczba_wierzcholkow];
    for (int i=0 ; i<a.liczba_wierzcholkow; i++){
        lista_w[i] = this->lista_wysokosci[i];
    }
    a.lista_wysokosci = lista_w;

    return a;
}

void Graph::save_graph(string name){
    ofstream out;
    string long_name;
    //long_name = "D:\\Dokumenty\\AGH\\Semestr 5\\MMWD\\Symulacja\\build-trasy_biegaczy-Desktop_Qt_5_4_0_MinGW_32bit-Debug"+ name +".txt";
    long_name = "C:\\Users\\Dzientak\\Documents\\GitHub\\Trasy\\Trasy_biegaczy\\testy\\"+ name +".txt";
    //long_name = "\\testy\\"+ name +".txt";
    //"D:\Dokumenty\AGH\Semestr 5\MMWD\Symulacja\build-trasy_biegaczy-Desktop_Qt_5_4_0_MinGW_32bit-Debug"+ name +".txt";
    cout<< long_name << endl;
    out.open(long_name.c_str(), ios_base::out);

    if(out.is_open()){
        //out.width(4);
        out << liczba_wierzcholkow <<endl;
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            for(int j =0; j<this->liczba_wierzcholkow; j++){
                out.width(5);
                out << this->macierz_przyleglosci[i][j] << ' ';
            }
            out<< endl;
        }

        out.close();
    }
    else
    {
        cout << "Nie otwarto pliku" <<endl;
    }

}

void Graph::load_graph(string filename){
    //13 zmiennych
    ifstream in;
    string name;

    this->destroy();

    /** macierz przyleglosci **/
    name = filename +".txt";
    in.open(name.c_str());
    if(in.good()){
        in>>this->liczba_wierzcholkow;
        //cout<<"wierz1: "<<this->liczba_wierzcholkow<<endl;
        (this->macierz_przyleglosci)  = new unsigned int * [this->liczba_wierzcholkow];
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            this->macierz_przyleglosci [i] = new unsigned int [(this->liczba_wierzcholkow)];
        }
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            for (int j=0 ; j<this->liczba_wierzcholkow; j++){
                in>>this->macierz_przyleglosci [i][j];
            }
        }
        in.close();
    }
    else{
        cerr << "Nie udało się otworzyc pliku z macierza przyleglosci: "<< name << endl;
    }

    /** macierz betonu **/
    name = filename +".mbet";
    in.open(name.c_str());
    if(in.good()){
        in>>this->liczba_wierzcholkow;
        //cout<<"wierz2: "<<this->liczba_wierzcholkow<<endl;
        this->macierz_betonu = new int * [this->liczba_wierzcholkow];
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            this->macierz_betonu[i] = new int [(this->liczba_wierzcholkow)];
        }
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            for (int j=0 ; j<this->liczba_wierzcholkow; j++){
                in>>this->macierz_betonu[i][j];
            }
        }
        in.close();
    }
    else{
        cerr << "Nie udało się otworzyc pliku z macierza betonu: "<< name << endl;
    }

    /** macierz wysokosci **/
    name = filename +".mwys";
    in.open(name.c_str());
    if(in.good()){
        in>>this->liczba_wierzcholkow;
        //cout<<"wierz3: "<<this->liczba_wierzcholkow<<endl;
        this->macierz_wysokosci = new int * [this->liczba_wierzcholkow];
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            this->macierz_wysokosci[i] = new int [(this->liczba_wierzcholkow)];
        }
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            for (int j=0 ; j<this->liczba_wierzcholkow; j++){
                in>>this->macierz_wysokosci[i][j];
            }
        }
        in.close();
    }
    else{
        cerr << "Nie udało się otworzyc pliku z macierza wysokosci: "<< name << endl;
    }

    /** lista wierzcholkow **/
    name = filename + ".xy";
    in.open(name.c_str());
    int x = 0, y = 0; //zmienne tymczasowe;
    this->lista_wierzcholkow = new QPoint[this->liczba_wierzcholkow];
    if(in.good()){
        in>> this->szerokosc_grafu;
        in>> this->wysokosc_grafu;
        //cout<<"szer: "<<this->szerokosc_grafu<<"wys: "<< this->wysokosc_grafu<<endl;
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            in>>x>>y;
            this->lista_wierzcholkow[i] = QPoint(x,y);
        }
        in.close();
    }
    else{
         cerr << "Nie udało się otworzyc pliku z wierzcholkami: "<< name << endl;
    }

    /** lista wysokosci **/
    name = filename +".wys";
    in.open(name.c_str());
    this->lista_wysokosci = new int [this->liczba_wierzcholkow];
    if(in.good()){
        in>>this->min_wysokosc>>this->max_wysokosc;
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            in>>this->lista_wysokosci[i];
        }
        in.close();
    }
    else{
         cerr << "Nie udało się otworzyc pliku z listą wysokości : "<< name << endl;
    }

    /** lista betonu **/
    name = filename +".bet";
    in.open(name.c_str());
    this->lista_betonu = new int [this->liczba_krawedzi];
    if(in.good()){
        in>>this->liczba_krawedzi;
        //cout<<"kraw1: "<<this->liczba_krawedzi<<endl;
        for (int i=0 ; i<this->liczba_krawedzi; i++){
            in>>this->lista_betonu[i];
        }
        in.close();
    }
    else{
         cerr << "Nie udało się otworzyc pliku z listą betonu : "<< name << endl;
    }

    /** lista krawedzi **/
    name = filename +".kr";
    in.open(name.c_str());
    if(in.good()){
        in>>this->liczba_krawedzi;
        //cout<<"kraw2: "<<this->liczba_krawedzi<<endl;
        this->lista_krawedzi = new QLine [this->liczba_krawedzi];
        int x1,y1,x2,y2;
        for (int i=0 ; i<(this->liczba_krawedzi); i++){
            in>>x1>>y1>>x2>>y2;
            this->lista_krawedzi[i] = QLine(x1,y1,x2,y2);
        }
        in.close();
    }
    else{
         cerr << "Nie udało się otworzyc pliku z listą krawędzi : "<< name << endl;
    }
}

void Graph::create_graph(string outFileName, int h, int w, int marginesX, int marginesY,
                     int szerokosc, int wysokosc, bool czy_kwadrat, int roznorodnosc,
                     bool pionowo, bool poziomo, bool skos1, bool skos2, int betonowosc, int profil)
{
    this->destroy();

    //13 zmiennych
    ofstream out;
    string name;
    this->liczba_wierzcholkow = h*w; //ile wszystkich
    this->szerokosc_grafu = w;
    this->wysokosc_grafu = h;

    int** wspolrzedneX;
    int** wspolrzedneY;

    int u; //iterator


    /** Wspolrzedne punktow **/
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


    // zapisanie współrzednych do pliku
    name = outFileName+".xy";
    out.open(name.c_str(), ios::out);
    if(out.good())
    {
        out.width(4);
        this->lista_wierzcholkow = new QPoint[this->liczba_wierzcholkow];//do klasy Graph
        u = 0;
        //pierwsza linijka - w h
        out.width(4);
        out << this->szerokosc_grafu;
        out.width(4);
        out << this->wysokosc_grafu<<endl;
        for (int i=0 ; i<h; i++){
            for(int j =0; j<w; j++){
                out.width(4);
                out << wspolrzedneX[i][j];
                out.width(4);
                out << wspolrzedneY[i][j];
                out<< endl;
                this->lista_wierzcholkow[u] = QPoint(wspolrzedneX[i][j],wspolrzedneY[i][j]);
                u++; //u to po prostu iterator
            }
        }
        out.close();
    }
    else
        cout << "Nie utworzono pliku ze wspolrzednymi wierzcholkow: "<< name << endl;

    /** macierz przyleglosci **/
    /* wierzcholek moze laczyc sie tylko z wierzcholkami sasiednimi, czyli majac polozenie x, y kandydatami sa:
       x,   y,  numer
       x+1, y,  numer + w      - jeden w dol
       x+1, y+1 numer +w+1     - jeden w prawo i jeden w dol (przekatna)
       x,   y+1 numer+1        - jeden w prawo
       x+1, y-1 numer+w -1     - 2nd przekatna  - nie dziala dla y==0, czyli numer%w==1

     macierz ma wymiar (h*x) x (this->liczba_wierzcholkow) - calkiem spora sie robi, ale wypelniamy tylko g�rna pol�wk�
     wartosci - rzeczywiste odleglosci miedzy punktami (zeby cos sie dzialo)
    */


    this->macierz_przyleglosci = new unsigned int *[this->liczba_wierzcholkow];
    for (int i = 0; i< this->liczba_wierzcholkow; i++){
        this->macierz_przyleglosci[i] = new unsigned int [this->liczba_wierzcholkow];
    }
    //zerowanie - ona w calosci idzie do pliku pozniej
    for (int i=0 ; i< this->liczba_wierzcholkow; i++){
        for(int j =0; j< this->liczba_wierzcholkow; j++){
            this->macierz_przyleglosci[i][j] = 0;
        }
    }
    for (int i=0 ; i<h; i++){
        for(int j=0; j<w; j++){
            int szansa = 4; //rand() % 5+2; // ile krawedzi
            int ktora = -1;
            for(int m =0; m<szansa; m++){
                ktora = ktora+1; //rand() % 4; //ktora krawedz
                switch (ktora){
                    case 0:
                        // jeden w prawo
                        if(poziomo){
                            if((((i)*w+j+1)< this->liczba_wierzcholkow)&&((j+1)<w)){
                                this->macierz_przyleglosci[i*w+j][i*w+(j+1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i][j+1], wspolrzedneY[i][j+1]);
                            }
                        }
                        break;
                    case 1:
                        //jeden w dol
                        if(pionowo){
                            if((((i+1)*w+j)< this->liczba_wierzcholkow)&&((i+1)<h)){
                                this->macierz_przyleglosci[i*w+j][(i+1)*w+(j)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j], wspolrzedneY[i+1][j]);
                            }
                        }
                        break;
                    case 2:
                        //przekatna na +
                        if(skos1){
                            if((((i+1)*w+j+1)<this->liczba_wierzcholkow)&&((j+1)<w)&&((i+1)<h)){
                                this->macierz_przyleglosci[i*w+j][(i+1)*w+ (j+1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j+1], wspolrzedneY[i+1][j+1]);
                            }
                        }
                        break;
                    case 3:
                        //przekatna na -
                        if (skos2){
                            if(((i*w+j+1)%w!=1)&&((i+1)*w+ (j-1)<this->liczba_wierzcholkow)){
                                this->macierz_przyleglosci[i*w+j][(i+1)*w+ (j-1)] = this->distance(wspolrzedneX[i][j],wspolrzedneY[i][j],wspolrzedneX[i+1][j-1], wspolrzedneY[i+1][j-1]);
                            }
                        }
                        break;
                    default:
                        break;
                }
            }
        }

    }

    //Symetryczność macierzy
    for (int i = 0;i<this->liczba_wierzcholkow;i++){
        for(int j=0; j<i; j++){
            this->macierz_przyleglosci[i][j] =  this->macierz_przyleglosci[j][i];
        }
    }

    //zapis macierzy przyległości do pliku
    name = outFileName+".txt";
    out.open(name.c_str());
    if(out.good()){
        out<<this->liczba_wierzcholkow<<endl;
        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            for(int j =0; j<this->liczba_wierzcholkow; j++){
                out.width(4);
                out << this->macierz_przyleglosci[i][j];
            }
            out<< endl;
        }
        out.close();
    }
    else{
        cout << "Nie utworzono pliku macierzy przyległości: "<<name<< endl;
    }

    /** lista krawedzi **/
    name = outFileName+".kr";
    out.open(name.c_str());
    int licznik_krawedzi = 0;

    for (int i=0 ; i< this->liczba_wierzcholkow; i++){
        for(int j =i; j< this->liczba_wierzcholkow; j++){
            if(macierz_przyleglosci[i][j]!=0){
                licznik_krawedzi++;
            }
        }
    }


    this->liczba_krawedzi = licznik_krawedzi;
    this->lista_krawedzi = new QLine[licznik_krawedzi];

    int x1, x2, y1, y2;
    u = 0;
    if(out.good()){
        out.width(4);
        out<<licznik_krawedzi<<endl; //pierwsza linijka pliku
        for (int i=0 ; i< (this->liczba_wierzcholkow); i++){
            for(int j =i; j< (this->liczba_wierzcholkow); j++){
                if(this->macierz_przyleglosci[i][j]!=0){
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
                    out<< endl;

                    this->lista_krawedzi[u] = QLine(x1,y1,x2,y2);
                    u++;
                }
            }
        }
        out.close();
    }
    else{
        cout << "Nie utworzono pliku z lista krawedzi: "<<name<< endl;
    }


    /** lista wysokości wierzcholkow + macierz **/

    this->lista_wysokosci = new int[this->liczba_wierzcholkow];

    switch(profil){
    case 1: //góra - nie działa
        u = 0;
        for (int i=0 ; i<h; i++){
            for(int j =0; j<w; j++){
                this->lista_wysokosci[u] = -1*(i-h/2)*(j-w/2);
                u++; //u to po prostu iterator
            }
        }
        break;
    case 2: //dolina -nie działa wciąż!
        u = 0;
        for (int i=0 ; i<h; i++){
            for(int j =0; j<w; j++){
                this->lista_wysokosci[u] = (i-h/2)*(j-w/2);
                u++; //u to po prostu iterator
            }
        }
        break;
    case 3: //przelęcz
        u = 0;
        for (int i=0 ; i<h; i++){
            for(int j =0; j<w; j++){
                this->lista_wysokosci[u] = -1*(i-h/2)*(j-w/2);
                u++; //u to po prostu iterator
            }
        }
        break;
    case 0: //losowo
    default: //losowo też
        for(int i=0; i<(this->liczba_wierzcholkow); i++){
            this->lista_wysokosci[i] = rand()%100;
            //cerr<<this->lista_wysokosci[i]<<endl;
        }
        break;
    }
    //min i max wysokość
    this->max_wysokosc = (this->lista_wysokosci[0]);
    this->min_wysokosc = this->lista_wysokosci[0];

    for(int i=1; i<(this->liczba_wierzcholkow); i++){
        if((this->lista_wysokosci[i])>this->max_wysokosc){
            this->max_wysokosc = this->lista_wysokosci[i];
        }
        if((this->lista_wysokosci[i])<this->min_wysokosc){
            this->min_wysokosc = this->lista_wysokosci[i];
        }
    }


    // zapisanie listy wysokości do pliku
    name = outFileName+".wys";
    out.open(name.c_str());

    if(out.good()){
        //pierwsza linijka - min i max
        out.width(5);
        out << this->min_wysokosc;
        out.width(5);
        out << this->max_wysokosc;

        for(int i=0; i<(this->liczba_wierzcholkow); i++){
            out.width(4);
            out<< (this->lista_wysokosci[i]);
            out<< endl;
        }
        out.close();
    }
    else{
        cout << "Nie utworzono pliku z lista wsokosci wierzcholkow: "<<name<< endl;
    }
    //cout<<"Lista wysokosci OK";

    //macierz wysokości

    this->macierz_wysokosci = new int *[this->liczba_wierzcholkow];
    for (int i = 0; i< this->liczba_wierzcholkow; i++){
        this->macierz_wysokosci[i] = new int [this->liczba_wierzcholkow];
    }

    for (int i=0 ; i<this->liczba_wierzcholkow; i++){
        for(int j =0; j<this->liczba_wierzcholkow; j++){
            this->macierz_wysokosci[i][j] = 0;
            if(this->macierz_przyleglosci[i][j]!=0){
                this->macierz_wysokosci[i][j] = (this->lista_wysokosci[i]) - (this->lista_wysokosci[j]); //albo na odwrót
            }
        }
    }

    // zapisanie macierzy wysokości do pliku
    name = outFileName+".mwys";
    out.open(name.c_str());

    if(out.good()){
        out << this->liczba_wierzcholkow <<endl;

        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            for(int j =0; j<this->liczba_wierzcholkow; j++){
                out.width(4);
                out << this->macierz_wysokosci[i][j];
            }
            out<< endl;
        }

        out.close();
    }
    else{
        cout << "Nie utworzono pliku z macierza wysokosci: "<<name<< endl;
    }


//betonowość
    this->lista_betonu = new int [this->liczba_krawedzi];

    switch(betonowosc){
    case 1: //pion i poziom
        u = 0;
        for (int i=0 ; i< (this->liczba_wierzcholkow); i++){
            for(int j =i; j< (this->liczba_wierzcholkow); j++){
                if(this->macierz_przyleglosci[i][j]!=0){
                    this->lista_betonu[u] = 0;
                    if(i == (j+1)){
                        this->lista_betonu[u] = 1;
                    }
                    if((i+1) == (j+1)){
                        this->lista_betonu[u] = 1;
                    }
                    u++;
                }
            }
        }
    case 0: //losowo
        for (int i=0 ; i< (this->liczba_krawedzi); i++){
            this->lista_betonu [i] = rand() %2;
        }
        break;
    case 2: //brak
    default: //brak betonu
        for (int i=0 ; i< (this->liczba_krawedzi); i++){
            this->lista_betonu [i] = 0;
        }
        break;
    }


    // zapisanie listy betonu do pliku
    name = outFileName+".bet";
    out.open(name.c_str());

    if(out.good()){
        out<<(this->liczba_krawedzi)<<endl;

        for (int i=0 ; i< (this->liczba_krawedzi); i++){
            out<<(this->lista_betonu [i]);
            out<<endl;
        }
        out.close();
    }
    else{
        cout << "Nie utworzono pliku z lista betonu: "<<name<< endl;
    }
    //cout<<"Beton"<<endl;

    //macierz betonu

    this->macierz_betonu = new int *[this->liczba_wierzcholkow];
    for (int i = 0; i< this->liczba_wierzcholkow; i++){
        this->macierz_betonu[i] = new int [this->liczba_wierzcholkow];
    }
    //zerowanie - ona w calosci idzie do pliku pozniej
    for (int i=0 ; i< this->liczba_wierzcholkow; i++){
        for(int j =0; j< this->liczba_wierzcholkow; j++){
            this->macierz_betonu[i][j] = 0;
        }
    }

    u = 0;
    for (int i=0 ; i< (this->liczba_wierzcholkow); i++){
        for(int j =i; j< (this->liczba_wierzcholkow); j++){
            if(this->macierz_przyleglosci[i][j]!=0){
                this->macierz_betonu[i][j] = this->lista_betonu[u];
                u++;
            }
        }
    }
    // zapisanie macierzy betonu do pliku
    name = outFileName+".mbet";
    out.open(name.c_str());

    if(out.good()){
        out<<(this->liczba_wierzcholkow)<<endl;//pierwsza linia

        for (int i=0 ; i<this->liczba_wierzcholkow; i++){
            for(int j =0; j<this->liczba_wierzcholkow; j++){
                out.width(4);
                out << this->macierz_betonu[i][j];
            }
            out<< endl;
        }
        out.close();
    }
    else{
        cout << "Nie utworzono pliku z macierza betonu: "<<name<< endl;
    }
}


unsigned int Graph :: distance(int ax,int ay,int bx,int by){
    return (unsigned int)(sqrt((ax-bx)*(ax-bx) + (ay-by)*(ay-by)));
}

void Graph::debug(){
    cout<<"Liczba krawedzi: "<<this->liczba_krawedzi<<endl;
    cout<<"Liczba wierzcholkow: "<<this->liczba_wierzcholkow<<endl;
    cout<<"Max wys: "<< this->max_wysokosc<< endl;
    cout<<"Min wys: "<< this->min_wysokosc<< endl;
    cout<<"Ostatni wierzcholek "<< this->lista_wierzcholkow[this->liczba_wierzcholkow-1].x()<<" "<< this->lista_wierzcholkow[this->liczba_wierzcholkow-1].y()<<endl;
    cout<<"Ostatnia krawedz: "<< this->lista_krawedzi[this->liczba_krawedzi-1].x1()<<" "<< endl;
}

void Graph::destroy(){
        //fajne usuwanie
        for (int i = 0; i<(liczba_wierzcholkow); i++){
            delete macierz_przyleglosci[i];
            delete macierz_betonu[i];
            delete macierz_wysokosci[i];
        }
        delete macierz_przyleglosci;
        delete macierz_betonu;
        delete macierz_wysokosci;
        macierz_przyleglosci = NULL;
        macierz_betonu = NULL;
        macierz_wysokosci = NULL;

        delete lista_wierzcholkow;
        lista_wierzcholkow = NULL;

        delete lista_betonu;
        lista_betonu = NULL;

        delete lista_krawedzi;
        lista_krawedzi = NULL;

        delete lista_wysokosci;
        lista_wysokosci = NULL;

        min_wysokosc = 0;
        max_wysokosc = 0;
        szerokosc_grafu = 0;
        wysokosc_grafu = 0;
        liczba_krawedzi = 0;
        liczba_wierzcholkow = 0;

}
