#include "includes/gameStructure.h"
#include "includes/cards.h"

#include <algorithm>
#include <random>
#include <stdexcept>

namespace GameLogic {

struct GameBase::RNGImpl {
    std::mt19937 engine;

    RNGImpl()
        : engine(std::random_device{}()) {}
};

GameBase::GameBase()
    : deck_(Cards::Deck.begin(), Cards::Deck.end()),
      rng_(std::make_unique<RNGImpl>()) {
    reset();
}

GameBase::~GameBase() = default;

void GameBase::setPlayerCount(std::size_t n) {
    if (n == 0 || n > MaxPlayers) {
        throw std::out_of_range("player count outside supported range");
    }
    playerCount_ = n;
    reset();
}

std::size_t GameBase::playerCount() const noexcept {
    return playerCount_;
}

const std::array<GameBase::Hand, GameBase::MaxPlayers>& GameBase::hands() const noexcept { return players_.playerHands; }

void GameBase::reset() {
    players_.playerHands.fill(Player_Structure::PlayerSets::Hand{});
    deck_.assign(Cards::Deck.begin(), Cards::Deck.end());
    std::shuffle(deck_.begin(), deck_.end(), rng_->engine);
}

void GameBase::dealHands() {
    constexpr std::size_t CardsPerPlayer = Player_Structure::DefaultPlayerSets::MaxCardsPerPlayer;

    if (playerCount_ == 0) {
        throw std::logic_error("cannot deal cards with no seated players");
    }

    const auto required = playerCount_ * CardsPerPlayer;
    if (deck_.size() < required) {
        reset();
    }

    auto current = deck_.begin();
    // Round-robin dealing: one card to each player per pass
    for (std::size_t card = 0; card < CardsPerPlayer; ++card) {
        for (std::size_t player = 0; player < playerCount_; ++player) {
            players_.playerHands[player][card] = *current++;
        }
    }

    deck_.erase(deck_.begin(), deck_.begin() + static_cast<std::ptrdiff_t>(required));
}

NinePlayerGame::NinePlayerGame() {
    setPlayerCount(9);
}

EightPlayerGame::EightPlayerGame() {
    setPlayerCount(8);
}

SevenPlayerGame::SevenPlayerGame() {
    setPlayerCount(7);
}

SixPlayerGame::SixPlayerGame() {
    setPlayerCount(6);
}

FivePlayerGame::FivePlayerGame() {
    setPlayerCount(5);
}

FourPlayerGame::FourPlayerGame() {
    setPlayerCount(4);
}

ThreePlayerGame::ThreePlayerGame() {
    setPlayerCount(3);
}

TwoPlayerGame::TwoPlayerGame() {
    setPlayerCount(2);
}

} // namespace GameLogic
