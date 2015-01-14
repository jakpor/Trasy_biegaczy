#ifndef TRASA
#define TRASA

#include "graph.h"
//#include <vector>

#define INF 0
enum kryterium{distances=1, attractiveness=2, profile=3};
/************ Clasa rozwiązania***********/
class Trasa{
public:
    /************ Pola ************/
    Graph Graf;
    //lista wynik;
    // zbior krawędzi dołączonych do rozwiązania
    int wierzcholek_poczatkowy; //czy będzie to zrobione w tej postaci??
    int wierzcholek_koncowy;

    //aktualnie najlepsza trasa
    QVector <int> path_best;
    //cała pamięć tras
    QVector < QVector < int > > path_all;
    QVector <int> Long_Term;

    // Mnożniki poszczegolnych parametrów
    float w_distance;
    float w_attractiveness;
    float w_profile;

    int wanted_distance;
    int wanted_attractiveness;
    int wanted_profile;

    // ocena trasy wg poszczególnych kryteriów - ostatnia wartość
    QVector <int> f_distance;
    QVector <int> f_attractiveness ;
    QVector <int> f_profile;
    QVector <int> funkcja_celu;

    //potrzebuję tego do kolorowania najczęściej używanych tras
    QVector <int *> historia_tras;

    // ocena wg kryteriów zbiorowych
    //vector<??> wykluczenia // krótko, średnio i długoterminowe

    // enum umożliwiający słowne wybieranie ocenianego
    // kryterium w metodach

    /************ Metody ************/
    Trasa();
    Trasa(Graph Graf_in);
    Trasa(int start, int end);
    ~Trasa();
    Trasa(Trasa & trasa); //konstruktor kopiujący
    void copy_graf(Graph graf_in);
    void clear_result();

//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)
    int calc_funkcja_celu(QVector<int> odcinek);
    int calc_distance(QVector<int> odcinek);
    int calc_attractiveness(QVector<int> odcinek);
    int calc_profile(QVector<int> odcinek);
//calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)
    int calc_funkcja_celu();
    int calc_distance();
    int calc_attractiveness();
    int calc_profile();
    //calc_<name> dobre dla liczenia po raz pierwszy f() lub dla odcinka f(odcinek)
        int calc_funkcja_celu(int s, int e);
        int calc_distance(int s, int e);
        int calc_attractiveness(int s, int e);
        int calc_profile(int s, int e);

    void set_parameters(int p_distance, int p_attractiveness, int p_profile);
    void set_edges(int start, int end);
    void set_wanted(int dist, int att, int prof);
    //odpalaj po wykonaniu całego algorytmu, ale przed wizualizacją
    void aktualizuj_historie_tras();

    // tworzenie pustego rozwiązania
        //trasa(wierzcholek);
    // tworzenie rozwiązania przechodzącego przez dany wierzchołek
        //trasa(trasa);
    // nie wiem czy będzie potrzebna, można stworzyć coś o podobnej konstrukcji
    // do wykorzystania przy implementacji wykluczeń
        //lista/trasa rozwizanie_poczatkowe(){
    //generowanie rozwiązania począktkowego przy założeniu losowego rozwiązania
        //wierzcholek= rand();
        //trasa::trasa(wiercholek)

    // inna wersja rozważa wywołanie tej funkcji z argumentem mówiącym
    //czy generujemy randomową trasę, czy trasę minimalną

    int dijkstra(kryterium type);
    // uzywana w kontruktorze
     QVector<int> nastepniki(int x);
     int minimum(unsigned int * temp,unsigned int * perm, int size);
     int minimum(QVector<int> temp);
     int minimumMarks(QVector<int> temp);
     int maksimum(QVector<int> temp);
     QVector<int> build_result(int * history, int start, int end);
         //wyznaczenie nastepnikow wierzcholka x, n -rozmiar macierzy A


    /****** Metody algorytmu *******/
     void algorithm_1(int ile_wykluczac);
     void algorithm_2(int ile_wykluczac);
        //lista/trasa skroc(kryterium, wykluczenie);
        //lista/trasa wydluz(kryterium, wkluczenie);
        //wykluczenie/ void ocena_trasy(kryterium);
    // zmienia tablice wykluczeń lub zwraca wykluczenie, lub wywołuje
    // bezposrednio metody skroc, wydluz
QVector< QVector<int> > otoczenie (QVector<int> wykluczenie, int rozmiar, int l_krawedzi);
    // wyznacza otoczenie rozwiązania, w kierunku którego chcemy
    //podążać i wybrać z niego rozwiązanie
        //bool kryterium_stopu( argumenty do kryt stopu);
    // do ustalenia dzialanie tej metody
        //wykluczenie wybor_wykluczenia(kryterium, ...); // ...
};




#endif // TRASA

