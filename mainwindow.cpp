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

void MainWindow::on_widthSlider_valueChanged(int value)
{
    ui->widthBox->setValue(value);
}
void MainWindow::on_widthBox_valueChanged(int value)
{
    ui->widthSlider->setValue(value);
}

void MainWindow::on_odlegloscSlider_valueChanged(int value)
{
    ui->odlegloscBox->setValue(value);
}
void MainWindow::on_odlegloscBox_valueChanged(int value)
{
    ui->odlegloscSlider->setValue(value);
}

void MainWindow::on_betonowoscSlider_valueChanged(int value)
{
    ui->betonowoscBox->setValue(value);
}
void MainWindow::on_betonowoscBox_valueChanged(int value)
{
    ui->betonowoscSlider->setValue(value);
}

void MainWindow::on_wysokoscSlider_valueChanged(int value)
{
    ui->wysokoscBox->setValue(value);
}
void MainWindow::on_wysokoscBox_valueChanged(int value)
{
    ui->wysokoscSlider->setValue(value);
}



/** Przycisk "Wygeneruj Graf" **/
void MainWindow::on_createGraphButton_clicked()
{
    int betonowosc=ui->betonBox->currentIndex(); //0-losowo, 1-pion+poziom
    int profil=ui->heightProfileBox->currentIndex(); //0-losowo, 1- góra, 2-dolina, 3 - przełęcz

    graphview.gr.create_graph(ui->filenameOutEdit->text().toStdString(),ui->heightBox->value(),ui->widthBox->value(),50,20,800,600,
                   ui->cubeBox->isChecked(),50,ui->pionowoBox->isChecked(),
                   ui->poziomoBox->isChecked(),ui->skos1Box->isChecked(),ui->skos2Box->isChecked(),betonowosc, profil, ui->losowoscBox->isChecked());

    pathview.graph = graphview.gr.copy_graph(); //wersja do rysowania trasy
    pathview.repaint();
    graphview.repaint();
    ui->tabWidget->setCurrentIndex(1);

    pathview.trasa.clear_result(); // wyczyszczenie wyników ze starej trasy jesli zmiana mapy

}
/** Przycisk "Zapisz" **/
void MainWindow::on_saveButton_clicked(){
    graphview.gr.save_graph(ui->filenameOutEdit->text().toStdString());

}

/** Przycisk "Oblicz" w zakładce 2 **/
void MainWindow::on_countButton_clicked(){
    int r;
    //tu też czyszczę trasę
    pathview.trasa.clear_result();

    pathview.trasa.copy_graf(graphview.gr);
    pathview.trasa.set_edges(ui->start->text().toInt(),ui->end->text().toInt());
    pathview.trasa.set_parameters(ui->pOdlegloscBox->value(),ui->pBetonowoscBox->value(),ui->pWysokoscBox->value());
    pathview.trasa.set_wanted(ui->odlegloscBox->value(),ui->betonowoscBox->value(),ui->wysokoscBox->value());

    r=pathview.trasa.dijkstra(distances);
//    cout<<r<<endl;

    ui->result->setText(QString::number(r));

    r=pathview.trasa.calc_profile(pathview.trasa.path_best);
    ui->result_wysokosc->setText(QString::number(r));

    r=pathview.trasa.calc_attractiveness(pathview.trasa.path_best);

    ui->result_beton->setText(QString::number(r));

    r=pathview.trasa.calc_funkcja_celu(pathview.trasa.path_best);
    ui->funckja_celu->setText(QString::number(r));
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

    //COŚ TUTAJ NAPRAWIA WCZYTYWANIE
    int betonowosc=ui->betonBox->currentIndex(); //0-losowo, 1-pion+poziom
    int profil=ui->heightProfileBox->currentIndex(); //0-losowo, 1- góra, 2-dolina, 3 - przełęcz
    graphview.gr.create_graph("awaryjny",30,30,50,20,800,600,
                   ui->cubeBox->isChecked(),100,ui->pionowoBox->isChecked(),
                   ui->poziomoBox->isChecked(),ui->skos1Box->isChecked(),ui->skos2Box->isChecked(),betonowosc, profil, ui->losowoscBox->isChecked());

    graphview.gr.load_graph(ui->filenameInEdit->text().toStdString());

    //graphview.gr.debug();

    pathview.graph = graphview.gr.copy_graph();
    //pathview.graph.save_graph("zapis"); //wypisuję skopiowany

    pathview.repaint();
    graphview.repaint();
    ui->tabWidget->setCurrentIndex(1);
}

/** Przycisk "Przerysuj grafy" **/
void MainWindow::on_pushButton_clicked()
{
    pathview.repaint();
    graphview.repaint();
    setupplot1(ui->plotWidget_1);
    setupplot2(ui->plotWidget_2);
    setupplot3(ui->plotWidget_3);
    setupplot4(ui->plotWidget_4);
}



/** Wykresy **/


/** Wykres funkcji celu **/
void MainWindow::setupplot1(QCustomPlot *customPlot){
    // funkcja celu od iteracji
    QVector<double> x(pathview.trasa.funkcja_celu.size()), y(pathview.trasa.funkcja_celu.size()), z(pathview.trasa.funkcja_celu.size()); // initialize with entries 0..100
    int max = 0;
    int min = 0;
    int wartosc = 0;
    for (int i=0; i<pathview.trasa.funkcja_celu.size(); ++i)
    {
      wartosc = pathview.trasa.funkcja_celu.at(i);
      x[i] = i; // x goes from -1 to 1
      y[i] = wartosc;  //konwersja na double
      if(max<wartosc){
          max = wartosc;
      }
      if(min>wartosc){
          min = wartosc;
      }
      z[i] = 0; //wartość zadana - minimum
    }
    // create graph and assign data to it:
//    customPlot->clearGraphs();
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->addGraph();
    customPlot->graph(1)->setData(x, z);
    QPen pen( Qt::green, 1 );
    customPlot->graph(1)->setPen(pen);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Numer zmiany");
    customPlot->yAxis->setLabel("Funkcja celu");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, pathview.trasa.funkcja_celu.size());
    customPlot->yAxis->setRange(min-0.1*(max-min), max+ 0.1*(max-min));
    customPlot->replot();

}

/** Wykres odległości **/
void MainWindow::setupplot2(QCustomPlot *customPlot){
    // odległość od iteracji
    QVector<double> x(pathview.trasa.f_distance.size()), y(pathview.trasa.f_distance.size()), z(pathview.trasa.f_distance.size()); // initialize with entries 0..100
    int max = pathview.trasa.wanted_distance;
    int min = pathview.trasa.wanted_distance;
    int wartosc = 0;
    for (int i=0; i<pathview.trasa.f_distance.size(); ++i)
    {
      wartosc = pathview.trasa.wanted_distance - pathview.trasa.f_distance.at(i);
      x[i] = i; // x goes from -1 to 1
      y[i] = wartosc;  //konwersja na double
      if(max<wartosc){
          max = wartosc;
      }
      if(min>wartosc){
          min = wartosc;
      }
      z[i] = pathview.trasa.wanted_distance;
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->addGraph();
    customPlot->graph(1)->setData(x, z);
    QPen pen( Qt::green, 1 );
    customPlot->graph(1)->setPen(pen);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Numer zmiany");
    customPlot->yAxis->setLabel("Długość trasy");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, pathview.trasa.f_distance.size());
    customPlot->yAxis->setRange(min-0.1*(max-min), max+ 0.1*(max-min));
    customPlot->replot();
}
/** Wykres betonu **/
void MainWindow::setupplot3(QCustomPlot *customPlot){
    // betonowośc od iteracji
    QVector<double> x(pathview.trasa.f_attractiveness.size()), y(pathview.trasa.f_attractiveness.size()), z(pathview.trasa.f_attractiveness.size()); // initialize with entries 0..100
    int max = pathview.trasa.wanted_attractiveness;
    int min = pathview.trasa.wanted_attractiveness;
    int wartosc = 0;
    for (int i=0; i<pathview.trasa.f_attractiveness.size(); ++i)
    {
      wartosc = pathview.trasa.wanted_attractiveness - pathview.trasa.f_attractiveness.at(i);
      x[i] = i; // x goes from -1 to 1
      y[i] = wartosc;  //konwersja na double
      if(max<wartosc){
          max = wartosc;
      }
      if(min>wartosc){
          min = wartosc;
      }
      z[i] = pathview.trasa.wanted_attractiveness;
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->addGraph();
    customPlot->graph(1)->setData(x, z);
    QPen pen( Qt::green, 1 );
    customPlot->graph(1)->setPen(pen);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Numer zmiany");
    customPlot->yAxis->setLabel("Ilość betonu");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, pathview.trasa.f_attractiveness.size());
    customPlot->yAxis->setRange(min-0.1*(max-min), max+ 0.1*(max-min));
    customPlot->replot();
}

/** Wykres wysokości **/
void MainWindow::setupplot4(QCustomPlot *customPlot){
    //wysokość
    QVector<double> x(pathview.trasa.f_profile.size()), y(pathview.trasa.f_profile.size()), z(pathview.trasa.f_profile.size()); // initialize with entries 0..100
    int max = pathview.trasa.wanted_profile;
    int min = pathview.trasa.wanted_profile;
    int wartosc = 0;
    for (int i=0; i<pathview.trasa.f_profile.size(); ++i)
    {
      wartosc = pathview.trasa.wanted_profile - pathview.trasa.f_profile.at(i);
      x[i] = i; // x goes from -1 to 1
      y[i] = wartosc;  //konwersja na double
      if(max<wartosc){
          max = wartosc;
      }
      if(min>wartosc){
          min = wartosc;
      }
      z[i] = pathview.trasa.wanted_profile;
    }
    // create graph and assign data to it:
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->addGraph();
    customPlot->graph(1)->setData(x, z);
    QPen pen( Qt::green, 1 );
    customPlot->graph(1)->setPen(pen);
    // give the axes some labels:
    customPlot->xAxis->setLabel("Numer zmiany");
    customPlot->yAxis->setLabel("Zgodność z profilem");
    // set axes ranges, so we see all data:
    customPlot->xAxis->setRange(0, pathview.trasa.f_profile.size());
    if(min==max){
        min = min-1;
        max = max+1;
    }
    customPlot->yAxis->setRange(min-0.1*(max-min), max+ 0.1*(max-min));
    customPlot->replot();
}


/** Przycisk "Oblicz najkrótszą trasę!" **/
void MainWindow::on_liczButton_clicked(){
        int r;
        pathview.trasa.clear_result();
        pathview.trasa.copy_graf(graphview.gr);
        pathview.trasa.set_edges(ui->start->text().toInt(),ui->end->text().toInt());
        pathview.trasa.set_parameters(ui->pOdlegloscBox->value(),ui->pBetonowoscBox->value(),ui->pWysokoscBox->value());
        pathview.trasa.set_wanted(ui->odlegloscBox->value(),ui->betonowoscBox->value(),ui->wysokoscBox->value());
        pathview.trasa.MAX_ITERACJI=ui->iteracje->value();
        pathview.trasa.MAX_BRAK_POPRAW=ui->czekanie->value();

        int rozmiar_wykluczenia = ui->wykluczenieBox->value();
        if(ui->alg1->isChecked()){
            pathview.trasa.algorithm_1(rozmiar_wykluczenia);
        }
        else if(ui->alg2->isChecked()){
            pathview.trasa.algorithm_2(rozmiar_wykluczenia);
        }
        else if(ui->alg3->isChecked()){
             pathview.trasa.algorithm_3(rozmiar_wykluczenia);
        }

        //wypisz najlepszą trasę
//        cout<<"Koncowa trasa to: ";
//        for(int i =0; i<pathview.trasa.path_best.size(); i++){
//            cout<<pathview.trasa.path_best[i]<< " ";
//        }
//        cout<<endl;

        //wypisz wszystkie trasy:
        cout<<endl;
        for(int i=0; i<pathview.trasa.path_all.size(); i++){
            cout<<"Trasa nr "<<i << " f_distance= "<<pathview.trasa.f_distance[i]<<" : ";
            for(int j =0; j<pathview.trasa.path_all[i].size(); j++){
                         cout<<pathview.trasa.path_all[i][j]<<" ";
                    }
            cout<<endl;
        }


        r=pathview.trasa.f_distance.back();
        ui->result->setText(QString::number(r));
        r=pathview.trasa.f_profile.back();
        ui->result_wysokosc->setText(QString::number(r));
        r=pathview.trasa.f_attractiveness.back();
        ui->result_beton->setText(QString::number(r));
        r=pathview.trasa.funkcja_celu.back();
        ui->funckja_celu->setText(QString::number(r));

        ui->stop->setText(pathview.trasa.kryterium_stopu);
        ui->stop_i->setText(QString::number(pathview.trasa.iteracje));

        //po wykonaniu algorytmu aktualizuje historie
       // pathview.trasa.aktualizuj_historie_tras();

        setupplot1(ui->plotWidget_1);
        setupplot2(ui->plotWidget_2);
        setupplot3(ui->plotWidget_3);
        setupplot4(ui->plotWidget_4);
        ui->tabWidget->setCurrentIndex(3);

        //odkomentuj jak bedziesz pewna, że wszystko już jest na tip top i historia nie popsuje
        //po odkomentowaniu automatycznie będzie klikało nam na przycisk z historią - nie wyrzucajmy przycisku :)
        //ui->historyButton->click();
    }

void MainWindow::on_historyButton_clicked()
{
    //po wykonaniu algorytmu aktualizuje historie
   pathview.trasa.aktualizuj_historie_tras();
   pathview.repaint();
}

void MainWindow::on_pBetowoscSlider_valueChanged(int value)
{
    ui->pBetonowoscBox->setValue(value);
    if(value< 100 - ui->pWysokoscBox->value()){
        ui->pOdlegloscBox->setValue(100-value-ui->pWysokoscBox->value());
        ui->pOdlegloscSlider->setValue(ui->pOdlegloscBox->value());
    }
    else{
        ui->pOdlegloscBox->setValue(0);
        ui->pOdlegloscSlider->setValue(ui->pOdlegloscBox->value());
        ui->pWysokoscBox->setValue(100-value);
        ui->pWysokoscSlider->setValue(ui->pWysokoscBox->value());
    }
}


void MainWindow::on_pBetonowoscBox_valueChanged(int arg1)
{
    ui->pBetowoscSlider->setValue(arg1);
    if(arg1< 100 - ui->pWysokoscBox->value()){
        ui->pOdlegloscBox->setValue(100-arg1-ui->pWysokoscBox->value());
        ui->pOdlegloscSlider->setValue(ui->pOdlegloscBox->value());
    }
    else{
        ui->pOdlegloscBox->setValue(0);
        ui->pOdlegloscSlider->setValue(ui->pOdlegloscBox->value());
        ui->pWysokoscBox->setValue(100-arg1);
        ui->pWysokoscSlider->setValue(ui->pWysokoscBox->value());
    }
}



void MainWindow::on_pWysokoscSlider_valueChanged(int value)
{
    ui->pWysokoscBox->setValue(value);
    if(value< 100 - ui->pOdlegloscBox->value()){
        ui->pBetonowoscBox->setValue(100-value-ui->pOdlegloscBox->value());
        ui->pBetowoscSlider->setValue(ui->pBetonowoscBox->value());
        }
    else{
        ui->pBetonowoscBox->setValue(0);
        ui->pBetowoscSlider->setValue(ui->pBetonowoscBox->value());
        ui->pOdlegloscBox->setValue(100-value);
        ui->pOdlegloscSlider->setValue(ui->pOdlegloscBox->value());
    }
}



void MainWindow::on_pWysokoscBox_valueChanged(int arg1)
{
    ui->pWysokoscSlider->setValue(arg1);
    if(arg1< 100 - ui->pOdlegloscBox->value()){
        ui->pBetonowoscBox->setValue(100-arg1-ui->pOdlegloscBox->value());
        ui->pBetowoscSlider->setValue(ui->pBetonowoscBox->value());
        }
    else{
        ui->pBetonowoscBox->setValue(0);
        ui->pBetowoscSlider->setValue(ui->pBetonowoscBox->value());
        ui->pOdlegloscBox->setValue(100-arg1);
        ui->pOdlegloscSlider->setValue(ui->pOdlegloscBox->value());
    }
}



void MainWindow::on_pOdlegloscSlider_valueChanged(int value)
{
    ui->pOdlegloscBox->setValue(value);
    if(value< 100 - ui->pBetonowoscBox->value()){
        ui->pWysokoscBox->setValue(100-value-ui->pBetonowoscBox->value());
        ui->pWysokoscSlider->setValue(ui->pWysokoscBox->value());
        }
    else{
        ui->pWysokoscBox->setValue(0);
        ui->pWysokoscSlider->setValue(ui->pWysokoscBox->value());
        ui->pBetonowoscBox->setValue(100-value);
        ui->pBetowoscSlider->setValue(ui->pBetonowoscBox->value());
    }
}



void MainWindow::on_pOdlegloscBox_valueChanged(int arg1)
{
    ui->pOdlegloscSlider->setValue(arg1);
    if(arg1< 100 - ui->pBetonowoscBox->value()){
        ui->pWysokoscBox->setValue(100-arg1-ui->pBetonowoscBox->value());
        ui->pWysokoscSlider->setValue(ui->pWysokoscBox->value());
        }
    else{
        ui->pWysokoscBox->setValue(0);
        ui->pWysokoscSlider->setValue(ui->pWysokoscBox->value());
        ui->pBetonowoscBox->setValue(100-arg1);
        ui->pBetowoscSlider->setValue(ui->pBetonowoscBox->value());
    }
}
