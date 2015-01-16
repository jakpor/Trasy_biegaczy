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
    path_all = QVector < QVector < int > >(trasa.path_all);

}


//WYLICZENIE ATRAKCYJNOSCI ODCINKA - SZACOWANIE PRZED DŁUGOŚĆ TRASY OPTYMALNEJ
int Trasa::calc_attractiveness(QVector<int> odcinek){
    int wynik = 0;
    for(int i=0; i< odcinek.size()-1; i++){
        wynik+=Graf.macierz_betonu[odcinek[i]][odcinek[i+1]];
    }
    wynik=wynik*100;

    wynik = wynik / (path_best.size()-1); //przyblizenie globalnego wplywu tej krawedzi
    return wynik; //WYNIK WYRAŻONY W PROCENTACH
}

//WYLICZENIE ATRAKCYJNOSCI AKTUALNEJ TRASY OPTYMALNEJ I AKTUALIZACJA WEKTORA ROZWIAZAŃ
int Trasa::calc_attractiveness(){
    int wynik = 0;
    for(int i=0; i< path_best.size()-1; i++){
        wynik+=Graf.macierz_betonu[path_best[i]][path_best[i+1]];
    }
    wynik=wynik*100;
    wynik= wynik / (path_best.size()-1);
    wynik= this->wanted_attractiveness - wynik; //różnica procentów od zadanej wartości
    this->f_attractiveness.push_back( wynik);
    return wynik;

}

//WYLICZENIE ATRAKCYJNOSCI ODCINKA BĘDĄCEGO FRAGMENTEM TRASY AKTUALNEJ- SZACOWANIE PRZED DŁUGOŚĆ TRASY AKTUALNEJ
int Trasa::calc_attractiveness(int s, int e){
    int wynik = 0;

    if(e <= s || s<0 || e>this->path_best.size())  // sprawdzenie poprawnosci
        return wynik;

    for(int i=s; i< e; i++){
        wynik+=Graf.macierz_betonu[path_best[i]][path_best[i+1]];
    }
    wynik=wynik*100;
    wynik = wynik / (path_best.size()-1);
    return wynik;
}

//WYLICZENIE FUNKCJI KARY ZA PRZEKROCZENIE WARTOSCI ZADANEJ DLA ODCINKA
int Trasa::calc_profile(QVector<int> odcinek){
    int wynik = 0;
    for(int i=0; i< odcinek.size()-1; i++){
        if(Graf.macierz_wysokosci[odcinek[i]][odcinek[i+1]]>wanted_profile){
            wynik+=this->wanted_profile;
            wynik-=Graf.macierz_wysokosci[odcinek[i]][odcinek[i+1]];
        }
    }

    return wynik; //suma nadmiaru wysokości, które zostały przekroczone
}

//WYLICZENIE FUNKCJI KARY ZA PRZEKROCZENIE WARTOSCI ZADANEJ DLA TRASY OPTYMALNEJ I AKTUALIZACJA WEKTORA ROZWIAZAN
int Trasa::calc_profile(){
    int wynik = 0;
    for(int i=0; i< path_best.size()-1; i++){
        if(Graf.macierz_wysokosci[this->path_best[i]][this->path_best[i+1]]>wanted_profile){
            wynik+=this->wanted_profile;
            wynik-=Graf.macierz_wysokosci[this->path_best[i]][this->path_best[i+1]];
        }
    }
    this->f_profile.push_back( wynik);
    return wynik;
}



//WYLICZENIE FUNKCJI KARY ZA PRZEKROCZENIE WARTOSCI ZADANEJ DLA ODCINKA BĘDĄCEGO FRAGMENTEM TRASY AKTUALNEJ
int Trasa::calc_profile(int s, int e){
    int wynik = 0;

    if(e <= s || s<0 || e>this->path_best.size()) // sprawdzenie poprawnosci
        return wynik;

    for(int i=s; i< e; i++){
        if(Graf.macierz_wysokosci[this->path_best[i]][this->path_best[i+1]]>wanted_profile){
            wynik+=this->wanted_profile;
            wynik-=Graf.macierz_wysokosci[path_best[i]][path_best[i+1]];
        }
    }
    return wynik;
}

//WYLICZENIE DŁUGOŚCI ODCINKA
int Trasa::calc_distance(QVector<int> odcinek){

    int wynik = 0;

    for(int i=0; i< odcinek.size()-1; i++){
        wynik+=Graf.macierz_przyleglosci[odcinek[i]][odcinek[i+1]];
    }

    return wynik;
}

//WYLICZENIE UCHYBU DŁUGOŚCI TRASY AKTUALNEJ OD ZADANEJ ODLEGŁOŚCI I DODANIE WYNIKU DO WEKTORA ROZWIĄZAŃ
int Trasa::calc_distance(){
    int wynik = 0;
    for(int i=0; i< this->path_best.size()-1; i++){
        wynik+=Graf.macierz_przyleglosci[this->path_best[i]][this->path_best[i+1]];
    }
    wynik = this->wanted_distance - wynik;
    this->f_distance.push_back( wynik);
    return wynik;
}

//WYLICZENIE DŁUGOŚCI ODCINKA BĘDĄCEGO FRAGMENTEM TRASY AKTUALNEJ
int Trasa::calc_distance(int s, int e){
    int wynik = 0;

    if(e <= s || s<0 || e>this->path_best.size()) return wynik; // sprawdzenie poprawnosci

    for(int i=s; i< e; i++){
        wynik+=Graf.macierz_przyleglosci[path_best[i]][path_best[i+1]];
    }
    return wynik;
}

//FUNKCJA CELU WYLICZONA DLA ODCINKA - SUMA WARTOŚCI BEZWZGLĘDNYCH WARTOŚCI ZWIĄZANYCH ZE SKŁADOWYMI
int Trasa::calc_funkcja_celu(QVector<int> odcinek){
    int wynik = 0;
    wynik= this->w_attractiveness*SKAL_ATTRACTIVENESS* abs(calc_attractiveness(odcinek))
            + this->w_distance*abs(calc_distance(odcinek))
            + w_profile*SKAL_PROFILE* abs(calc_profile(odcinek));
    return wynik;
}

//FUNKCJA CELU WYLICZONA DLA FRAGMENTU TRASY AKTUALNEJ- SUMA WARTOŚCI BEZWZGLĘDNYCH WARTOŚCI ZWIĄZANYCH ZE SKŁADOWYMI
int Trasa::calc_funkcja_celu(int s, int e){
    int wynik = 0;
    if(e <= s || s<0 || e>this->path_best.size()) return wynik; // sprawdzenie poprawnosci
    wynik= this->w_attractiveness*SKAL_ATTRACTIVENESS* abs(calc_attractiveness(s,e))
            + this->w_distance*abs(calc_distance(s,e))
            + w_profile*SKAL_PROFILE* abs(calc_profile(s,e));
    return wynik;
}


//FINALNA FUNKCJA CELU SUMUJĄCA WARTOŚĆ BEZWZGLĘDNĄ SKŁADOWYCH PRZEMNOŻONYCH PRZEZ ZADANY MNOŻNIK (ZAOKRĄGLENIE DO CAŁKOWITYCH)
int Trasa::calc_funkcja_celu(){
    int wynik = 0;
    wynik   = this->w_attractiveness * SKAL_ATTRACTIVENESS* abs(this->f_attractiveness.back())
            + this->w_distance * abs(this->f_distance.back())
            + w_profile * SKAL_PROFILE* abs(this->f_profile.back());
    this->funkcja_celu.push_back(wynik);
    return wynik;
}

//USTAWIENIE PARAMETRÓW ALGORYTMU - MNOŻNIKI DO FUNKCJI CELU
void Trasa::set_parameters(int p_distance, int p_attractiveness, int p_profile){
    this->w_distance=(float)p_distance/100.0;
    this->w_attractiveness=(float)p_attractiveness/100.0;
    this->w_profile=(float)p_profile/100.0;

}

//USTAWIENIE WIERZCHOŁKÓW KRAŃCOWYCH
void Trasa::set_edges(int start, int end){
    this->wierzcholek_poczatkowy=start;
    this->wierzcholek_koncowy = end;
}

//USTAWIENIE WARTOŚCI ZADANYCH
void Trasa::set_wanted(int dist, int att, int prof){
    this->wanted_attractiveness=att;
    this->wanted_distance=dist;
    this->wanted_profile=prof;
}

//ALGORYTM DIJKSTRY BAZUJĄCY NA ODLEGŁOŚCIACH
int Trasa::dijkstra(kryterium type){

    if(this->wierzcholek_poczatkowy<0 || this->wierzcholek_koncowy >= Graf.liczba_wierzcholkow || this->wierzcholek_poczatkowy==this->wierzcholek_koncowy)
        return 0; //SPRAWDZENIE POPRAWNOŚCI WIERZCHOŁKÓW

//    unsigned int** Matrix;
//    switch(type){
//        case distances:
//            Matrix = Graf.macierz_przyleglosci;
//            break;

//        case attractiveness:
//            Matrix = (unsigned int **)Graf.macierz_betonu;
//            break;

//        case profile:
//            Matrix =(unsigned int **)Graf.macierz_wysokosci;
//            break;

//    }


    unsigned int * T; //wektor wartości tymczasowych
    unsigned int * P; //wektor wartości ustalonych
    int * Previous;   //wektor ustalenie poprzednika dla danego wierzchołka
    T = new unsigned int [Graf.liczba_wierzcholkow];
    P = new unsigned int [Graf.liczba_wierzcholkow];
    Previous = new int [Graf.liczba_wierzcholkow];

    for(int i=0; i < Graf.liczba_wierzcholkow; i++){
        T[i]=0;
        P[i]=0;
        Previous[i]=0;
    }

    P[this->wierzcholek_poczatkowy]=1000000; // zabezpieczenie od zmiany
    QVector<int> followers;
    int current;
    current = this->wierzcholek_poczatkowy;

    while(current!= this->wierzcholek_koncowy){
        followers = nastepniki(current);
        for (int i=0; i<followers.size(); i++){
            if(T[followers[i]]==0){
                Previous[followers[i]]=current;
                T[followers[i]]=T[current]+Graf.macierz_przyleglosci[current][followers[i]];
            }
            else{
                int old=T[followers[i]];
                T[followers[i]]=min(T[followers[i]], T[current]+Graf.macierz_przyleglosci[current][followers[i]]);
                if(old!=T[followers[i]])
                    Previous[followers[i]]=current;
            }
        }
        current=minimum(T, P, Graf.liczba_wierzcholkow);
        P[current]=T[current];
        followers.clear();
    }

    QVector<int> result = build_result(Previous,this->wierzcholek_poczatkowy,this->wierzcholek_koncowy);
    this->path_best = result;
    result.clear();

    int wynik =P[current];
    delete T;
    delete P;
    return wynik;

}

//ZBUDOWANIE ROZWIĄZANIA DO DIJKSTRY
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


//MINIMUM DLA DIJKSTRY
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

//MINIMUM UNIWERSALNE
int Trasa::minimum(QVector<int> temp){

    int min = temp.front();

    int i_min =0;

   for(int i=0; i<temp.size(); i++){

       if(abs(temp[i]) <= min){
           min=abs(temp[i]);
           i_min=i;
       }
   }

   return i_min;

}

//MINIMUM DO OCENY WYKLUCZEN
int Trasa::minimumMarks(QVector<int> temp){

    int min = 1000000;

    int i_min =0;
    int flag=1;

   for(int i=0; i<temp.size(); i++){

       if(Long_Term.contains(path_best[i])){
           flag++;
       }
       else{

           if(abs(temp[i])<min){
               min=abs(temp[i]);
               i_min=i;
           }
       }
   }

   if(flag == temp.size() || min==1000000){
       return flag;
   }
   else
       return i_min;

}


//WYZNACZANIE NASTĘPNIKÓW WIERZCHOŁKA W GRAFIE
QVector<int> Trasa::nastepniki(int x){
    QVector<int> wynik;

    for (int i=0;i<Graf.liczba_wierzcholkow;i++)
        if ( Graf.macierz_przyleglosci[x][i]!=0 )
            wynik.push_back(i);

    return(wynik);
}

int Trasa::maksimumMarks(QVector<int> temp){

    int max = 0;

    int i_max =0;
    int flag=1;

   for(int i=0; i<temp.size(); i++){

       if(Long_Term.contains(path_best[i])){
               flag++;
       }
       else if(temp[i]>max){
               max=temp[i];
               i_max=i;
       }

   }

   if(flag == temp.size() || max==0){
       return flag; //jeśli wszystkie były na liście zabronień
   }
   return i_max;    //zwraca indeks minimalnego elementu

}


//ALGORYTM DO WYZNACZANIA OTOCZENIA WYKLUCZANYCH KRAWĘDZI, ROZMIAR OTOCZENIA DECYDUJE O TYM JAK SZEROKO "SIĘ ROZGLĄDAMY"
//OTOCZENIE JEST LISTĄ WIERZCHOŁKÓW KTÓRE MOGA ZASTĄPIĆ TE PODANE DO ALGORYTMU, PRZY ZACHOWANIU CIĄGŁOŚCI ROZWIĄZANIA
QVector< QVector<int> > Trasa::otoczenie (QVector<int> wykluczenie, int rozmiar, int l_krawedzi){

    QVector<int> n_first =nastepniki(wykluczenie.front()); //NASTĘPNIKI PIERWSZEGO ELEMENTU
    QVector<int> n_last = nastepniki(wykluczenie.back());  //NASTĘPNIKI OSTATNIEGO ELEMENTU
    QVector< QVector<int> >wynik;
    QVector<int> line;


    if(l_krawedzi<3){   //ograniczenie na liczbę krawędzi wykluczanych

            QVector<int> kand_1; //wierzchołki będące wspólnymi następnikami końcowej i początkowej krawędzi
            set_intersection (n_first.begin(), n_first.end(), n_last.begin(), n_last.end(), std::back_inserter(kand_1));

            if(l_krawedzi == 2){

                    //wstawienie alternatywnych tras składających się z 2-ch wierzchołków
                if(n_last.contains(wykluczenie.front())){
                    line.push_back(wykluczenie.front());
                    line.push_back(wykluczenie.back());
                    wynik.push_back(line);
                    line.clear();
                }
            }
                    //wstawienie alternatywnych tras składających się z 3-ch wierzchołków
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
            if(rozmiar == 1) return wynik; //jesli rozmiar był równy 1

                //wstawienie alternatywnych tras składających się z 4-ch wierzchołków
            QVector<int> kand_2;          
            for(int i=0 ; i<n_first.size(); ++i){
                 kand_2=nastepniki(n_first[i]);
                 set_intersection (kand_2.begin(), kand_2.end(), n_last.begin(), n_last.end(), std::back_inserter(kand_1));
                 //kand_1 jest teraz wspólnymi następnikami i-tego następnika wierzchołka początkowego i wierzchołka końcowego

                 for(int j=0 ; j < kand_1.size(); ++j) {
                     if(this->path_best.contains(kand_1[j]) == false && this->path_best.contains(n_first[i]) == false){

                         line.push_back(wykluczenie.front());
                         line.push_back(n_first[i]);
                         line.push_back(kand_1[j]);
                         line.push_back(wykluczenie.back());
                         wynik.push_back(line);
                         line.clear();
                     }

                 }

                 kand_1.clear();
                 kand_2.clear();
            }

            if(rozmiar==2) return wynik;



    }

    return wynik;
}

//FUNKCJA UŻYWANA DO WYŚWIETLANIA HISTORII TRAS
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


//PIERWSZA WERSJA ALGORYTMU BAZUJĄCA NA SAMYCH ODLEGŁOŚCIACH
void Trasa::algorithm_1(int ile_wykluczac){

    iteracje=0;

    int acc =this->dijkstra(distances); // jesli zwraca zero to brak rozwiazan lub unvalid edges
    if(acc==0 || path_best.size() < ile_wykluczac+1) {//|| path_best.size() < 4)
        kryterium_stopu="STOP 7 - niepoprawne wierzchołki krańcowe. ";
        calc_distance();
        calc_attractiveness();
        calc_profile();
        calc_funkcja_celu();
        path_all.push_back(path_best);
        return;
    }
    acc= this->wanted_distance - acc;


    QVector<int> Marks; //tablica oceny wszystkich potencjalnych wykluczeń
    QVector< QVector<int> > Potencials; //zbior tras do włączenia potencjalnie do trasy

    //aktualizacja listy rozwiązań
    this->f_distance.push_back(acc);
    this->path_all.push_back(this->path_best);
    calc_attractiveness();
    calc_profile();
    calc_funkcja_celu();

    if(acc<0){
        kryterium_stopu="STOP 6 - chciwy użytkownik. ";
        return;
    }


    int znacznik_zmian=0;

//GŁÓWNA PĘTLA ALGORYTMU
    while( acc!= 0 && iteracje<MAX_ITERACJI && znacznik_zmian<MAX_BRAK_POPRAW){

        Marks.clear();
        //stworzenie tablicy wartości dla poszczegolnych wykluczen potencjalnych
        for(int i=0; i<this->path_best.size()-ile_wykluczac; i++){
            Marks.push_back(this->calc_distance(i, i+ile_wykluczac));
        }

        iteracje++;

        int wyklucz;
        //KRYTERIUM WYKLUCZEŃ ZALEŻNE  OD AKTUALNEJ WARTOŚCI FUNKCJI CELU
        if(acc<0){
            wyklucz=maksimumMarks(Marks);
        }
        else{
            wyklucz= minimumMarks(Marks); //wybranie najlepszego wykluczenia - najgorsze polepszenie f. celu
        }

        if(Long_Term.contains(path_best[wyklucz])){
            kryterium_stopu="STOP 4 - coś poszło nie tak. ";
            break;
        }

        if(wyklucz==Marks.size()){ //warunek na brak nastepnikow
            kryterium_stopu="STOP 5 - wykluczone wszystkie. ";
            break;
        }


        QVector<int> Wykluczenie; //zbudowanie vektora wykluczenia wysylanego do metody otoczenie
        for(int i=0; i<=ile_wykluczac;i++){
            Wykluczenie.push_back(this->path_best[wyklucz+i]);
        }

        Potencials = otoczenie(Wykluczenie, 2, ile_wykluczac); //wszystkie potencjalne zamienniki dla naszego wykluczenia
        Wykluczenie.clear();

        if(Potencials.size()==0){ //-warunek na brak otoczenia krawedzi
            Long_Term.push_back(path_best[wyklucz]);
            cerr<< endl<<path_best[wyklucz]<< ": taboo - brak otoczenia (0)"<<endl;
            continue;
        }

        QVector<int> Potencials_Marks; //ocena wszystkich zamienników pod względem polepszenia f. celu
            //wartość bezwzględna( ile brakuje + ile da wykluczenie - ile da zamiennik wykluczenia)
        for(int i=0; i< Potencials.size(); i++){
            Potencials_Marks.push_back(abs(acc + Marks[wyklucz]- this->calc_distance(Potencials[i])));
        }

        int best_index = minimum(Potencials_Marks); //wybieramy tą która najbardziej zblizy nam f. c. do zera

        if(best_index==Potencials.size()){
            Long_Term.push_back(path_best[wyklucz]);
            cerr<< endl<< path_best[wyklucz]<< ": taboo - zly dobor otoczenia(1)"<<endl;
            continue;
        }
        Potencials_Marks.clear();

        //zmieniamy aktualną wartość f. c.
        acc -= calc_distance(Potencials[best_index]);
        acc=acc + Marks[wyklucz];

//warunek dodania rozwiązania
        if(iteracje!=1 &&  abs(f_distance.back()) <= abs(acc)){ //jeśli pogorszenie
            Long_Term.push_back(path_best[wyklucz]);
            cerr<< path_best[wyklucz] << " :taboo - brak poprawy (2)"<<endl;
            znacznik_zmian++;

            if(f_distance.back()*acc >0){ //jeśli nie oscylujemy wokół - nie zmieniamy rozwiązania
                acc=f_distance.back();
                continue;
            }
        }
        else{       //jeśli poprawa
            znacznik_zmian=0;
        }


//blok do wstawiania nowych krawedzi
        if(ile_wykluczac == 1){
        for(int i=1; i!=Potencials[best_index].size()-1; i++){
            path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);

            //Marks[wyklucz+i-1]=this->calc_distance(wyklucz+i-ile_wykluczac, wyklucz+i);
            //Marks.insert(Marks.begin()+wyklucz+i,this->calc_distance(wyklucz+i, wyklucz+i+ile_wykluczac));
        }
    }
    else if(ile_wykluczac==2){
        for(int i=0; i!=Potencials[best_index].size()-1; i++){
            if(i==0 && Potencials[best_index].size()==2){
                path_best.erase(path_best.begin()+wyklucz+1);
                //Marks.erase(Marks.begin()+wyklucz+1);
                //Marks[wyklucz]=this->calc_distance(wyklucz, wyklucz+ile_wykluczac);
            }
            else if(i==1){
                path_best[wyklucz+1]=Potencials[best_index][i];
                //Marks[wyklucz]=this->calc_distance(wyklucz, wyklucz+ile_wykluczac);
                //if((wyklucz+1)<Marks.size()){
                    //Marks[wyklucz+1]=this->calc_distance(wyklucz+1, wyklucz+1+ile_wykluczac);
                //}

            }
            else if(i==2){
                path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);

                //                Marks[wyklucz+i-1]=this->calc_distance(wyklucz+i-1, wyklucz+i-1 +ile_wykluczac);
                //if((wyklucz+i-1)<Marks.size()){
                //                Marks.insert(Marks.begin()+wyklucz+i-1,this->calc_distance(wyklucz+i-1, wyklucz+i-1+ile_wykluczac));
                //}
                //else{
                //    Marks.push_back(this->calc_distance(wyklucz+i-1, wyklucz+i-1+ile_wykluczac));
                //}

            }
        }
    }

        Potencials.clear();

        //aktualizacja listy wykluczeń, wyrzucamy element jeśli jego otoczenie się zmieniło
        if(wyklucz>0)
            if(Long_Term.contains(path_best[wyklucz-1])){
               Long_Term.remove( Long_Term.indexOf(path_best[wyklucz-1]));
            }

        //aktualizacja rozwiązania
        this->f_distance.push_back(acc);
        this->path_all.push_back(this->path_best);
        calc_attractiveness();
        calc_profile();
        calc_funkcja_celu();

    }
//END OF MAIN LOOP

    // ustalenie optymalnej znalezionej wartości
    int minimo=minimum(this->f_distance);
    f_distance.push_back(f_distance[minimo]);
    f_profile.push_back(f_profile[minimo]);
    f_attractiveness.push_back((f_attractiveness[minimo]));
    funkcja_celu.push_back(funkcja_celu[minimo]);

    //określenie wystąpienia warunku stopu
    if(acc==0){
        kryterium_stopu= "STOP 1 - znaleziono rozwiazanie";
    }
    else if(znacznik_zmian == MAX_BRAK_POPRAW){
        kryterium_stopu= "STOP 2 - dlugi okres braku poprawy";
    }
    else if(iteracje == MAX_ITERACJI){
        kryterium_stopu= "STOP 3 - maksymalna liczba iteracji";
    }

}



void Trasa::algorithm_2(int ile_wykluczac){

    iteracje=0;
    int znacznik_zmian=0;
    QVector< QVector<int> > Potencials; //zbior tras do włączenia potencjalnie do trasy
    srand(time(NULL));

    //wyznaczenie trasy początkowej i zbadanie warunków
    int acc =this->dijkstra(distances); // jesli zwraca zero to brak rozwiazan lub unvalid edges

    if(acc==0 || path_best.size() < ile_wykluczac+1) {
        kryterium_stopu="STOP 7 - niepoprawne wierzchołki krańcowe. ";
        calc_distance();
        calc_attractiveness();
        calc_profile();
        calc_funkcja_celu();
        path_all.push_back(path_best);
        return;
    }

    if(acc<0){ //opcjonalne przy tym algorytmie
        kryterium_stopu="STOP 6 - CHCIWY UZYTKOWNIK";
        return;
    }

    //wprowdzenie rozwiązania początkowego
    calc_distance();
    calc_attractiveness();
    calc_profile();
    acc= calc_funkcja_celu();
    this->path_all.push_back(this->path_best);




//*** MAIN LOOP ***//
//Główna pętla algorytmu
    while( acc!= 0 && iteracje<MAX_ITERACJI && znacznik_zmian<MAX_BRAK_POPRAW){
        iteracje++;

        int wyklucz;

        if(Long_Term.size()>=(path_best.size()-ile_wykluczac)){ //warunek na niemożliwość wykluczeń
            kryterium_stopu="STOP 5 - wykluczone wszystkie";
            break;
        }

        //wybór wykluczenia, poprzez losowanie z dostępnych wierzchołków
        wyklucz= rand() % (path_best.size()-ile_wykluczac -Long_Term.size());

        //wyłuskanie indeksu wylosowanego wierzchołka
        int i_w=0;
        for(int j=wyklucz+1; j>0; i_w++){
                if(!Long_Term.contains(path_best[i_w])){
                --j;
            }
        }
        wyklucz=i_w-1;

        //sprawdzenie poprawności działania
        if(Long_Term.contains(path_best[wyklucz])){
            cerr<<"wykluczamy cos co jest zabronione!"<<endl;
            kryterium_stopu="STOP 4 - cos poszlo nie tak...";
            break;
        }

        QVector<int> Wykluczenie; //zbudowanie vektora wykluczenia wysylanego do metody otoczenie
        for(int i=0; i<=ile_wykluczac;i++){
            Wykluczenie.push_back(this->path_best[wyklucz+i]);
        }
        Potencials = otoczenie(Wykluczenie, 2, ile_wykluczac); //wszystkie potencjalne zamienniki dla naszego wykluczenia

        if(Potencials.size()==0){ // warunek na brak otoczenia krawedzi
            Long_Term.push_back(path_best[wyklucz]);
            cerr<< endl<<path_best[wyklucz]<< " :taboo - brak otoczenia (0)"<<endl;
            continue;
        }

        QVector<int> Potencials_Marks; //ocena wszystkich zamienników pod względem polepszenia f. celu
        //wartość bezwzględna( ile brakuje + ile da wykluczenie - ile da zamiennik wykluczenia)
        for(int i=0; i< Potencials.size(); i++){
            Potencials_Marks.push_back(abs(acc + calc_funkcja_celu(Wykluczenie)- this->calc_funkcja_celu(Potencials[i])));
        }

        int best_index = minimum(Potencials_Marks); //wybieramy tą która najbardziej zblizyla nam f. c. do zera


// BLOK ZMIANY ROZWIAZANIA
        if(ile_wykluczac == 1){//WARUNEK NA LICZBĘ WYKLUCZANYCH KRAWĘDZI
            for(int i=1; i!=Potencials[best_index].size()-1; i++){
                path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);

            }
        }
        else if(ile_wykluczac==2){//WARUNEK NA LICZBĘ WYKLUCZANYCH KRAWĘDZI
            for(int i=0; i!=Potencials[best_index].size()-1; i++){//INDEKSACJA PO ILOSCI WSTAWIANYCH NOWYCH DANYCH
                if(i==0 && Potencials[best_index].size()==2){ //JESLI WYCINAMY WIERZCHOLEK
                    path_best.erase(path_best.begin()+wyklucz+1);
                }
                else if(i==1){//JESLI ZAMIENIAMY WIERZCHOLEK
                    path_best[wyklucz+1]=Potencials[best_index][i];
                }
                else if(i==2){//JESLI DOSTAWIAMY WIERZCHOLEK
                    path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);
                }
            }
        }

        Potencials.clear();
        Wykluczenie.clear();
        Potencials_Marks.clear();

//AKTUALIZACJA ROZWIAZANIA I WEKTOROW ROZWIAZAN
        this->path_all.push_back(this->path_best);
        calc_attractiveness();
        calc_profile();
        calc_distance();
        acc=calc_funkcja_celu();

        //aktualizacja listy wykluczeń, wyrzucamy element jeśli jego otoczenie się zmieniło
        if(wyklucz>0)
            if(Long_Term.contains(path_best[wyklucz-1])){
               Long_Term.remove( Long_Term.indexOf(path_best[wyklucz-1]));
               cerr<<endl<<path_best[wyklucz-1]<<": untabooed"<<endl;
            }

//BLOK BADAJĄCY CHARAKTER ZMIAN I DECYDUJĄCY O LOSIE ROZWIĄZANIA
        if(iteracje!=1 &&  funkcja_celu[funkcja_celu.size()-2] <= acc){             //JESLI BRAK POPRAWY
            Long_Term.push_back(path_best[wyklucz]);
            cerr<< path_best[wyklucz] << " :taboo - brak poprawy (2)"<<endl;
            znacznik_zmian++;

            if(funkcja_celu[funkcja_celu.size()-3] < funkcja_celu[funkcja_celu.size()-2]){ //JESLI NIE OSCYLUJEMY W POBLIŻU
                //USUNIĘCIE ROZWIĄZANIA KTÓRE DŁUGO SIĘ ODDALA
                path_all.pop_back();
                f_distance.pop_back();
                funkcja_celu.pop_back();
                f_attractiveness.pop_back();
                f_profile.pop_back();
                path_best=path_all.back();
                acc=funkcja_celu.back();
            }
        }
        else {
            //JESLI POPRAWA
            znacznik_zmian=0;
        }
    }
//KONIEC PĘTLI GŁÓWNEJ

//USTALENIE NAJLEPSZEJ ZNALEZIONEJ WARTOŚCI
    int minimo=minimum(this->funkcja_celu);
    f_distance.push_back(f_distance[minimo]);
    f_profile.push_back(f_profile[minimo]);
    f_attractiveness.push_back((f_attractiveness[minimo]));
    funkcja_celu.push_back(funkcja_celu[minimo]);
    path_best=path_all[minimo];
    path_all.push_back(path_best);

//USTALENIE, KTÓRE KRYTERIUM STOPU ZADZIAŁAŁO
    if(acc==0){
        kryterium_stopu="STOP 1 - znaleziono rozwiazanie";
    }
    else if(znacznik_zmian == MAX_BRAK_POPRAW){
        kryterium_stopu="STOP 2 - dlugi okres braku poprawy";
    }
    else if(iteracje == MAX_ITERACJI){
        kryterium_stopu="STOP 3 - maksymalna liczba iteracji";
    }
}


void Trasa::algorithm_3(int ile_wykluczac){

    iteracje=0;
    int znacznik_zmian=0;
    QVector< QVector<int> > Potencials; //zbior tras do włączenia potencjalnie do trasy
    srand(time(NULL));

    //wyznaczenie trasy początkowej i zbadanie warunków
    int acc =this->dijkstra(distances); // jesli zwraca zero to brak rozwiazan lub unvalid edges

    if(acc==0 || path_best.size() < ile_wykluczac+1) {
        kryterium_stopu="STOP 7 - niepoprawne wierzchołki krańcowe. ";
        calc_distance();
        calc_attractiveness();
        calc_profile();
        calc_funkcja_celu();
        path_all.push_back(path_best);
        return;
    }

    if(acc<0){ //opcjonalne przy tym algorytmie
        kryterium_stopu="STOP 6 - CHCIWY UZYTKOWNIK";
        return;
    }

    //wprowdzenie rozwiązania początkowego
    calc_distance();
    calc_attractiveness();
    calc_profile();
    acc= calc_funkcja_celu();
    this->path_all.push_back(this->path_best);




//*** MAIN LOOP ***//
//Główna pętla algorytmu
    while( acc!= 0 && iteracje<MAX_ITERACJI && znacznik_zmian<MAX_BRAK_POPRAW){
        iteracje++;

        int wyklucz;

        if(Long_Term.size()>=(path_best.size()-ile_wykluczac)){ //warunek na niemożliwość wykluczeń
            kryterium_stopu="STOP 5 - wykluczone wszystkie";
            break;
        }

        //wybór wykluczenia, poprzez losowanie z dostępnych wierzchołków
        wyklucz= rand() % (path_best.size()-ile_wykluczac -Long_Term.size());

        //wyłuskanie indeksu wylosowanego wierzchołka
        int i_w=0;
        for(int j=wyklucz+1; j>0; i_w++){
                if(!Long_Term.contains(path_best[i_w])){
                --j;
            }
        }
        wyklucz=i_w-1;

        //sprawdzenie poprawności działania
        if(Long_Term.contains(path_best[wyklucz])){
            cerr<<"wykluczamy cos co jest zabronione!"<<endl;
            kryterium_stopu="STOP 4 - cos poszlo nie tak...";
            break;
        }

        QVector<int> Wykluczenie; //zbudowanie vektora wykluczenia wysylanego do metody otoczenie
        for(int i=0; i<=ile_wykluczac;i++){
            Wykluczenie.push_back(this->path_best[wyklucz+i]);
        }
        Potencials = otoczenie(Wykluczenie, 2, ile_wykluczac); //wszystkie potencjalne zamienniki dla naszego wykluczenia

        if(Potencials.size()==0){ // warunek na brak otoczenia krawedzi
            Long_Term.push_back(path_best[wyklucz]);
            cerr<< endl<<path_best[wyklucz]<< " :taboo - brak otoczenia (0)"<<endl;
            continue;
        }

        QVector<int> Potencials_Marks; //ocena wszystkich zamienników pod względem polepszenia f. celu
        //wartość bezwzględna( ile brakuje + ile da wykluczenie - ile da zamiennik wykluczenia)
        for(int i=0; i< Potencials.size(); i++){
            Potencials_Marks.push_back(abs(acc + calc_funkcja_celu(Wykluczenie)- this->calc_funkcja_celu(Potencials[i])));
        }

        int best_index = minimum(Potencials_Marks); //wybieramy tą która najbardziej zblizyla nam f. c. do zera


// BLOK ZMIANY ROZWIAZANIA
        if(ile_wykluczac == 1){//WARUNEK NA LICZBĘ WYKLUCZANYCH KRAWĘDZI
            for(int i=1; i!=Potencials[best_index].size()-1; i++){
                path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);

            }
        }
        else if(ile_wykluczac==2){//WARUNEK NA LICZBĘ WYKLUCZANYCH KRAWĘDZI
            for(int i=0; i!=Potencials[best_index].size()-1; i++){//INDEKSACJA PO ILOSCI WSTAWIANYCH NOWYCH DANYCH
                if(i==0 && Potencials[best_index].size()==2){ //JESLI WYCINAMY WIERZCHOLEK
                    path_best.erase(path_best.begin()+wyklucz+1);
                }
                else if(i==1){//JESLI ZAMIENIAMY WIERZCHOLEK
                    path_best[wyklucz+1]=Potencials[best_index][i];
                }
                else if(i==2){//JESLI DOSTAWIAMY WIERZCHOLEK
                    path_best.insert(path_best.begin()+wyklucz+i,Potencials[best_index][i]);
                }
            }
        }

        Potencials.clear();
        Wykluczenie.clear();
        Potencials_Marks.clear();

//AKTUALIZACJA ROZWIAZANIA I WEKTOROW ROZWIAZAN
        this->path_all.push_back(this->path_best);
        calc_attractiveness();
        calc_profile();
        calc_distance();
        acc=calc_funkcja_celu();

        //aktualizacja listy wykluczeń, wyrzucamy element jeśli jego otoczenie się zmieniło
        if(wyklucz>0)
            if(Long_Term.contains(path_best[wyklucz-1])){
               Long_Term.remove( Long_Term.indexOf(path_best[wyklucz-1]));
               cerr<<endl<<path_best[wyklucz-1]<<": untabooed"<<endl;
            }
//BLOK BADAJĄCY CHARAKTER ZMIAN I DECYDUJĄCY O LOSIE ROZWIĄZANIA
                if(iteracje!=1 &&  funkcja_celu[funkcja_celu.size()-2] <= acc){             //JESLI BRAK POPRAWY
                    Long_Term.push_back(path_best[wyklucz]);
                    cerr<< path_best[wyklucz] << " :taboo - brak poprawy (2)"<<endl;
                    znacznik_zmian++;

                    //USUNIĘCIE ROZWIĄZANIA KTÓRE SIĘ ODDALA
                    path_all.pop_back();
                    f_distance.pop_back();
                    funkcja_celu.pop_back();
                    f_attractiveness.pop_back();
                    f_profile.pop_back();
                    path_best=path_all.back();
                    acc=funkcja_celu.back();

                }
                else {
                    //JESLI POPRAWA
                    znacznik_zmian=0;
                }

    }
//KONIEC PĘTLI GŁÓWNEJ

//USTALENIE NAJLEPSZEJ ZNALEZIONEJ WARTOŚCI
    int minimo=minimum(this->funkcja_celu);
    f_distance.push_back(f_distance[minimo]);
    f_profile.push_back(f_profile[minimo]);
    f_attractiveness.push_back((f_attractiveness[minimo]));
    funkcja_celu.push_back(funkcja_celu[minimo]);
    path_best=path_all[minimo];
    path_all.push_back(path_best);


//USTALENIE, KTÓRE KRYTERIUM STOPU ZADZIAŁAŁO
    if(acc==0){
        kryterium_stopu="STOP 1 - znaleziono rozwiazanie";
    }
    else if(znacznik_zmian == MAX_BRAK_POPRAW){
        kryterium_stopu="STOP 2 - dlugi okres braku poprawy";
    }
    else if(iteracje == MAX_ITERACJI){
        kryterium_stopu="STOP 3 - maksymalna liczba iteracji";
    }
}

