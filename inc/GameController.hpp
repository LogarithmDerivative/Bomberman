#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Board;
class Player;
class Enemy;

enum class GAME_STATE {
    WON,
    LOST,
    IN_PROGRESS,
};

class GameController {
    Player& player_;
    Board& board_;
    std::vector<std::shared_ptr<Enemy>> enemies_;
    sf::Clock bombClock_;
    sf::Clock enemyMoveClock_;
    sf::Clock explosionClock_;
    sf::Time elapsedTimeAfterBombPlaced_;

    bool isExplosionClockRestarted_{false};
    bool isBombPlaced_{false};
    bool isBombBlown_{false};

    GAME_STATE currentGameState{GAME_STATE::IN_PROGRESS};
    double explosionDuration{0.7};

    void updateGameState();
    void moveEnemies();
    void updateIsBombPlaced();
    void updateIsBombBlown();
    void updateExplosionClock();

    public:
    GameController(Board& b, Player& p, std::vector<std::shared_ptr<Enemy>>& e);
    void handleEvents(sf::Event& event);
    bool isBombPlaced() const { return isBombPlaced_; }
    bool isBombBlown() const { return isBombBlown_; }
    void removeEnemies();
    bool playerIsInBombRange();
    void updateGame();
    bool isExplosion();
    GAME_STATE getGameState() { return currentGameState; }
};
