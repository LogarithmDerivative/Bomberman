#include "Board.hpp"

void Board::init(int height, int width) {
    for (int row = 0; row < height_; ++row) {
        for (int col = 0; col < width_; ++col) {
            if(col == 0 || col == width_ - 1) {
                board_[row][col].isWall = true;
            } else if (row == 0 || row == height_ - 1) {
                board_[row][col].isWall = true;
            } else if (row % 2 == 0 && col % 2 == 0) {
                board_[row][col].isWall = true;
            } else {
                board_[row][col].isWall = false;
            }
        }
    }
}

Board::Board(int height, int width)
:height_(height), width_(width) {
    init(height_, width_);
}
