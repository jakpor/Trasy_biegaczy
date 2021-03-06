#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"
#include "graph.h"
#include "graphview.h"
#include "pathview.h"
#include "trasa.h"

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
    void on_widthSlider_valueChanged(int value);

    void on_heightBox_valueChanged(int value);
    void on_widthBox_valueChanged(int value);

    void on_createGraphButton_clicked();
    void on_saveButton_clicked();
    void on_closeButton_clicked();
    void on_countButton_clicked();
    void on_liczButton_clicked();

    void on_odlegloscSlider_valueChanged(int value);
    void on_odlegloscBox_valueChanged(int value);
    void on_betonowoscSlider_valueChanged(int value);
    void on_betonowoscBox_valueChanged(int value);
    void on_wysokoscSlider_valueChanged(int value) ;
    void on_wysokoscBox_valueChanged(int value);


    void on_drawFromFileButton_clicked();

    void on_pushButton_clicked();

    void on_historyButton_clicked();

    void on_pBetowoscSlider_valueChanged(int value);

    void on_pBetonowoscBox_valueChanged(int arg1);

    void on_pWysokoscSlider_valueChanged(int value);

    void on_pWysokoscBox_valueChanged(int arg1);

    void on_pOdlegloscSlider_valueChanged(int value);

    void on_pOdlegloscBox_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    GraphView graphview;
    PathView pathview;
    void setupplot1(QCustomPlot *customplot);
    void setupplot2(QCustomPlot *customplot);
    void setupplot3(QCustomPlot *customplot);
    void setupplot4(QCustomPlot *customplot);
};

#endif // MAINWINDOW_H
