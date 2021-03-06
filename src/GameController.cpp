#include "GameController.hpp"

#include "Board.hpp"
#include "Bomb.hpp"
#include "Enemy.hpp"
#include "Player.hpp"

#include <cmath>

GameController::GameController(Board& board, Player& player, std::vector<std::shared_ptr<Enemy>>& enemy) :
    board_(board), player_(player), enemies_(enemy)
{
}

void GameController::handleEvents(sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::W) {
            player_.moveUp();
        }
        if (event.key.code == sf::Keyboard::A) {
            player_.moveLeft();
        }
        if (event.key.code == sf::Keyboard::S) {
            player_.moveDown();
        }
        if (event.key.code == sf::Keyboard::D) {
            player_.moveRight();
        }
        if (event.key.code == sf::Keyboard::Space) {
            if (isBombPlaced_) {
                return;
            }
            player_.placeBomb();
            bombClock_.restart();
            isBombPlaced_ = true;
        }
    }
}

void GameController::updateGame()
{
    updateGameState();
    moveEnemies();
    updateIsBombBlown();
    updateIsBombPlaced();
    updateExplosionClock();
}

void GameController::updateGameState()
{
    if (isBombBlown_ && playerIsInBombRange()) {
        currentGameState = GAME_STATE::LOST;
    }
    if (std::any_of(enemies_.cbegin(), enemies_.cend(), [this](auto & enemy) {
            return enemy->getCol() == player_.getCol() && enemy->getRow() == player_.getRow();
        })) {
        currentGameState = GAME_STATE::LOST;
    }
}

bool GameController::playerIsInBombRange()
{
    auto bomb = player_.getBomb();
    if (player_.getCol() == bomb.getCol() && std::abs(player_.getRow() - bomb.getRow()) <= bomb.getFirePower()) {
        return true;
    }
    if (player_.getRow() == bomb.getRow() && std::abs(player_.getCol() - bomb.getCol()) <= bomb.getFirePower()) {
        return true;
    }
    return false;
}

void GameController::moveEnemies()
{
    auto time = enemyMoveClock_.restart();
    for (auto enemy : enemies_) {
        enemy->move(time, player_);
    }
}

void GameController::updateIsBombPlaced()
{
    auto timeToBlow = player_.getBomb().getTimeToBlow();
    if (elapsedTimeAfterBombPlaced_.asSeconds() >= timeToBlow + explosionDuration) {
        isBombPlaced_ = false;
    }
}

void GameController::updateIsBombBlown()
{
    if (!isBombPlaced_) {
        return;
    }
    elapsedTimeAfterBombPlaced_ = bombClock_.getElapsedTime();
    auto timeToBlow = player_.getBomb().getTimeToBlow();
    if (elapsedTimeAfterBombPlaced_.asSeconds() >= timeToBlow) {
        isBombBlown_ = true;
    }
    if (elapsedTimeAfterBombPlaced_.asSeconds() >= timeToBlow + explosionDuration) {
        isBombBlown_ = false;
    }
}

void GameController::updateExplosionClock() {
    if (isBombBlown_ && !isExplosionClockRestarted_) {
        explosionClock_.restart();
        isExplosionClockRestarted_ = true;
    }
    if (isExplosionClockRestarted_ && !isExplosion()) {
        isExplosionClockRestarted_ = false;
    }
}

void GameController::removeEnemies()
{
    auto bomb = player_.getBomb();
    for (auto enemy : enemies_) {
        if (enemy->getRow() == bomb.getRow()) {
            if (std::abs(enemy->getCol() - bomb.getCol()) <= bomb.getFirePower()) {
                enemy->setPosition(-1, -1);
            }
        }
        if (enemy->getCol() == bomb.getCol()) {
            if (std::abs(enemy->getRow() - bomb.getRow()) <= bomb.getFirePower()) {
                enemy->setPosition(-1, -1);
            }
        }
    }
}

bool GameController::isExplosion()
{
    if (!isBombPlaced_) {
        return false;
    }
    auto timeOfExplosion = explosionClock_.getElapsedTime().asSeconds();
    if (timeOfExplosion <= explosionDuration) {
        return true;
    }
    return false;
}
