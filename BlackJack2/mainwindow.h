#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include <vector>
// #include <QtCharts/QLineSeries>
// #include <QtCharts/QChart>
// #include <QtCharts/QChartView>

#include "blackjack.h"
#include "player.h"
#include "simstats.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void updateChart();
    void updateResults();
    void numToTable(int row, double value);

    void on_strategyComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    blackjack *game;
    Player *nate;
    QLineSeries *series;     // Series to hold data points
    QLineSeries *newSeries;
    std::vector<QLineSeries*>* seriesList; // vector to hold multiple series
    QChart *chart;           // Chart to display
    QChartView *chartView;   // View for the chart
    SimStats *stats;
    int turnCount;
    int simCount;
};
#endif // MAINWINDOW_H
