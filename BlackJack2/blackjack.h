#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <vector>
#include <utility>
#include <random>

class blackjack
{
private:
    // RNG setup as member variables
    std::mt19937 gen;
    std::uniform_real_distribution<double> dist;

public:
    blackjack();

    double playHand(double betAmount);
};

#endif // BLACKJACK_H
