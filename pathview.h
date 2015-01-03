#ifndef PATHVIEW_H
#define PATHVIEW_H

#include <QWidget>
#include "graph.h"
#include "trasa.h"

namespace Ui {
class PathView;
}

class PathView : public QWidget
{
    Q_OBJECT

public:
    explicit PathView(QWidget *parent = 0);
    ~PathView();
    Graph graph;
    Trasa trasa;

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::PathView *ui;
};

#endif // PATHVIEW_H
