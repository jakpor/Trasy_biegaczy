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

    /** aktualna trasa **/
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::red);
    pen.setWidth(5);
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
        for(int i = 0; i<(graph.liczba_wierzcholkow); i++){
            //zielone - minimum, czerwone - maksimum
            painter.setBrush(QColor::fromHsv(((120/(graph.min_wysokosc-graph.max_wysokosc))*graph.lista_wysokosci[i]-(120/(graph.min_wysokosc-graph.max_wysokosc)*graph.max_wysokosc)),255, 255, 255)); //H, S, V, A (przezroczystość), H - 0-360, SVA - 0-255
            painter.drawEllipse(graph.lista_wierzcholkow[i],r,r);
            painter.drawText(QPoint(graph.lista_wierzcholkow[i].x()-r/2, graph.lista_wierzcholkow[i].y()+r/2),QString::number(i)); //cyferki
        }

    painter.end();
}
