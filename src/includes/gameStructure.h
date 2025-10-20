#pragma once 

#include "cards.h"
#include "playerSets.h"

#include <cstddef>
#include <cstdint>
#include <vector>
#include <array>
#include <memory>

namespace GameLogic {

// shared game base for N-player poker-like games 
class GameBase {
public:
    using Card = std::uint8_t;
    using Players = Player_Structure::DefaultPlayerSets;
    using Hand = Players::Hand;

    // single source of truth for table size
    static constexpr std::size_t MaxPlayers = Players::MaxPlayers;

    // construct an empty table. actual player count is set by derived types
    GameBase();
    virtual ~GameBase();

    void dealHands();
    void reset();
    std::size_t playerCount() const noexcept; // current number of seated players at this table
    const std::array<Hand, MaxPlayers>& hands() const noexcept; // read-only view of players' hands

protected:
    void setPlayerCount(std::size_t n); // set fixed player count for derived tables.
    Players players_; // owns playerHands
    std::vector<Card> deck_;
    struct RNGImpl; // RNG kept per-table
    std::unique_ptr<RNGImpl> rng_;
    std::size_t playerCount_ = 0; // fixed per derived game
};

// Renamed from GameLogic to avoid GameLogic::GameLogic confusion
class RoundLogic {
public:
    RoundLogic() = default;
    virtual ~RoundLogic() = default;

    // Prevent copying - each game state instance should be unique.
    RoundLogic(const RoundLogic&) = delete;
    RoundLogic& operator=(const RoundLogic&) = delete;

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
