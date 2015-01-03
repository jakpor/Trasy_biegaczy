#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QString"
#include "trasa.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graphViewLayout->addWidget(&graphview);
    ui->pathViewLayout->addWidget(&pathview);
    //graphview.show();

    //widoczna karta to generowanie grafu
    ui->tabWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// slidery i zmienne do generowania grafu
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

/** Przycisk "Wygeneruj Graf" **/
void MainWindow::on_createGraphButton_clicked()
{
    int betonowosc=ui->betonBox->currentIndex(); //0-losowo, 1-pion+poziom
    int profil=ui->heightProfileBox->currentIndex(); //0-losowo, 1- góra, 2-dolina, 3 - przełęcz

    graphview.gr.create_graph(ui->filenameOutEdit->text().toStdString(),ui->heightBox->value(),ui->widthBox->value(),50,20,800,600,
                   ui->cubeBox->isChecked(),ui->percentageBox->value(),ui->pionowoBox->isChecked(),
                   ui->poziomoBox->isChecked(),ui->skos1Box->isChecked(),ui->skos2Box->isChecked(),betonowosc, profil);


    //graphview.gr.save_graph(ui->filenameOutEdit->text().toStdString());

    pathview.graph = graphview.gr.copy_graph(); //wersja do rysowania trasy
    pathview.repaint();
    graphview.repaint();
    ui->tabWidget->setCurrentIndex(1);

}

/** Przycisk "Oblicz" w zakładce 2 **/
void MainWindow::on_countButton_clicked(){
    int r;
//    cout<<a.dijkstra(ui->start->text().toInt(),ui->end->text().toInt(),graphview.gr,distances)<<endl;
    r=pathview.trasa.dijkstra(ui->start->text().toInt(),ui->end->text().toInt(),graphview.gr,distances);
    cout<<r<<endl;

    ui->result->setText(QString::number(r));
}

/** przycisk zamknięcia - zamykanie okna głównego jest zrealizowane w Designerze (łączenie slotów). To zamyka wszyskie poboczne okienka
 * */
void MainWindow::on_closeButton_clicked()
{
    graphview.close();
}

/** Przycisk "Wczytaj Graf" **/
void MainWindow::on_drawFromFileButton_clicked()
{
    graphview.gr.load_graph(ui->filenameInEdit->text().toStdString());
    //cerr<<graphview.gr.liczba_krawedzi<<endl;
    //cerr<<graphview.gr.liczba_wierzcholkow<<endl;

    graphview.gr.debug();
    //pathview.graph = graphview.gr.copy_graph();
    //pathview.repaint();
    graphview.repaint();
    ui->tabWidget->setCurrentIndex(1);
}

/** Przycisk "Przerysuj grafy" **/
void MainWindow::on_pushButton_clicked()
{
    pathview.repaint();
    graphview.repaint();
}
