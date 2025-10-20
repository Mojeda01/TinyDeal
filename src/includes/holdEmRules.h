#pragma once

#include "gameStructure.h"
#include "cards.h"

#include <array>
#include <cstdint>
#include <utility>
#include <vector>

namespace GameLogic {

// Texas Hold'em rules and round controller.
// Header defines the public interface and core types; implementation lives in a .cpp
// or can be added inline later. This class coordinates betting rounds, blinds/antes,
// board dealing with burns, pots/side-pots, and showdown resolution.
class HoldEmTable : public GameBase, public RoundLogic {
public:
    using Chips = std::uint64_t;

    enum class LimitType { NoLimit, PotLimit, FixedLimit };
    enum class ActionType { Fold, Check, Call, Bet, Raise, AllIn };
    enum class SeatState { Empty, Seated, Folded, AllIn, Acting, Out };

    struct Config {
        Chips smallBlind{50};
        Chips bigBlind{100};
        Chips ante{0};
        LimitType limit{LimitType::NoLimit};
        bool useAntes{false};
        bool allowStraddle{false};
    };

    struct PlayerInfo {
        SeatState state{SeatState::Empty};
        Chips stack{0};
        Chips committedThisStreet{0};
        Chips committedTotal{0};
        bool sitsOut{false};
    };

    struct Pot {
        Chips amount{0};
        std::vector<std::size_t> eligible; // seat indexes eligible to win this pot
    };

    struct Board {
        std::array<std::uint8_t, 5> cards{}; // encoded using Cards helpers
        std::size_t count{0};
    };

    struct Action {
        ActionType type{ActionType::Fold};
        Chips size{0}; // bet/raise/call amount as applicable
    };

    struct TableView {
        std::size_t button{0};
        std::size_t toAct{0};
        Board board{};
        std::vector<PlayerInfo> players; // size == MaxPlayers
        Chips currentBet{0};
        Chips minRaise{0};
        std::vector<Pot> pots;
        Phase phase{Phase::NotStarted};
    };

    // Construction
    HoldEmTable() : cfg_{} {
        static_assert(Players::MaxCardsPerPlayer == 2, "Hold'em requires 2 hole cards");
    }

    explicit HoldEmTable(const Config& cfg) : cfg_(cfg) {
        static_assert(Players::MaxCardsPerPlayer == 2, "Hold'em requires 2 hole cards");
        // Default stacks left as zero; caller should set stacks via setStack().
        // Button starts at seat 0.
    }

    // Configure stacks/seating
    void setStack(std::size_t seat, Chips stack) {
        validateSeat_(seat);
        auto& pi = playersMeta_[seat];
        pi.state = (stack > 0) ? SeatState::Seated : SeatState::Empty;
        pi.stack = stack;
    }

    // Read-only snapshot for UI/tests
    TableView view() const {
        TableView v;
        v.button = button_;
        v.toAct = toAct_;
        v.board = board_;
        v.players.assign(playersMeta_.begin(), playersMeta_.end());
        v.currentBet = currentBet_;
        v.minRaise = minRaise_;
        v.pots = pots_;
        v.phase = currentPhase_;
        return v;
    }

    // RoundLogic overrides
    bool resetRound() override;   // posts blinds/antes, deals hole cards, sets preflop action
    bool preFlop() override;      // prepare/advance preflop betting round
    bool flop() override;         // burn + 3 board cards, start/advance betting
    bool turn() override;         // burn + 1 board card, start/advance betting
    bool river() override;        // burn + 1 board card, start/advance betting
    bool finalizeRound() override;// resolve pots and move to Complete

    // Player action entrypoint for current street
    bool applyAction(std::size_t seat, Action action);

    // Button management (typically rotate per hand)
    void setButton(std::size_t seat) { validateSeat_(seat); button_ = seat; }
    std::size_t button() const noexcept { return button_;
    }

private:
    // Helpers — declarations only in header for now
    void clearForNewHand_();
    void postForcedBets_();
    void setFirstToActPreFlop_();
    void setFirstToActPostFlop_();
    bool isBettingClosed_() const;
    std::size_t nextEligible_(std::size_t from) const;
    void burnAndDealBoard_(std::size_t count);
    void maybeCreateSidePots_();
    std::pair<Chips, Chips> legalRaiseRange_() const; // min, max
    void resolveShowdown_();
    bool hasMultipleLivePlayers_() const;
    void validateSeat_(std::size_t seat) const;

private:
    Config cfg_{};

    // Positions
    std::size_t button_{0};
    std::size_t sbSeat_{0};
    std::size_t bbSeat_{0};
    std::size_t toAct_{0};

    // Betting state
    Chips currentBet_{0};
    Chips minRaise_{0};

    // Board & pots
    Board board_{};
    std::vector<Pot> pots_{}; // first is the main pot

    // Player meta state aligned with players_.playerHands
    std::array<PlayerInfo, MaxPlayers> playersMeta_{};
};

} // namespace GameLogic
