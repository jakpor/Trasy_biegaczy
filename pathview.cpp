#include "pathview.h"
#include "ui_pathview.h"
#include "trasa.h"

PathView::PathView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PathView)
{
    ui->setupUi(this);
    graph = Graph();
}

PathView::~PathView()
{
    delete ui;
}

void PathView::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint( QPainter::Antialiasing, true );
    QPen pen( Qt::black, 1 );

    //painter.drawLines(gr.lista_krawedzi,gr.liczba_krawedzi); //rysuje wszystkie krawędzie naraz - może się przydać kiedyś...

    /** krawędzie + beton **/
    for(int i = 0; i<(graph.liczba_krawedzi); i++){
        if(graph.lista_betonu[i]==1){
            pen.setStyle(Qt::DotLine);
        }
        else{
            pen.setStyle(Qt::SolidLine);
        }
        painter.setPen(pen);
        painter.drawLine(graph.lista_krawedzi[i]);
    }

    /** historia tras **/
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::green);
    pen.setWidth(7);
    painter.setPen(pen);
    int kolor = 0;
    for(int i = 1; i<(trasa.historia_tras.size()); i++){
        kolor = 255-trasa.historia_tras[i][2]*255/trasa.historia_tras[0][1];
        if((kolor<=255)&&(kolor>=0)){
            pen.setColor(QColor::fromHsv(120, 40, kolor , 255)); //H, S, V, A (przezroczystość), H - 0-360, SLA - 0-255
        }
        else{
            cerr<<"ERROR: Historia tras: Bledna wartosc koloru = "<<kolor<<". Indeks: "<<i<<", wartosc historii: "<<trasa.historia_tras[i][2]<<", rozmiar: "<< trasa.historia_tras.size()<<endl;
            pen.setColor(QColor::fromHsv(180, 255, 255 , 255)); //H, S, V, A (przezroczystość), H - 0-360, SLA - 0-255
        }
        painter.setPen(pen);
        painter.drawLine(graph.lista_wierzcholkow[trasa.historia_tras[i][0]],graph.lista_wierzcholkow[trasa.historia_tras[i][1]]);
    }

    /** aktualna trasa **/
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::red);
    pen.setWidth(4);
    painter.setPen(pen);
    for(int i = 0; i<(trasa.path_best.size()-1); i++){
        //używam funkcji at, bo jest szybsza i read only
        painter.drawLine(graph.lista_wierzcholkow[trasa.path_best.at(i)],graph.lista_wierzcholkow[trasa.path_best.at(i+1)]);
    }

    /** dobór promienia kółek **/
    int r = 5;
    if(graph.szerokosc_grafu<15){
        r = 20-graph.szerokosc_grafu;
    }
    else{
        r =5;
    }

    /** kółka = wierzchołki **/
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(Qt::black);
    painter.setBrush(Qt::green);
    painter.setPen( pen );
    if(graph.liczba_wierzcholkow<200){
        for(int i = 0; i<(graph.liczba_wierzcholkow); i++){
            //zielone - minimum, czerwone - maksimum
            if((graph.min_wysokosc-graph.max_wysokosc)!=0){
                kolor = ((120/(graph.min_wysokosc-graph.max_wysokosc))*graph.lista_wysokosci[i]-(120/(graph.min_wysokosc-graph.max_wysokosc)*graph.max_wysokosc));
            }
            else{
                kolor = 120;
            }
            painter.setBrush(QColor::fromHsv(kolor,255, 255, 255)); //H, S, V, A (przezroczystość), H - 0-360, SVA - 0-255
            painter.drawEllipse(graph.lista_wierzcholkow[i],r,r);
            painter.drawText(QPoint(graph.lista_wierzcholkow[i].x()-r/2, graph.lista_wierzcholkow[i].y()+r/2),QString::number(i)); //cyferki
        }
    }
    else{
        for(int i = 0; i<(graph.liczba_wierzcholkow); i++){
            //zielone - minimum, czerwone - maksimum
            if((graph.min_wysokosc-graph.max_wysokosc)!=0){
                kolor = ((120/(graph.min_wysokosc-graph.max_wysokosc))*graph.lista_wysokosci[i]-(120/(graph.min_wysokosc-graph.max_wysokosc)*graph.max_wysokosc));
            }
            else{
                kolor = 120;
            }
            painter.setBrush(QColor::fromHsv(kolor,255, 255, 255)); //H, S, V, A (przezroczystość), H - 0-360, SVA - 0-255
            painter.drawEllipse(graph.lista_wierzcholkow[i],r,r);
        }
    }

    /** Początek i koniec **/
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(3);
        pen.setColor(Qt::black);
        painter.setPen( pen );
        int a = trasa.wierzcholek_poczatkowy;
        painter.setBrush(QColor::fromHsv(((120/(graph.min_wysokosc-graph.max_wysokosc))*graph.lista_wysokosci[a]-(120/(graph.min_wysokosc-graph.max_wysokosc)*graph.max_wysokosc)),255, 255, 255)); //H, S, V, A (przezroczystość), H - 0-360, SVA - 0-255
        painter.drawEllipse(graph.lista_wierzcholkow[a],r,r);
        painter.drawText(QPoint(graph.lista_wierzcholkow[a].x()-r/2, graph.lista_wierzcholkow[a].y()+r/2),QString::number(a)); //cyferki

        a = trasa.wierzcholek_koncowy;
        painter.setBrush(QColor::fromHsv(((120/(graph.min_wysokosc-graph.max_wysokosc))*graph.lista_wysokosci[a]-(120/(graph.min_wysokosc-graph.max_wysokosc)*graph.max_wysokosc)),255, 255, 255)); //H, S, V, A (przezroczystość), H - 0-360, SVA - 0-255
        painter.drawEllipse(graph.lista_wierzcholkow[a],r,r);
        painter.drawText(QPoint(graph.lista_wierzcholkow[a].x()-r/2, graph.lista_wierzcholkow[a].y()+r/2),QString::number(a)); //cyferki

    painter.end();
}
