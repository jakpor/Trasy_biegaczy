#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "generator.h"
#include "QString"
#include "trasa.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graphview.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_heightSlider_valueChanged(int value)
{
    ui->heightBox->setValue(value);
}
void MainWindow::on_heightBox_valueChanged(int value)
{
    ui->heightSlider->setValue(value);
}

void MainWindow::on_percentageSlider_valueChanged(int value)
{
    ui->percentageBox->setValue(value);
}
void MainWindow::on_percentageBox_valueChanged(int value)
{
    ui->percentageSlider->setValue(value);
}

void MainWindow::on_widthSlider_valueChanged(int value)
{
    ui->widthBox->setValue(value);
}
void MainWindow::on_widthBox_valueChanged(int value)
{
    ui->widthSlider->setValue(value);
}


void MainWindow::on_createGraphButton_clicked()
{
    /*
    generator a = generator(ui->filenameOutEdit->text().toStdString(),ui->heightBox->value(),ui->widthBox->value(),50,20,800,600,
              ui->cubeBox->isChecked(),ui->percentageBox->value(),ui->pionowoBox->isChecked(),
              ui->poziomoBox->isChecked(),ui->skos1Box->isChecked(),ui->skos2Box->isChecked());
    */
    //delete &g;

    int betonowosc=ui->betonBox->currentIndex(); //0-losowo, 1-pion+poziom
    int profil=ui->heightProfileBox->currentIndex(); //0-losowo, 1- góra, 2-dolina


    graphview.gr.create_graph(ui->filenameOutEdit->text().toStdString(),ui->heightBox->value(),ui->widthBox->value(),50,20,800,600,
                   ui->cubeBox->isChecked(),ui->percentageBox->value(),ui->pionowoBox->isChecked(),
                   ui->poziomoBox->isChecked(),ui->skos1Box->isChecked(),ui->skos2Box->isChecked(),betonowosc, profil); //wersja do algorytmu (z założenia)...
    graphview.gr.save_graph(ui->filenameOutEdit->text().toStdString());


    //graphview.gr = g.copy_graph(); //wersja do rysowania
    graphview.repaint();
    //cout<<graphview.gr.liczba_krawedzi;

}

void MainWindow::on_countButton_clicked(){
    Trasa a;
    int r;
//    cout<<a.dijkstra(ui->start->text().toInt(),ui->end->text().toInt(),graphview.gr,distances)<<endl;
    r=a.dijkstra(ui->start->text().toInt(),ui->end->text().toInt(),graphview.gr,distances);
    cout<<r<<endl;
    QString answear = QString::number(r);

    ui->result->setText(answear);
}

//przycisk zamknięcia - zamykanie okna głównego jest zrealizowane w Designerze (łączenie slotów). To zamyka wszyskie poboczne okienka
void MainWindow::on_closeButton_clicked()
{
    graphview.close();
}
