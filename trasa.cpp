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

void Trasa::clear_result(){
        f_distance.clear();
        f_attractiveness.clear();
        f_profile.clear();
        funkcja_celu.clear();
        path_best.clear();
        for( int i =path_all.size()-1; i>=0; i--){
            path_all[i].clear();
        }
        path_all.clear();
        historia_tras.clear();
        Long_Term.clear();
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
    wynik=wynik*100;
    wynik= wynik / (odcinek.size()-1);
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)
int Trasa::calc_attractiveness(){
    int wynik = 0;
    for(int i=0; i< path_best.size()-1; i++){
        wynik+=Graf.macierz_betonu[path_best[i]][path_best[i+1]];
    }
    wynik=wynik*100;
    wynik= wynik / path_best.size();
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
    wynik=wynik*100;
    wynik= wynik / (e-s);
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
   // wynik= abs(this->w_attractiveness*calc_attractiveness(odcinek)) + abs(this->w_distance*calc_distance(odcinek)) + abs(w_profile*calc_profile(odcinek));
    return wynik;
}

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)
//wywoluje wszystkie do policzenia i pushuje je do rozwiązań
//brak zabezpieczenia przed niezsumowaniem się w_(...)
int Trasa::calc_funkcja_celu(){
    int wynik = 0;
    wynik   = this->w_attractiveness * abs(this->calc_attractiveness())
            + this->w_distance * abs(this->calc_distance())
            + w_profile * abs(this->calc_profile());
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
    //this->f_distance.push_back(wynik);
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

    int min = temp.front();

    int i_min =0;

   for(int i=0; i<temp.size(); i++){

           if(abs(temp[i])<min){
               min=temp[i];
               i_min=i;
           }
   }

   return i_min;

}

int Trasa::minimumMarks(QVector<int> temp){

    int min = temp.front();

    int i_min =0;
    int flag=1;

   for(int i=0; i<temp.size(); i++){

       if(Long_Term.contains(path_best[i])){
           flag++;
       }
       else{

               if(abs(temp[i])<min){
                   min=temp[i];
                   i_min=i;
               }
       }
   }

   if(flag == temp.size()){
       return flag;
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

int Trasa::maksimum(QVector<int> temp){

    int max = temp.front();

    int i_max =0;
    int flag=1;

   for(int i=0; i<temp.size(); i++){

       if(Long_Term.contains(path_best[i])){
                          flag++;
           }
       else{
       if(temp[i]>max){
           max=temp[i];
           i_max=i;

       }
   }
   }

   if(flag == temp.size()){
       return flag;
   }
   return i_max;

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

            if(l_krawedzi == 2)
                if(n_last.contains(wykluczenie.front())){
                    line.push_back(wykluczenie.front());
                    line.push_back(wykluczenie.back());
                    wynik.push_back(line);
                    line.clear();
                }

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
                     if(this->path_best.contains(kand_1[j]) == false && this->path_best.contains(n_first[i]) == false){

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

    try{
        //cerr<<"Rozmiar historii przed to: "<<this->historia_tras.size()<<endl;
        for( int i = this->historia_tras.size()-1; i>=0; i--){
            //cerr<<"delete "<<i<<" "<<this->historia_tras.size()<<endl;
            delete this->historia_tras[i];
        }
        this->historia_tras.clear();
        //cerr<<"Rozmiar historii po to: "<<this->historia_tras.size()<<endl;

        QVector <int> temp = QVector <int>();
        // kaw[0] - poczatek linii, kaw[1] - koniec linii (wiekszy niz kaw[0]), kaw[2] - ile razy sie powtorzyl
        int kawalek_historii[3];
        int sprawdzenie_czy_if = 0;

        //pierwszy element historii tras to min i max
        this->historia_tras.append(new int[3]);
        this->historia_tras.last()[0] = 1;
        this->historia_tras.last()[1] = 1;
        this->historia_tras.last()[2] = 0;

        for(int i = 0; i<path_all.size();i++){
            temp.clear();
            temp = QVector<int>(path_all[i]);

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
                sprawdzenie_czy_if = 0;

                for(int u = 1; u<(this->historia_tras.size()); u++){ //tu się może psuć!!!
                    if((this->historia_tras[u][0]==kawalek_historii[0])&&(this->historia_tras[u][1]==kawalek_historii[1])){
                        this->historia_tras[u][2]++; //zwieksz ilosc
                        sprawdzenie_czy_if = 1;
                        // zwiekszam maksimum
                        if (this->historia_tras[u][2]>this->historia_tras[0][1]){
                            this->historia_tras[0][1]=this->historia_tras[u][2];
                        }
                    }
                }

                if(sprawdzenie_czy_if==0){
                    //jesli if sie nie wykonaly
                    this->historia_tras.push_back(new int[3]);
                    this->historia_tras.last()[0] = kawalek_historii[0];
                    this->historia_tras.last()[1] = kawalek_historii[1];
                    this->historia_tras.last()[2] = kawalek_historii[2];
                    sprawdzenie_czy_if = 0;

                    //cout<<"ostatni: "<<historia_tras.last()[0]<< " "<< historia_tras.last()[1]<< " "<< historia_tras.last()[2]<<endl;
                }
            }
        }
    }
    catch(...){
        cerr<<"ERROR: Historia trasy: Znowu historia sieje bledami..."<<endl;
    }

//    cerr<< "Aktualna historia to: (A,B,ilosc): ";
//    for(int i = 0; i<historia_tras.size(); i++){
//        cerr<<"("<<historia_tras.at(i)[0]<<", "<< historia_tras.at(i)[1]<<", "<< historia_tras.at(i)[2]<<") ";
//    }
//    cerr<< endl;
}

void Trasa::algorithm_1(int ile_wykluczac){
//cout<<"5 ";
    int acc =this->dijkstra(distances); // jesli zwraca zero to brak rozwiazan lub unvalid edges
    if(acc==0) return;
    acc= this->wanted_distance - acc;
    //

    QVector<int> Marks; //tablica oceny wszystkich potencjalnych wykluczeń
    QVector< QVector<int> > Potencials; //zbior tras do włączenia potencjalnie do trasy
//cout<<"6 ";
    this->f_distance.push_back(acc); //aktualizacja listy rozwiązań
    this->path_all.push_back(this->path_best);
    if(acc<0)
        return;
    //stworzenie tablicy wartości dla poszczegolnych wykluczen potencjalnych
    for(int i=0; i<this->path_best.size()-ile_wykluczac; i++){
        Marks.push_back(this->calc_distance(i, i+ile_wykluczac));
    }
    int iteracje=0;
    int znacznik_zmian=0;
//cout<<"7 ";

//Główna pętla algorytmu
    while( acc!= 0 && iteracje<100 && znacznik_zmian<20){

         iteracje++;

//cout<<"8 ";
        int wyklucz;
        if(acc<0){
            wyklucz=maksimum(Marks);
        }
        else{
            wyklucz= minimumMarks(Marks); //wybranie najlepszego wykluczenia - najgorsze polepszenie f. celu
        }
        if(Long_Term.contains(path_best[wyklucz])){
            cerr<<"wykluczamy cos co jest zabronione";
            break;
        }
//cout<<"9 ";
        if(wyklucz==Marks.size()){ //warunek na brak nastepnikow
            cerr<<"brak rozwiazan dopuszczalnych w otoczeniu";
            break;
        }
//cout<<"10a ";

        QVector<int> Wykluczenie; //zbudowanie vektora wykluczenia wysylanego do metody otoczenie
        for(int i=0; i<=ile_wykluczac;i++){

            Wykluczenie.push_back(this->path_best[wyklucz+i]);
        }
//cout<<"10b ";

        Potencials = otoczenie(Wykluczenie, 2, ile_wykluczac); //wszystkie potencjalne zamienniki dla naszego wykluczenia
        Wykluczenie.clear();

        if(Potencials.size()==0){ //-warunek na brak otoczenia krawedzi
            Long_Term.push_back(path_best[wyklucz]);
            //Marks[wyklucz]=1000000; //zabronienie dlugoterminowe na wykluczanie tej krawedzi.
            cout<< endl<<path_best[wyklucz]<< " :nie powinno byc juz wykluczane (0)"<<endl;
            continue;
        }
//cout<<"11 ";
        QVector<int> Potencials_Marks; //ocena wszystkich zamienników pod względem polepszenia f. celu
//wartość bezwzględna( ile brakuje + ile da wykluczenie - ile da zamiennik wykluczenia)
        for(int i=0; i< Potencials.size(); i++){
            Potencials_Marks.push_back(abs(acc + Marks[wyklucz]- this->calc_distance(Potencials[i])));
        }
//cout<<"12 ";
        int best_index = minimum(Potencials_Marks); //wybieramy tą która najbardziej zblizyla nam to ile brakuje do zera

        if(best_index==Potencials.size()){
            Long_Term.push_back(path_best[wyklucz]);
            //Marks[wyklucz]=1000000;
        cout<< endl<< path_best[wyklucz]<< " :nie powinno byc juz wykluczane (1)"<<endl;
            continue;
        }
//cout<<"12a ";
//cout<<endl<<Potencials.size()<<" "<<best_index<<" "<<Potencials_Marks.size()<<endl;
        acc -= calc_distance(Potencials[best_index]);
        Potencials_Marks.clear();
//cout<<"12b ";
        acc=acc + Marks[wyklucz];
                  //zmianiamy aktualną wartość tego ile brakuje
//warunek dodania
//cout<<"12c ";
        if(iteracje!=1 &&  abs(f_distance.back()) <= abs(acc)){
            Long_Term.push_back(path_best[wyklucz]);
            //Marks[wyklucz]=1000000;
cerr<< path_best[wyklucz] << " :nie powinno byc juz wykluczane (2)"<<endl;
            znacznik_zmian++;
            if(f_distance.back()*acc >0){
                acc=f_distance.back();
                continue;
            }
//cout<<"13 ";
        }
cout<<path_best[wyklucz]<<" ";
//cout<<"14 ";

        if(ile_wykluczac == 1){
        for(int i=1; i!=Potencials[best_index].size()-1; i++){

            path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);
//cout<<"15 ";
            Marks[wyklucz+i-1]=this->calc_distance(wyklucz+i-ile_wykluczac, wyklucz+i);
            Marks.insert(Marks.begin()+wyklucz+i,this->calc_distance(wyklucz+i, wyklucz+i+ile_wykluczac));
        }
    }
    else if(ile_wykluczac==2){
        for(int i=0; i!=Potencials[best_index].size()-1; i++){
//cout<<"16a ";
            if(i==0 && Potencials[best_index].size()==2){
//cout<<"16b1 ";
                path_best.erase(path_best.begin()+wyklucz+1);
                Marks.erase(Marks.begin()+wyklucz+1);
                Marks[wyklucz]=this->calc_distance(wyklucz, wyklucz+ile_wykluczac);
//cout<<"16b ";
            }
            else if(i==1){
//cout<<"16c1 ";
                path_best[wyklucz+1]=Potencials[best_index][i];

                Marks[wyklucz]=this->calc_distance(wyklucz, wyklucz+ile_wykluczac);
if((wyklucz+1)<Marks.size()){
                Marks[wyklucz+1]=this->calc_distance(wyklucz+1, wyklucz+1+ile_wykluczac);
}
//cout<<"16c ";
            }

            else if(i==2){
//cout<<"16d1 ";
                path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);

                Marks[wyklucz+i-1]=this->calc_distance(wyklucz+i-1, wyklucz+i-1 +ile_wykluczac);
if((wyklucz+i-1)<Marks.size()){
                Marks.insert(Marks.begin()+wyklucz+i-1,this->calc_distance(wyklucz+i-1, wyklucz+i-1+ile_wykluczac));
}
else{
    Marks.push_back(this->calc_distance(wyklucz+i-1, wyklucz+i-1+ile_wykluczac));
}
//cout<<"16d ";
            }
//cout<<"17 ";
        }
//cout<<"17a "   ;
    }

//cout<<"18 ";
        if(f_distance.back()>=abs(acc)){
            this->f_distance.push_back(acc);
            this->path_all.push_back(this->path_best);
            znacznik_zmian=0;
        }
        else{
            znacznik_zmian++;
            this->f_distance.push_back(acc);
            this->path_all.push_back(this->path_best);
            //path_best=path_all.back();
           // Marks[]=
           //Marks[wyklucz]=1000000;
            //acc=f_distance.back();
            //znacznik_zmian++;
//cout<<"19 ";
        }
        Long_Term.push_back(path_best[wyklucz]);
        Potencials.clear();
        calc_attractiveness();
        calc_profile();
        calc_funkcja_celu();
    }
    int minimo=minimum(this->f_distance);
    f_distance.push_back(f_distance[minimo]);

}
