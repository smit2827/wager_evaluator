#include "simstats.h"

#include <QDebug>

#include <vector>
#include <QtCharts>
#include <QtCharts/QLineSeries>


SimStats::SimStats(const std::vector<QLineSeries*>* seriesList)
    : seriesList(seriesList){}

double SimStats::calcMeanHandsPlayed(){
    double numSeries = 0;
    double sumHandsPlayed = 0;

    // Each series is a simulation
    for(const QLineSeries* series : *seriesList){
        if(series != nullptr){ //ensure not pointing to null
            sumHandsPlayed += series->points().isEmpty() ? 0 : series->points().size() - 1;
            numSeries++;
        }
    }

    return numSeries == 0 ? 0.0 : sumHandsPlayed / numSeries;
}

double SimStats::calcMeanReturn(){
    double numSeries = 0;
    double sumProfits = 0;

    // Each series is a simulation
    for(const QLineSeries* series : *seriesList){
        if(series != nullptr){ //ensure not pointing to null
            sumProfits += series->points().isEmpty() ? 0 : series->points().last().y() - series->points().first().y();
            numSeries++;
        }
    }

    return numSeries == 0 ? 0.0 : sumProfits / numSeries;
}
