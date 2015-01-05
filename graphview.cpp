#include "graphview.h"
#include "ui_graphview.h"
#include <QtGui>
#include <QColor>
#include "iostream"

using namespace std;

GraphView::GraphView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphView)
{
    ui->setupUi(this);
    gr = Graph();
}

GraphView::~GraphView()
{
    delete ui;
}


void GraphView::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint( QPainter::Antialiasing, true );
    QPen pen( Qt::black, 1 );

    //painter.drawLines(gr.lista_krawedzi,gr.liczba_krawedzi); //rysuje wszystkie krawędzie naraz - może się przydać kiedyś...

    /** krawędzie + beton **/
    for(int i = 0; i<(gr.liczba_krawedzi); i++){
        if(gr.lista_betonu[i]==1){
            pen.setStyle(Qt::DotLine);
        }
        else{
            pen.setStyle(Qt::SolidLine);
        }
        painter.setPen(pen);
        painter.drawLine(gr.lista_krawedzi[i]);
    }

    /** dobór promienia kółek **/
    int r = 5;
    if(gr.szerokosc_grafu<15){
        r = 20-gr.szerokosc_grafu;
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
        for(int i = 0; i<(gr.liczba_wierzcholkow); i++){
            //zielone - minimum, czerwone - maksimum
            painter.setBrush(QColor::fromHsv(((120/(gr.min_wysokosc-gr.max_wysokosc))*gr.lista_wysokosci[i]-(120/(gr.min_wysokosc-gr.max_wysokosc)*gr.max_wysokosc)),255, 255, 255)); //H, S, V, A (przezroczystość), H - 0-360, SVA - 0-255
            painter.drawEllipse(gr.lista_wierzcholkow[i],r,r);
            painter.drawText(QPoint(gr.lista_wierzcholkow[i].x()-r/2, gr.lista_wierzcholkow[i].y()+r/2),QString::number(i)); //cyferki
        }

    painter.end();
}
