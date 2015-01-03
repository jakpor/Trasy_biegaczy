#include "trasa.h"

Trasa::Trasa(){
    wierzcholek_poczatkowy= 0;
    wierzcholek_koncowy =0;
    f_distance = QVector <int> ();
    f_attractiveness = QVector <int> ();
    f_profile = QVector <int> ();
    funkcja_celu = QVector <int> ();
    path_best = QVector <int> ();
    path_all = QVector < QVector < int > >();

}
Trasa::Trasa(int start, int end){
    wierzcholek_poczatkowy= start;
    wierzcholek_koncowy = end;
    f_distance = QVector <int> ();
    f_attractiveness = QVector <int> ();
    f_profile = QVector <int> ();
    funkcja_celu = QVector <int> ();
    path_best = QVector <int> ();
    path_all = QVector < QVector < int > >();
}

Trasa::~Trasa(){

}

Trasa::Trasa(Trasa & trasa){
    wierzcholek_poczatkowy= trasa.wierzcholek_poczatkowy;
    wierzcholek_koncowy = trasa.wierzcholek_koncowy;
    f_distance = QVector<int>(trasa.f_distance);
    f_attractiveness = QVector<int>(trasa.f_attractiveness);
    funkcja_celu = QVector<int>(trasa.funkcja_celu);
    path_best = QVector <int> (trasa.path_best);
    //uwaga - nie wiem, czy to jest inteligentne!
    path_all = QVector < QVector < int > >(trasa.path_all);

}

int Trasa::dijkstra(int wierzcholek_poczatkowy, int wierzcholek_koncowy, Graph graf, kryterium type){
    unsigned int** Matrix;
    if(wierzcholek_poczatkowy<0 || wierzcholek_koncowy >= graf.liczba_wierzcholkow || wierzcholek_poczatkowy==wierzcholek_koncowy)
        return 0;
//to nie dziala!!
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
    int * Previous;
    T = new unsigned int [graf.liczba_wierzcholkow];
    P = new unsigned int [graf.liczba_wierzcholkow];
    Previous = new int [graf.liczba_wierzcholkow];

    for(int i=0; i < graf.liczba_wierzcholkow; i++){
        //T[i]=Matrix[wierzcholek_poczatkowy][i];
        T[i]=0;
        P[i]=0;
        Previous[i]=0;
    }

    P[wierzcholek_poczatkowy]=1000000; // zabezpieczenie od zmiany
    QVector<int> followers;
    int current;
    current = wierzcholek_poczatkowy;

//    followers = nastepniki(wierzcholek_poczatkowy, Matrix, graf.liczba_wierzcholkow);
//    for (int i=0;i<followers.size();i++){
//        if(followers[i]==wierzcholek_koncowy)
//            return P[wierzcholek_koncowy];
//    }

   // T[current]= INF;

    while(current!= wierzcholek_koncowy){
        followers = nastepniki(current, Matrix, graf.liczba_wierzcholkow);
        for (int i=0; i<followers.size(); i++){
            if(T[followers[i]]==0){
                Previous[followers[i]]=current;
                T[followers[i]]=T[current]+Matrix[current][followers[i]];
            }
            else{
                int old=T[followers[i]];
                T[followers[i]]=min(T[followers[i]], T[current]+Matrix[current][followers[i]]);
                if(old!=T[followers[i]])
                    Previous[followers[i]]=current;
            }
        }
        current=minimum(T, P, graf.liczba_wierzcholkow);
        P[current]=T[current];
        //cout<< P[current];
        //T[current]= INF;
    }

QVector<int> result=build_result(Previous,wierzcholek_poczatkowy,wierzcholek_koncowy);
    this->path_best = result;
    //            result.push_back(wierzcholek_koncowy);
    //            result=build_result(result,Previous,wierzcholek_poczatkowy);
            for(int i =0; i<result.size(); i++){
                cout<<result[i]<<' ';
            }
    //    for(int i=0; i<graf.liczba_wierzcholkow; i++)
    //        cout<< Previous[i]<<' ';


    int wynik =P[current];
    delete T;
    delete P;
    return wynik;

}

QVector<int> Trasa::build_result(int * history, int start, int end){
    QVector<int> result;
    int i=end;
    while (i!=start) {
        result.push_back(i);
        i=history[i];
    }
    result.push_back(start);
    return result;

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

QVector<int> Trasa::nastepniki(int x, unsigned int** A, int n){
    QVector<int> wynik;

    for (int i=0;i<n;i++)
        if ( A[x][i]!=0 )
            wynik.push_back(i);

    return(wynik);
}
