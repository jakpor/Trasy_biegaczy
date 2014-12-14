#ifndef GENERATOR_H
#define GENERATOR_H
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <vector>
using namespace std;

class generator
{
private:
    string outFileName;
    int h;
    int w;
    int** macierz_wierzcholkow;
    int** wspolrzedneX;
    int** wspolrzedneY;
    unsigned int** macierz_przyleglosci;

public:
    generator();
    ~generator();
    void wypisz_macierz(int w, int h, int *macierz);
    unsigned int distance(int ax,int ay,int bx,int by);
};

#endif // GENERATOR_H
