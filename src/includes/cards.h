#pragma once 

#include <cstdint>
#include <string_view>
#include <array>
#include <cstdint>

class Cards{
public:
    
    /* Defining the 52-deck of cards */

    // Defining card ranks
    static constexpr std::uint8_t Ace       = 0x7F;
    static constexpr std::uint8_t King      = 0x6E;
    static constexpr std::uint8_t Queen     = 0x5D;
    static constexpr std::uint8_t Jack      = 0x4C;
    static constexpr std::uint8_t Ten       = 0x3B;
    static constexpr std::uint8_t Nine      = 0x2A;
    static constexpr std::uint8_t Eight     = 0x19;
    static constexpr std::uint8_t Seven     = 0x08;
    static constexpr std::uint8_t Six       = 0x07;
    static constexpr std::uint8_t Five      = 0x06;
    static constexpr std::uint8_t Four      = 0x05;
    static constexpr std::uint8_t Three     = 0x04;
    static constexpr std::uint8_t Two       = 0x03;

    // Defining suits as 4-bit upper identifiers
    static constexpr std::uint8_t Hearts            = 0x09;
    static constexpr std::uint8_t Diamonds          = 0x0A;
    static constexpr std::uint8_t Clubs             = 0x0B;
    static constexpr std::uint8_t Spades            = 0x0C;

    // Mapping back to human-readable names 
    static constexpr std::array<std::string_view, 13> Names = {
        "Ace","King","Queen","Jack","Ten","Nine",
        "Eight","Seven","Six","Five","Four","Three","Two"
    };

    static constexpr std::array<std::string_view, 4> Suits = {
        "Hearts", "Diamonds", "Clubs", "Spades"
    };

    // Generating the full 52-card deck as raw bytes 
    static constexpr std::array<std::uint8_t, 52> Deck = [] {
    
        std::array<std::uint8_t, 52> cards{};
        std::uint8_t idx = 0;

        constexpr std::array<std::uint8_t, 13> ranks = {
            Ace, King, Queen, Jack, Ten, Nine, Eight, Seven, Six, Five, 
            Four, Three, Two
        };

        for (std::uint8_t suit : {Hearts, Diamonds, Clubs, Spades}){
            for (auto rank : ranks){
                cards[idx++] = static_cast<std::uint8_t>(suit | rank);
            }
        }
        return cards;
    }(); 

    // Utility functions for decoding 
    static constexpr std::uint8_t getSuit(std::uint8_t card) { return card & 0xF0; }
    static constexpr std::uint8_t getRank(std::uint8_t card) { return card & 0x0F; }

};
