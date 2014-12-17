#include "graphview.h"
#include "ui_graphview.h"
#include <QtGui>
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
    painter.setPen( QPen( Qt::black, 1 ) );

    //QPointF p1 = QPointF( 750, 20 ); //600,0
    //QPointF p2 = QPointF( 20, 550); //600,800
    //QPointF p2 = QPointF( 20, 50);
    //painter.drawLine(p1,p2);
    //QLine line;
    //cerr<<'b';

    for(int i = 0; i<(gr.liczba_krawedzi); i++){
        painter.drawLine(gr.lista_krawedzi[i]);
    }
    painter.end();
}
