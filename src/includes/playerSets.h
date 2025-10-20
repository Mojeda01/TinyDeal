#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <array>
#include <cstdint>
#include "cards.h"


namespace Player_Structure {

// Generic player set that can model different variants by cards per player
// and different table sizes. Defaults to 2 cards/player and 9 players.
template <std::size_t CardsPerPlayerT = 2, std::size_t MaxPlayersT = 9>
class PlayerSets {
public:
    // Expose template parameters as named constants
    static constexpr std::size_t MaxCardsPerPlayer = CardsPerPlayerT;
    static constexpr std::size_t MaxPlayers = MaxPlayersT;

    // Type aliases for clarity
    using Card = std::uint8_t;
    using Hand = std::array<Card, MaxCardsPerPlayer>;

    // Mapping: Player ID -> their hand
    std::array<Hand, MaxPlayers> playerHands{};

    // Constructor initializes to zeroed hands
    PlayerSets() = default;
};

// Convenient alias for current default configuration (Texas Hold'em, 9-max)
using DefaultPlayerSets = PlayerSets<>;

class PlayerBankRollStructure {
public:
    // defines 1 BB as the universal currency base
    static constexpr std::uint64_t BB = 100;
    using BankRoll = std::uint64_t;
    BankRoll current_balance;

    explicit PlayerBankRollStructure(BankRoll starting_balance = 0)
        : current_balance(starting_balance) {}
};

} // namespace Player_Structure
