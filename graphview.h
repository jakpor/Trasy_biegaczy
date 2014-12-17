#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>
#include "graph.h"

namespace Ui {
class GraphView;
}

class GraphView : public QWidget
{
    Q_OBJECT

public:
    explicit GraphView(QWidget *parent = 0);
    ~GraphView();
    Graph gr;

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::GraphView *ui;
};

#endif // GRAPHVIEW_H
