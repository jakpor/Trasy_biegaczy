#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "generator.h"
#include "QString"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    g = Graph();
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
    generator a = generator(ui->filenameOutEdit->text().toStdString(),ui->heightBox->value(),ui->widthBox->value(),50,20,800,600,
              ui->cubeBox->isChecked(),ui->percentageBox->value(),ui->pionowoBox->isChecked(),
              ui->poziomoBox->isChecked(),ui->skos1Box->isChecked(),ui->skos2Box->isChecked());
    delete &g;
    g = a.create_graph(); //wersja do algorytmu (z założenia)...
    graphview.gr = g.copy_graph(); //wersja do rysowania
    graphview.repaint();
    cout<<graphview.gr.liczba_krawedzi;
}
