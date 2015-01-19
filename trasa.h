#ifndef TRASA
#define TRASA

#include "graph.h"
//#include <vector>

#define INF 0
// zmienne skalujące składowe funkcji celu tak by miały podobny priorytet
#define SKAL_PROFILE 35         //400
#define SKAL_ATTRACTIVENESS 30 //35

enum kryterium{distances=1, attractiveness=2, profile=3};
/************ Clasa rozwiązania***********/
class Trasa{
private:
    /************ Pola ************/
    Graph Graf;


    QVector <int> Long_Term; //wektor wykluczeń

    // Mnożniki poszczegolnych parametrów
    float w_distance;
    float w_attractiveness;
    float w_profile;

public:

    int wierzcholek_poczatkowy;
    int wierzcholek_koncowy;

    //wartości zadane
    int wanted_distance;
    int wanted_attractiveness;
    int wanted_profile;



    QVector <int> path_best;     //aktualna trasa
    QVector < QVector < int > > path_all;    //cała pamięć tras
    // ocena trasy wg poszczególnych kryteriów - ostatnia wartość
    QVector <int> f_distance;
    QVector <int> f_attractiveness ;
    QVector <int> f_profile;
    QVector <int> funkcja_celu;

    //potrzebuję tego do kolorowania najczęściej używanych tras
    QVector <int *> historia_tras;

    // parametry przeprowadzanego algorytmu
    int MAX_ITERACJI;
    int MAX_BRAK_POPRAW;
    QString kryterium_stopu;
    int iteracje;


    /************ Metody ************/
    Trasa();
    Trasa(Graph Graf_in);
    Trasa(int start, int end);
    ~Trasa();
    Trasa(Trasa & trasa); //konstruktor kopiujący
    void copy_graf(Graph graf_in);
    void clear_result();

//aktualizacja funkcji celu i poszczególnych składowych na podstawie aktualnego path_best
    int calc_funkcja_celu();
    int calc_distance();
    int calc_attractiveness();
    int calc_profile();

//oszacowanie wartości funkcji celu i poszczególnych składowych dla odcinka
    int calc_funkcja_celu(QVector<int> odcinek);
    int calc_distance(QVector<int> odcinek);
    int calc_attractiveness(QVector<int> odcinek);
    int calc_profile(QVector<int> odcinek);

//oszacowanie wartości funkcji celu i poszczególnych składowych dla fragmentu wektora path_best
    int calc_funkcja_celu(int s, int e);
    int calc_distance(int s, int e);
    int calc_attractiveness(int s, int e);
    int calc_profile(int s, int e);

//ustawianie parametrów algorytmu przed uruchomieniem
    void set_parameters(int p_distance, int p_attractiveness, int p_profile);
    void set_edges(int start, int end);
    void set_wanted(int dist, int att, int prof);
//aktualizacja historii tras do wyświetlenia po wykonaniu
    void aktualizuj_historie_tras();

    /****** Metody algorytmu *******/

     int dijkstra(kryterium type);                      //uzywana do generacji rozwiązania początkowego
     int minimum(unsigned int * temp,unsigned int * perm, int size); //minimum dla dijkstry
     QVector<int> build_result(int * history, int start, int end);   // budowanie rozwiązania algorytmu dijkstry

     QVector<int> nastepniki(int x);                    //wyznaczenie nastepnikow wierzcholka x
     int minimum(QVector<int> temp);                                 // minimum uniwersalne
     int minimumMarks(QVector<int> temp);                            // minimum dla oceny wyluczeń
     int maksimumMarks(QVector<int> temp);                           // maksimum dla oceny wyluczeń
     QVector< QVector<int> > otoczenie (QVector<int> wykluczenie, int rozmiar, int l_krawedzi);
     //otoczenie zabranianego fragmentu

    // poszczególne wersja algorytmu
     void algorithm_1(int ile_wykluczac);   // wyspecjalizowana do poczukiwania konkretnej odległości w trasie
     void algorithm_2(int ile_wykluczac);   //
     void algorithm_3(int ile_wykluczac);   //

};

#endif // TRASA

