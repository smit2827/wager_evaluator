#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <QString>

class Player
{
public:
    Player(double stack, double goal);

    enum class BettingStrategy{
        nathan,
        broke,
        streaks,
        proportional
    };

    static std::vector<BettingStrategy> getAllStrategies();
    static QString toString(BettingStrategy strategy);

    double getStack();

    void adjustStack(double moneyIn);

    void resetStack(double initialStack);

    void setStrategy(BettingStrategy selectedStrategy);

    double determineBet(double minBet = 25);
private:
    double stack;
    double highestStack;
    double goal;
    int streak;
    BettingStrategy currentStrategy = BettingStrategy::nathan;
};

#endif // PLAYER_H
