#ifndef SIMSTATS_H
#define SIMSTATS_H

#include <vector>
#include <QLineSeries>

class SimStats
{
public:
    SimStats(const std::vector<QLineSeries*>* seriesList);

    double calcMeanHandsPlayed();
    double calcMeanReturn();

private:
    const std::vector<QLineSeries*>* seriesList;
};

#endif // SIMSTATS_H
