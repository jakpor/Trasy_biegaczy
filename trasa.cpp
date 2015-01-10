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

Trasa::Trasa(Graph Graf_in){
    wierzcholek_poczatkowy= 0;
    wierzcholek_koncowy =0;
    f_distance = QVector <int> ();
    f_attractiveness = QVector <int> ();
    f_profile = QVector <int> ();
    funkcja_celu = QVector <int> ();
    path_best = QVector <int> ();
    path_all = QVector < QVector < int > >();
    Graf= Graf_in.copy_graph();

    }
void Trasa::copy_graf(Graph graf_in){
    Graf=graf_in.copy_graph();
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
    //ja też nie ;)
    path_all = QVector < QVector < int > >(trasa.path_all);

}


//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)
int Trasa::calc_attractiveness(QVector<int> odcinek){
    int wynik = 0;
    for(int i=0; i< odcinek.size()-1; i++){
        wynik+=Graf.macierz_betonu[odcinek[i]][odcinek[i+1]];
    }
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)
int Trasa::calc_attractiveness(){
    int wynik = 0;
    for(int i=0; i< path_best.size()-1; i++){
        wynik+=Graf.macierz_betonu[path_best[i]][path_best[i+1]];
    }
    wynik= this->wanted_attractiveness - wynik;
    this->f_attractiveness.push_back( wynik);
    return wynik;
}

//zwraca 0 jesli indeks startowy lub koncowy invalid
int Trasa::calc_attractiveness(int s, int e){
    int wynik = 0;

    if(e <= s || s<0 || e>this->path_best.size()) return wynik; // sprawdzenie poprawnosci

    for(int i=s; i< e; i++){
        wynik+=Graf.macierz_betonu[path_best[i]][path_best[i+1]];
    }
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)
int Trasa::calc_profile(QVector<int> odcinek){
    int wynik = 0;
    for(int i=0; i< odcinek.size()-1; i++){
        wynik+=Graf.macierz_wysokosci[odcinek[i]][odcinek[i+1]];
    }
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)

int Trasa::calc_profile(){
    int wynik = 0;
    for(int i=0; i< path_best.size()-1; i++){
        wynik+=Graf.macierz_wysokosci[this->path_best[i]][this->path_best[i+1]];
    }
    wynik = this->wanted_profile - wynik;
    this->f_profile.push_back( wynik);
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)

//zwraca 0 jesli indeks startowy lub koncowy invalid
int Trasa::calc_profile(int s, int e){
    int wynik = 0;

    if(e <= s || s<0 || e>this->path_best.size()) return wynik; // sprawdzenie poprawnosci

    for(int i=s; i< e; i++){
        wynik+=Graf.macierz_wysokosci[path_best[i]][path_best[i+1]];
    }
    return wynik;
}

int Trasa::calc_distance(QVector<int> odcinek){
    int wynik = 0;
    for(int i=0; i< odcinek.size()-1; i++){
        wynik+=Graf.macierz_przyleglosci[odcinek[i]][odcinek[i+1]];
    }
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)

int Trasa::calc_distance(){
    int wynik = 0;
    for(int i=0; i< this->path_best.size()-1; i++){
        wynik+=Graf.macierz_przyleglosci[this->path_best[i]][this->path_best[i+1]];
    }
    wynik = this->wanted_distance - wynik;
    this->f_distance.push_back( wynik);
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)

//zwraca 0 jesli indeks startowy lub koncowy invalid
int Trasa::calc_distance(int s, int e){
    int wynik = 0;

    if(e <= s || s<0 || e>this->path_best.size()) return wynik; // sprawdzenie poprawnosci

    for(int i=s; i< e; i++){
        wynik+=Graf.macierz_przyleglosci[path_best[i]][path_best[i+1]];
    }
    return wynik;
}

int Trasa::calc_funkcja_celu(QVector<int> odcinek){
    int wynik = 0;
    wynik= this->w_attractiveness*calc_attractiveness(odcinek)+this->w_distance*calc_distance(odcinek)+w_profile*calc_profile(odcinek);
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)

int Trasa::calc_funkcja_celu(){
    int wynik = 0;
    wynik= this->w_attractiveness*this->f_attractiveness.back()+this->w_distance*this->f_distance.back()+w_profile*this->f_profile.back();
    this->funkcja_celu.push_back(wynik);
    return wynik;
}


void Trasa::set_parameters(int p_distance, int p_attractiveness, int p_profile){
    this->w_distance=(float)p_distance/100.0;
    this->w_attractiveness=(float)p_attractiveness/100.0;
    this->w_profile=(float)p_profile/100.0;

}

void Trasa::set_edges(int start, int end){
    this->wierzcholek_poczatkowy=start;
    this->wierzcholek_koncowy = end;
}


void Trasa::set_wanted(int dist, int att, int prof){
    this->wanted_attractiveness=att;
    this->wanted_distance=dist;
    this->wanted_profile=prof;
}


int Trasa::dijkstra(kryterium type){
    unsigned int** Matrix;
    if(this->wierzcholek_poczatkowy<0 || this->wierzcholek_koncowy >= Graf.liczba_wierzcholkow || this->wierzcholek_poczatkowy==this->wierzcholek_koncowy)
        return 0;
//to nie dziala!!
    switch(type){
        case distances:
            Matrix = Graf.macierz_przyleglosci;
            break;

        case attractiveness:
            Matrix = (unsigned int **)Graf.macierz_betonu;
            break;

        case profile:
            Matrix =(unsigned int **)Graf.macierz_wysokosci;
            break;

    }


    unsigned int * T;
    unsigned int * P;
    int * Previous;
    T = new unsigned int [Graf.liczba_wierzcholkow];
    P = new unsigned int [Graf.liczba_wierzcholkow];
    Previous = new int [Graf.liczba_wierzcholkow];

    for(int i=0; i < Graf.liczba_wierzcholkow; i++){
        //T[i]=Matrix[wierzcholek_poczatkowy][i];
        T[i]=0;
        P[i]=0;
        Previous[i]=0;
    }

    P[this->wierzcholek_poczatkowy]=1000000; // zabezpieczenie od zmiany
    QVector<int> followers;
    int current;
    current = this->wierzcholek_poczatkowy;

//    followers = nastepniki(wierzcholek_poczatkowy, Matrix, graf.liczba_wierzcholkow);
//    for (int i=0;i<followers.size();i++){
//        if(followers[i]==wierzcholek_koncowy)
//            return P[wierzcholek_koncowy];
//    }

   // T[current]= INF;

    while(current!= this->wierzcholek_koncowy){
        followers = nastepniki(current);
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
        current=minimum(T, P, Graf.liczba_wierzcholkow);
        P[current]=T[current];
        //cout<< P[current];
        //T[current]= INF;
    }

    QVector<int> result = build_result(Previous,this->wierzcholek_poczatkowy,this->wierzcholek_koncowy);
    this->path_best = result;


    int wynik =P[current];
    delete T;
    delete P;
    this->f_distance.push_back(wynik);
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

int Trasa::minimum(QVector<int> temp){

    unsigned int min = temp.front();

    int i_min =0;

   for(int i=0; i<temp.size(); i++){

       if(temp[i]!= 0){
           if(temp[i]<min){
               min=temp[i];
               i_min=i;
           }
       }
   }

   return i_min;

}

QVector<int> Trasa::nastepniki(int x){
    QVector<int> wynik;

    for (int i=0;i<Graf.liczba_wierzcholkow;i++)
        if ( Graf.macierz_przyleglosci[x][i]!=0 )
            wynik.push_back(i);

    return(wynik);
}

//dziala dla 1 lub 2 wykluczanych krawedzi, zwraca otoczenie <= rozmiarowi
QVector< QVector<int> > Trasa::otoczenie (QVector<int> wykluczenie, int rozmiar, int l_krawedzi){

    QVector<int> n_first =nastepniki(wykluczenie.front());
    QVector<int> n_last = nastepniki(wykluczenie.back());
    QVector< QVector<int> >wynik;
    QVector<int> line;


    if(l_krawedzi<3){

            QVector<int> kand_1;
            set_intersection (n_first.begin(), n_first.end(), n_last.begin(), n_last.end(), std::back_inserter(kand_1));

            for(int i=0 ; i < kand_1.size(); ++i) {
                if(this->path_best.contains(kand_1[i]) == false){
                    line.push_back(wykluczenie.front());
                    line.push_back(kand_1[i]);
                    line.push_back(wykluczenie.back());
                    wynik.push_back(line);
                    line.clear();
                }

            }

            kand_1.clear();

            rozmiar--;
            if(rozmiar == 0) return wynik;

            QVector<int> kand_2;
            for(int i=0 ; i<n_first.size(); ++i){
                kand_2=nastepniki(n_first[i]);
                 set_intersection (kand_2.begin(), kand_2.end(), n_last.begin(), n_last.end(), std::back_inserter(kand_1));

                 for(int j=0 ; j < kand_1.size(); ++j) {
//ten warunek nie zadziala z kand2 trzeba zmienic
                     if(this->path_best.contains(kand_1[j]) == false && this->path_best.contains(kand_2[j]) == false){

                         line.push_back(wykluczenie.front());
                         line.push_back(n_first[i]);
                         line.push_back(kand_1[j]);
                         line.push_back(wykluczenie.back());
                         wynik.push_back(line);
                         line.clear();
//cout<<"znalazlo cos ";
                     }

                 }

                 kand_1.clear();
                 kand_2.clear();
            }
            rozmiar--;
            if(rozmiar==0) return wynik;



    }

        return wynik;
}

void Trasa::aktualizuj_historie_tras(){
    historia_tras = QVector<int*>();
    QVector <int> temp;
    // kaw[0] - poczatek linii, kaw[1] - koniec linii (wiekszy niz kaw[0]), kaw[2] - ile razy sie powtorzyl
    int* kawalek_historii;
    int sprawdzenie_czy_if = 0;

    //pierwszy element historii tras to min i max
    historia_tras.append(new int[3]);
    historia_tras.last()[0] = 1;
    historia_tras.last()[1] = 1;
    historia_tras.last()[2] = 0;

    for(int i = 0; i<path_all.size();i++){
        temp = QVector<int>(path_all[i]);
        try{
            kawalek_historii = new int[3];
        }
        catch(...){
            cout<<"Kawalki historii sie rozsypaly"<<endl;
        }

        for(int j = 0;j<(temp.size()-1); j++){
            if(temp[j]<temp[j+1]){
                kawalek_historii[0] = temp[j];
                kawalek_historii[1] = temp[j+1];
            }
            else{
                kawalek_historii[0] = temp[j+1];
                kawalek_historii[1] = temp[j];
            }
            kawalek_historii[2] = 1;
            for(int u = 1; u<(historia_tras.size()-1); u++){ //tu się może psuć!!!
                if(historia_tras[i][0]==kawalek_historii[0]){
                    if(historia_tras[i][1]==kawalek_historii[1]){
                        historia_tras[i][2]++; //zwieksz ilosc
                        sprawdzenie_czy_if = 1;
                        // zwiekszam maksimum
                        if (historia_tras[i][2]>historia_tras[0][1]){
                            historia_tras[0][1]=historia_tras[i][2];
                        }
                    }
                }
            }
            if(sprawdzenie_czy_if==0){
                //jesli if sie nie wykonaly
                historia_tras.append(new int[3]);
                historia_tras.last()[0] = kawalek_historii[0];
                historia_tras.last()[1] = kawalek_historii[1];
                historia_tras.last()[2] = kawalek_historii[2];
                sprawdzenie_czy_if = 0;

                //cout<<"ostatni: "<<historia_tras.last()[0]<< " "<< historia_tras.last()[1]<< " "<< historia_tras.last()[2]<<endl;

            }
        }
    }
}

void Trasa::algorithm_1(){
    int acc =this->dijkstra(distances); // jesli zwraca zero to brak rozwiazan lub unvalid edges
    if(acc==0) return;


    acc= this->wanted_distance - acc;
    QVector<int> Marks;
    QVector< QVector<int> > Potencials;
    this->f_distance.push_back(acc);
    this->path_all.push_back(this->path_best);

    for(int i=0; i<this->path_best.size()-1; i++){
        Marks.push_back(this->calc_distance(i, i+1));
    }
    int iteracje=0;
    while(acc > 0 && iteracje<50){
         iteracje++;

        //cout<<endl<< iteracje <<" dupa 1";
        int wyklucz = minimum(Marks);
        //cout<<"dupa 2";
        QVector<int> Wykluczenie;
        //cout<<"dupa 3";
        Wykluczenie.push_back(this->path_best[wyklucz]);
        //cout<<"dupa 4";
        Wykluczenie.push_back(this->path_best[wyklucz+1]);
        //cout<<"dupa 5";
        Potencials = otoczenie(Wykluczenie, 2, 1);
        if(Potencials.size()==0){
            Marks[wyklucz]=1000000;
            continue;
        }

        //cout<<"dupa 6";
        QVector<int> Potencials_Marks;
        //cout<<"dupa 7";
        for(int i=0; i< Potencials.size(); i++){
            Potencials_Marks.push_back(abs(acc + this->calc_distance(Potencials[i])));
        }
        //cout<<"dupa 8";
        int best_index = minimum(Potencials_Marks);
        //cout<<"dupa8.5";
        acc=this->calc_distance(Potencials[best_index])-Marks[wyklucz];
        //cout<<"dupa 9";
        for(int i=1; i!=Potencials[best_index].size(); i++){

            path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);

            Marks[wyklucz+i-1]=this->calc_distance(wyklucz+i-1, wyklucz+i);
            Marks.insert(Marks.begin()+wyklucz+i,this->calc_distance(wyklucz+i, wyklucz+i+1));
        }
        //cout<<"dupa 10";
        this->f_distance.push_back(acc);
        //cout<<"dupa 11";
        this->path_all.push_back(this->path_best);


    }



}
