#pragma once 

#include "cards.h"
#include "playerSets.h"

#include <iostream>
#include <cstddef>
#include <cstdint>
#include <vector>

namespace GameLogic{

// shared game base for N-player poker-like games 
class Game_Base{
public:
    using Card = std::uint8_t;
    using Hand = Player_Structure::PlayerSets::Hand;

    // construct an empty table. actual player is count is set by derived types 
    GameBase(); 
    virtual ~GameBase();

    void dealHands();
    void reset();
    std::size_t playerCount() const noexcept; // current number of seated players at this table 
    const std::array<Hand, 9>& hands() const noexcept; // read-only view of player's hand.

protected:
    static constexpr std::size_t MaxPlayers = 9;
    void setPlayerCount(std::size_t n); // set fixed player count for derived tables. 
    Player_Structure::PlayerSets players_; // owns playerHands
    std::vector<Card> deck_;
    struct RNGImpl; // RNG kept per-table 
    RNGImpl * rng_; 
    std::size_t playerCount_ = 0; // fixed per derived game 

};

class Game_Logic{
public:
    GameLogic() = default;
    virtual ~GameLogic() = default;

    // Prevent copying - each game state instance should be unique.
    GameLogic(const GameLogic&) = delete;
    GameLogic& operator=(const GameLogic&) = delete;

    // Round progression interface
    virtual bool preFlop() = 0;
    virtual bool flop() = 0; 
    virtual bool turn() = 0;
    virtual bool river() = 0;

    // utility interface extensions 
    virtual bool resetRound() = 0;          // new round 
    virtual bool finalizeRound() = 0;       // handle pot distribution, cleanup

protected: 
    // Each implementation may track its current phase, player count 
    enum class Phase : std::uint8_t {
        NotStarted,
        PreFlop, 
        Flop,
        Turn,
        River,
        Complete
    };
    Phase currentPhase_ = Phase::NotStarted;
};

class NinePlayerGame : public GameBase { public: NinePlayerGame(); };
class EightPlayerGame : public GameBase { public: EightPlayerGame(); }; 
class SevenPlayerGame : public GameBase { public: SevenPlayerGame(); };
class SixPlayerGame : public GameBase { public: SixPlayerGame(); };
class FivePlayerGame : public GameBase { public: FivePlayerGame(); };
class FourPlayerGame : public GameBase { public: FourPlayerGame(); };
class ThreePlayerGame : public GameBase { public: ThreePlayerGame(); };
class TwoPlayerGame : public GameBase { public: TwoPlayerGame(); };

} // namespace GameLogic
