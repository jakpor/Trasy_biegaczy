#include "trasa.h"

Trasa::Trasa(){
    wierzcholek_poczatkowy= 0;
    wierzcholek_koncowy =0;
    f_distance= 0;
    f_attractiveness =0;
    f_profile= 0;
    funkcja_celu =0;
}
Trasa::Trasa(int start, int end){
    wierzcholek_poczatkowy= start;
    wierzcholek_koncowy = end;
    f_distance= 0;
    f_attractiveness =0;
    f_profile= 0;
    funkcja_celu =0;
}

Trasa::~Trasa(){

}

int Trasa::dijkstra(int wierzcholek_poczatkowy, int wierzcholek_koncowy, Graph graf, kryterium type){
    unsigned int** Matrix;

    switch(type){
        case distances:
            Matrix = graf.macierz_przyleglosci;
            break;

        case attractiveness:
            Matrix = (unsigned int **)graf.macierz_betonu;
            break;

        case profile:
            Matrix =(unsigned int **)graf.macierz_wysokosci;
            break;

    }
//    for (int i=0 ; i<graf.liczba_wierzcholkow; i++){
//        for(int j =0; j<graf.liczba_wierzcholkow; j++){
//            cout << Matrix[i][j] << ' ';
//        }
//        cout<< endl;
//    }

unsigned int * T;
unsigned int * P;
T= new unsigned int [graf.liczba_wierzcholkow];
P=new unsigned int [graf.liczba_wierzcholkow];

for(int i=0; i < graf.liczba_wierzcholkow; i++){
    T[i]=Matrix[wierzcholek_poczatkowy][i];
    P[i]=0;
}
    T[wierzcholek_poczatkowy]=1000000;
    P[wierzcholek_poczatkowy]=1000000;
    vector<int> followers;
    int current=wierzcholek_poczatkowy;

//    followers = nastepniki(wierzcholek_poczatkowy, Matrix, graf.liczba_wierzcholkow);
//    for (int i=0;i<followers.size();i++){
//        if(followers[i]==wierzcholek_koncowy)
//            return P[wierzcholek_koncowy];
//    }

   // T[current]= INF;

    while(current!= wierzcholek_koncowy){
        cout<<T[wierzcholek_koncowy]<<endl;
        followers = nastepniki(current, Matrix, graf.liczba_wierzcholkow);
        for (int i=0;i<followers.size();i++){
            if(T[followers[i]]==0)
                T[followers[i]]=T[current]+Matrix[current][followers[i]];
            else{
                T[followers[i]]=min(T[followers[i]], T[current]+Matrix[current][followers[i]]);
            }
        }
        current=minimum(T, P, graf.liczba_wierzcholkow);
        P[current]=T[current];
        //cout<< P[current];
        //T[current]= INF;
    }
    int wynik =P[current];
    delete T;
    delete P;
    return wynik;

}

    int Trasa::minimum(unsigned int * temp, unsigned int * perm,int size){
        unsigned int min = 1000000;
        int i_min;
       for(int i=0; i<size; i++){
           if(perm[i]==INF && temp[i]!=INF){
               if(temp[i]<min){
                   min=temp[i];
                   i_min=i;
               }
           }
       }
       return i_min;

    }

    vector<int> Trasa::nastepniki(int x, unsigned int** A, int n){
        vector<int> wynik;

        for (int i=0;i<n;i++)
            if ( A[x][i]!=0 )
                wynik.push_back(i);

        return(wynik);
    }
