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

    //widoczna karta to generowanie grafu
    ui->tabWidget->setCurrentIndex(0);
    setupplot1(ui->plotWidget_1);
    setupplot2(ui->plotWidget_2);
    setupplot3(ui->plotWidget_3);
    setupplot4(ui->plotWidget_4);
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
                   ui->poziomoBox->isChecked(),ui->skos1Box->isChecked(),ui->skos2Box->isChecked(),betonowosc, profil, ui->losowoscBox->isChecked());

    pathview.graph = graphview.gr.copy_graph(); //wersja do rysowania trasy
    pathview.repaint();
    graphview.repaint();
    ui->tabWidget->setCurrentIndex(1);

}
/** Przycisk "Zapisz" **/ //po co to nam potrzebne, skoro jeśli już generuję graf, to od razu zapisuje go do plików?
void MainWindow::on_saveButton_clicked(){
    graphview.gr.save_graph(ui->filenameOutEdit->text().toStdString());

}

/** Przycisk "Oblicz" w zakładce 2 **/
void MainWindow::on_countButton_clicked(){
    int r;

    r=pathview.trasa.dijkstra(ui->start->text().toInt(),ui->end->text().toInt(),graphview.gr,distances);
//    cout<<r<<endl;

    ui->result->setText(QString::number(r));

    //tą linijkę trzeba umiescić gdzieś w algorytmie, bo teraz za każdym kliknięciem się dodaje do wektora...
    pathview.trasa.path_all.push_back(pathview.trasa.path_best);

//    //testowy drugi wektor - możesz odkomentować i zobaczyć jak działa historia tras
//    QVector <int> temp;
//    temp.append(0);
//    temp.append(1);
//    temp.append(2);
//    temp.append(3);
//    pathview.trasa.path_all.push_back(temp);


    //po wykonaniu algorytmu aktualizuje historie
    pathview.trasa.aktualizuj_historie_tras();
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

    graphview.gr.debug();

    pathview.graph = graphview.gr.copy_graph();
    pathview.graph.save_graph("zapis"); //wypisuję skopiowany

    pathview.repaint();
    graphview.repaint();
    ui->tabWidget->setCurrentIndex(1);
}

/** Przycisk "Przerysuj grafy" **/
void MainWindow::on_pushButton_clicked()
{
    pathview.repaint();
    graphview.repaint();
}



/** Wykresy **/

void MainWindow::setupplot1(QCustomPlot *customPlot){
    // funkcja celu od iteracji
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i; // x goes from -1 to 1
      y[i] = 30*sqrt(x[i]);  // let's plot a sqrt function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Iteracje");
    customPlot->yAxis->setLabel("Funkcja celu");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 100);
    customPlot->yAxis->setRange(0, 1000);
}

void MainWindow::setupplot2(QCustomPlot *customPlot){
    // odległość od iteracji
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i; // x goes from -1 to 1
      y[i] = 50*sqrt(x[i]);  // let's plot a sqrt function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Iteracje");
    customPlot->yAxis->setLabel("Funkcja celu");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 100);
    customPlot->yAxis->setRange(0, 1000);
}
void MainWindow::setupplot3(QCustomPlot *customPlot){
    // betonowośc od iteracji
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i; // x goes from -1 to 1
      y[i] = 20*sqrt(x[i]);  // let's plot a sqrt function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Iteracje");
    customPlot->yAxis->setLabel("Funkcja celu");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 100);
    customPlot->yAxis->setRange(0, 1000);
}
void MainWindow::setupplot4(QCustomPlot *customPlot){
    //wysokość
    QVector<double> x(101), y(101); // initialize with entries 0..100
    for (int i=0; i<101; ++i)
    {
      x[i] = i; // x goes from -1 to 1
      y[i] = 40*sqrt(x[i]);  // let's plot a sqrt function
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Iteracje");
    customPlot->yAxis->setLabel("Funkcja celu");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, 100);
    customPlot->yAxis->setRange(0, 1000);
}
