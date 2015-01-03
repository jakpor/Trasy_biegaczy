#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "graphview.h"
#include "pathview.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_heightSlider_valueChanged(int value);
    void on_percentageSlider_valueChanged(int value);
    void on_widthSlider_valueChanged(int value);

    void on_heightBox_valueChanged(int value);
    void on_widthBox_valueChanged(int value);
    void on_percentageBox_valueChanged(int value);

    void on_createGraphButton_clicked();

    void on_closeButton_clicked();
    void on_countButton_clicked();

    void on_drawFromFileButton_clicked();

private:
    Ui::MainWindow *ui;
    GraphView graphview;
    PathView pathview;
};

#endif // MAINWINDOW_H
