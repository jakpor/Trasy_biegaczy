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
//    gr.sciezka.append(1);
//    gr.sciezka.append(2);
//    gr.sciezka.append(6);
//    gr.sciezka.append(5);
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

    //krawędzie + beton
    for(int i = 0; i<(gr.liczba_krawedzi); i++){
        if(gr.lista_betonu[i]==1){
            pen.setStyle(Qt::DotLine);
        }
        else{
            pen.setStyle(Qt::SolidLine);
        }
        painter.setPen(pen);
        painter.drawLine(gr.lista_krawedzi[i]);
            //testy
//            cout<<gr.lista_krawedzi[i].dx()<<"  "<< gr.lista_krawedzi[i].dy()<<endl;
//            for (int k = 0; k<gr.liczba_krawedzi; k++ ){
//                cout<<gr.lista_krawedzi[k].x1()<<" "<<gr.lista_krawedzi[k].y1()<<" "<<gr.lista_krawedzi[k].x2()<<" "<<gr.lista_krawedzi[k].y2()<<" "<<endl;
//            }
    }

    //aktualna trasa
    //gr.sciezka.fill(-1, 10);

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::red);
    pen.setWidth(5);
    painter.setPen(pen);
    for(int i = 0; i<(gr.sciezka.size()-1); i++){
        painter.drawLine(gr.lista_wierzcholkow[gr.sciezka.at(i)],gr.lista_wierzcholkow[gr.sciezka.at(i+1)]);
    }

    //dobór promienia kółek
    int r = 5;
    if(gr.szerokosc_grafu<15){
        r = 20-gr.szerokosc_grafu;
    }
    else{
        r =5;
    }
    //kółka - wierzchołki
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    pen.setColor(Qt::black);
    painter.setBrush(Qt::green);
    painter.setPen( pen );
        for(int i = 0; i<(gr.liczba_wierzcholkow); i++){
            //zielone - minimum, czerwone - maksimum
            painter.setBrush(QColor::fromHsv(((-120/(gr.max_wysokosc-gr.min_wysokosc))*gr.lista_wysokosci[i]+120/(1-gr.min_wysokosc/gr.max_wysokosc)),255, 255, 255)); //H, S, V, A (przezroczystość), H - 0-360, SVA - 0-255
            painter.drawEllipse(gr.lista_wierzcholkow[i],r,r);
            painter.drawText(QPoint(gr.lista_wierzcholkow[i].x()-r/2, gr.lista_wierzcholkow[i].y()+r/2),QString::number(i)); //cyferki
        }

    painter.end();
}
