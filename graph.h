#ifndef GRAPH_H
#define GRAPH_H


class graph
{
public:
    graph();
    graph(int** wierzcholki,unsigned int**  przyleglosci,int**  krawedzie);
    ~graph();
    int** lista_wierzcholkow;
    unsigned int** macierz_przyleglosci;
    int ** lista_krawedzi;
};

#endif // GRAPH_H
