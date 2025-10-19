#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <array>
#include <cstdint>
#include "cards.h"


namespace Player_Structure{

class PlayerSets{
public:

    // max amount of cards per player (e.g., 2 for Hold'em)
    static constexpr std::size_t MaxCardsPerPlayer = 2;

    // type aliases for Clarity 
    using Card = std::uint8_t;
    using Hand = std::array<Card, MaxCardsPerPlayer>;
    
    // Mapping: Player ID -> their hand
    std::array<Hand, 9> playerHands;


    // Constructor 
    PlayerSets();
};

class PlayerBankRollStructure{
public:

    // defines 1 BB as the universal currency base 
    static constexpr std::uint64_t BB = 100; 
    using BankRoll = std::uint64_t; 
    BankRoll current_balance; 

    explicit PlayerBankRollStructure(BankRoll starting_balance = 0)
        : current_balance(starting_balance) {}

};

} // namespace Player_Structure 
