#include "blackjack.h"
#include <iostream>

// Constructor: initialize player stack and RNG
blackjack::blackjack()
    : gen(std::random_device{}()), dist(0.0, 1.0)
{}

// Function to simulate a hand and return whether the player won
double blackjack::playHand(double betAmount) {
    std::vector<std::pair<double, double>> winnings = {
        {-8, 0.00000019},
        {-7, 0.00000235},
        {-6, 0.00001785},
        {-5, 0.00008947},
        {-4, 0.00048248},
        {-3, 0.00207909},
        {-2, 0.04180923},
        {-1, 0.40171191},
        {-0.5, 0.04470705},
        {0, 0.08483290},
        {1, 0.31697909},
        {1.5, 0.04529632},
        {2, 0.05844299},
        {3, 0.00259645},
        {4, 0.00076323},
        {5, 0.00014491},
        {6, 0.00003774},
        {7, 0.00000609}
    };


    double randomValue = dist(gen); // Generate a random number between 0 and 1
    double cumulativeProbability = 0.0;

    // Find the value corresponding to the random value
    for (const auto& event : winnings) {
        cumulativeProbability += event.second; // Add the event probability to the cumulative sum
        if (randomValue <= cumulativeProbability) {
            return event.first * betAmount; // Return the value associated with the winnings
        }
    }

    // Fallback (should never happen if probabilities sum to 1)
    return 0.0;
}

