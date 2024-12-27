#include "player.h"
#include <algorithm>
#include <vector>
#include <QString>

Player::Player(double stack, double goal)
    : stack(stack), goal(goal), highestStack(stack), streak(0){
}

double Player::getStack(){
    return stack;
}

void Player::adjustStack(double moneyIn){
    stack += moneyIn;

    // Player Memory - for strategies
    highestStack = std::max(stack, highestStack);
    if (moneyIn > 0) {
        // Win: Increment streak if not already on a winning streak
        streak = (streak >= 1) ? streak + 1 : 1;
    } else if (moneyIn < 0) {
        // Loss: Decrement streak if not already on a losing streak
        streak = (streak <= -1) ? streak - 1 : -1;
    }

}

void Player::resetStack(double initalStack){
    stack = highestStack = initalStack;

    // Player Memory - for strategies
    streak = 0;
}

std::vector<Player::BettingStrategy> Player::getAllStrategies(){
    return {
        BettingStrategy::nathan,
        BettingStrategy::broke,
        BettingStrategy::streaks,
        BettingStrategy::proportional
    };
}

QString Player::toString(BettingStrategy strategy){
    switch(strategy){
    case BettingStrategy::nathan: return "Nathan";
    case BettingStrategy::broke: return "Broke";
    case BettingStrategy::streaks: return "Streaks";
    case BettingStrategy::proportional: return "Proportional";
    default: return "Tell Nate to update toString()";
    }
}

void Player::setStrategy(BettingStrategy selectedStrategy) {
    currentStrategy = selectedStrategy;
}

double Player::determineBet(double minBet){
    // Enforce the minimum bet always (25 if not specified)
    if (stack < minBet || stack > goal){
        return 0;
    }

    switch (currentStrategy){

        // You're Nathan
        case BettingStrategy::nathan:{
            if (stack < minBet) {
                return 0; // below minimum bet

            } else if (stack < highestStack) {
                return std::min((highestStack + minBet) - stack, stack); // try to get back!

            } else if (stack > goal) {
                return 0; // goal achieved - go home

            } else {
                return 25; // bet the minimum
            }
        }
        case BettingStrategy::broke:{
            return minBet;
        }
        case BettingStrategy::streaks: {
            if (streak <= -1) {
                return static_cast<double>((-1 * 0.25 * minBet * streak) + minBet);
            }
            return static_cast<double>(minBet);
        }
        case BettingStrategy::proportional: {
            return std::max(minBet, stack*0.05);
        }

        // You're just some shmuck!
        default: {
            return minBet;
        }
    }
}
