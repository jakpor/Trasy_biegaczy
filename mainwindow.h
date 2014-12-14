#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
