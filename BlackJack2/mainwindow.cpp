#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "blackjack.h"
#include "player.h"
#include "simstats.h"

#include <QApplication>
#include <QDebug>
#include <QtCharts/QChart>
#include <QtCharts/QValueAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game(new blackjack())
    , nate(new Player(300, 1500))
    , turnCount(0)
    , simCount(0)
{
    ui->setupUi(this);
    ui->label->setText(QString("Player Money: $%1").arg(nate->getStack()));

    // Step 1: Create a QWidget to be the central widget
    QWidget *centralWidget = new QWidget(this);  // This will be the central widget

    // Step 4: Set the layout to the central widget
    centralWidget->setLayout(ui->horizontalLayout);

    // Step 5: Set the central widget in the QMainWindow
    setCentralWidget(centralWidget);

    for (Player::BettingStrategy strategy : Player::getAllStrategies()) {
        ui->strategyComboBox->addItem(Player::toString(strategy), QVariant::fromValue(static_cast<int>(strategy)));
    }

    // Set up the chart
    series = new QLineSeries();
    seriesList = new std::vector<QLineSeries*>;
    chart = new QChart();
    chartView = new QChartView(chart);

    // Set up stats
    stats = new SimStats(seriesList);

    // Configure the chart
    seriesList->push_back(series);

    chart->addSeries((*seriesList)[0]);
    chart->legend()->setVisible(false);

    chart->setTitle("Stack Over Time");
    chart->createDefaultAxes();

    chart->axes(Qt::Horizontal).first()->setTitleText("Turns");
    chart->axes(Qt::Vertical).first()->setTitleText("Player Stack");

    // Create shared X and Y axes
    // QValueAxis *xAxis = new QValueAxis();
    // QValueAxis *yAxis = new QValueAxis();

    // Set up axis titles (optional)
    // xAxis->setTitleText("Turns");
    // yAxis->setTitleText("Player Stack");

    // Add axes to the chart
    // chart->addAxis(xAxis, Qt::AlignBottom);
    // chart->addAxis(yAxis, Qt::AlignLeft);

    // Attach axes to the new series before adding to chart
    // series->attachAxis(xAxis);
    // series->attachAxis(yAxis);

    // Set the initial axis range (optional)
    // xAxis->setRange(0, 100); // Initially, show no data on the x-axis
    // yAxis->setRange(0, 300); // Initial stack range, can be adjusted dynamically

    // Add the chart view to the main window layout
    ui->verticalLayout->addWidget(chartView); // Assuming you have a `QVBoxLayout` in your UI

    // Set the initial stack value
    series->append(turnCount, nate->getStack());

    ui->resultsTable->setRowCount(4); // Set the number of rows
    ui->resultsTable->setColumnCount(2); // Set the number of columns

    ui->resultsTable->setItem(0, 0, new QTableWidgetItem("Simulations Completed:"));
    ui->resultsTable->setItem(1, 0, new QTableWidgetItem("Mean Hands Played:"));
    ui->resultsTable->setItem(2, 0, new QTableWidgetItem("Mean Return:"));
    ui->resultsTable->setItem(3, 0, new QTableWidgetItem("Total Return:"));
    ui->resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

MainWindow::~MainWindow()
{
    delete game;
    delete nate;
    delete series;
    delete newSeries;
    delete seriesList;
    delete stats;
    delete chart;
    delete chartView;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    // If turn count is not 0, we must've already simulated.
    if (turnCount) {
        nate->resetStack(300);
        turnCount = 0;
        ++simCount;

        // Check if simCount is within the valid range of seriesList
        if (simCount < seriesList->size()) {
            newSeries = (*seriesList)[simCount];  // Use existing series
        } else {
            // Create a new series if necessary
            newSeries = new QLineSeries();
            seriesList->push_back(newSeries);
        }

        // Add series to chart
        chart->addSeries(newSeries);

        // Attach the new series to the default axes created by the chart
        newSeries->attachAxis(chart->axes(Qt::Horizontal).first());
        newSeries->attachAxis(chart->axes(Qt::Vertical).first());

        // Append the initial point
        newSeries->append(turnCount, nate->getStack());

        chart->update(); // Update the chart view
    }

    // Main game loop (simulate bets and adjust stack)
    while (nate->determineBet(25) > 0) {
        double bet = nate->determineBet(25);
        bet = game->playHand(bet); // Play a hand
        nate->adjustStack(bet);
        ++turnCount; // Increment turn counter
        updateChart(); // Update the chart with the new stack value

        ui->label->setText(QString("Player Money: $%1").arg(nate->getStack()));

        QApplication::processEvents();
    }

    // Update the Table with summarized results
    // qDebug() << "Size of seriesList: " << seriesList->size();
    updateResults();
}

void MainWindow::updateChart() {
    double currentStack = nate->getStack();

    // Append the new data to the correct series
    if (simCount < seriesList->size()) {
        (*seriesList)[simCount]->append(turnCount, currentStack);
    }

    // Get the current vertical axis and its current range
    QValueAxis *yAxis = qobject_cast<QValueAxis *>(chart->axes(Qt::Vertical).first());
    QValueAxis *xAxis = qobject_cast<QValueAxis *>(chart->axes(Qt::Horizontal).first());

    // Optionally, adjust the axes dynamically
    // chart->axes(Qt::Horizontal).first()->setRange(0, turnCount);

    if (xAxis) {
        // Get the current upper range of the vertical axis
        double currentRightmostRange = xAxis->max();

        // Update the range only if the currentStack exceeds the current upper range
        if (turnCount > currentRightmostRange) {
            xAxis->setMax(turnCount); // Increase by 10%, adjust as needed
        }
    }

    if (yAxis) {
        // Get the current upper range of the vertical axis
        double currentUpperRange = yAxis->max();

        // Update the range only if the currentStack exceeds the current upper range
        if (currentStack > currentUpperRange) {
            yAxis->setMax(currentStack * 1.1); // Increase by 10%, adjust as needed
        }
    }
}

void MainWindow::on_strategyComboBox_currentIndexChanged(int index)
{
    //QVariant selectedValue = strategyComboBox->itemData(strategyComboBox->currentIndex());
    // Player::BettingStrategy selectedStrategy = static_cast<Player::BettingStrategy>(selectedValue.toInt());
    Player::BettingStrategy selectedStrategy = static_cast<Player::BettingStrategy>(index);
    nate->setStrategy(selectedStrategy);
}

void MainWindow::updateResults(){
    double avgProfit = stats->calcMeanReturn();

    numToTable(0, simCount + 1);
    numToTable(1, stats->calcMeanHandsPlayed());
    numToTable(2, avgProfit);
    numToTable(3, (simCount + 1) * avgProfit);

}

void MainWindow::numToTable(int row, double value){
    // Convert the double to a QString
    QString valueStr = QString::number(value);

    // Create a QTableWidgetItem with the string value
    QTableWidgetItem* item = new QTableWidgetItem(valueStr);

    // update correct row in table
    ui->resultsTable->setItem(row, 1, item);
}
