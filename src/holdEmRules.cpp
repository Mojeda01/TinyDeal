#include "includes/holdEmRules.h"

#include <algorithm>
#include <stdexcept>

namespace GameLogic {

void HoldEmTable::clearForNewHand_() {
    // Reset transient state
    board_ = Board{};
    pots_.clear();
    pots_.push_back(Pot{}); // main pot
    currentBet_ = 0;
    minRaise_ = 0;

    for (auto& p : playersMeta_) {
        if (p.state == SeatState::Seated) {
            p.committedThisStreet = 0;
            // keep stack and committedTotal as-is for now
        }
    }

    // Positions (simple rotation relative to button)
    sbSeat_ = (button_ + 1) % MaxPlayers;
    bbSeat_ = (button_ + 2) % MaxPlayers;
    toAct_ = 0;
}

void HoldEmTable::postForcedBets_() {
    // Stub: no-op
}

void HoldEmTable::setFirstToActPreFlop_() {
    // UTG (first left of BB)
    toAct_ = (bbSeat_ + 1) % MaxPlayers;
}

void HoldEmTable::setFirstToActPostFlop_() {
    // First left of button
    toAct_ = (button_ + 1) % MaxPlayers;
}

bool HoldEmTable::isBettingClosed_() const {
    // Stub: immediately closed
    return true;
}

std::size_t HoldEmTable::nextEligible_(std::size_t from) const {
    // Stub: advance one seat
    return (from + 1) % MaxPlayers;
}

void HoldEmTable::burnAndDealBoard_(std::size_t count) {
    if (count == 0) return;
    // Burn one card if possible
    if (!deck_.empty()) {
        deck_.erase(deck_.begin());
    }
    for (std::size_t i = 0; i < count && !deck_.empty() && board_.count < board_.cards.size(); ++i) {
        board_.cards[board_.count++] = deck_.front();
        deck_.erase(deck_.begin());
    }
}

void HoldEmTable::maybeCreateSidePots_() {
    // Stub: no-op
}

std::pair<HoldEmTable::Chips, HoldEmTable::Chips> HoldEmTable::legalRaiseRange_() const {
    // Stub: no constraints
    return {0, 0};
}

void HoldEmTable::resolveShowdown_() {
    // Stub: no-op
}

bool HoldEmTable::hasMultipleLivePlayers_() const {
    // Stub: assume not
    return false;
}

void HoldEmTable::validateSeat_(std::size_t seat) const {
    if (seat >= MaxPlayers) {
        throw std::out_of_range("seat out of range");
    }
}

bool HoldEmTable::resetRound() {
    clearForNewHand_();
    // Shuffle and deal hole cards using GameBase
    reset();
    dealHands();
    postForcedBets_();
    setFirstToActPreFlop_();
    currentPhase_ = Phase::PreFlop;
    return true;
}

bool HoldEmTable::preFlop() {
    // Stub: nothing to do beyond resetRound
    return true;
}

bool HoldEmTable::flop() {
    burnAndDealBoard_(3);
    setFirstToActPostFlop_();
    return true;
}

bool HoldEmTable::turn() {
    burnAndDealBoard_(1);
    setFirstToActPostFlop_();
    return true;
}

bool HoldEmTable::river() {
    burnAndDealBoard_(1);
    setFirstToActPostFlop_();
    return true;
}

bool HoldEmTable::finalizeRound() {
    resolveShowdown_();
    currentPhase_ = Phase::Complete;
    return true;
}

bool HoldEmTable::applyAction(std::size_t seat, Action action) {
    (void)action; // unused in stub
    validateSeat_(seat);
    toAct_ = nextEligible_(seat);
    return true;
}

} // namespace GameLogic

