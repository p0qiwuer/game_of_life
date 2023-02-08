#include "gameboard.hpp"
#include <cmath>
#include <iostream> // temp

GameBoard::GameBoard(QWidget* parent, const int default_grid_size, const QRect default_geometry)
    : QFrame(parent), auto_advance_started(false), grid_size(0){ // setting initial grid size to 0, since there are no CellButtons to delete
    setGeometry(default_geometry);
    initialise(default_grid_size);
}

void GameBoard::initialise(int new_grid_size) {
    close_all_buttons(); // delete previous CellButtons, if there are any
    grid_size = new_grid_size;

    // Used to correctly space the buttons
    const float board_width_increment = (float)this->width() / (float)grid_size;
    const float board_height_increment = (float)this->height() / (float)grid_size;
    const int board_width_increment_int = std::round(board_width_increment);
    const int board_height_increment_int = std::round(board_height_increment);

    cell_2dvector = {};
    for (int i = 0; i < grid_size; ++i) {
        std::vector<CellButton*> cell_vector;
        for (int j = 0; j < grid_size; ++j) {
            CellButton* new_button = new CellButton(this, i, j);
            new_button->setGeometry(i * board_width_increment, j * board_height_increment, board_width_increment_int, board_height_increment_int);
            connect(new_button, SIGNAL(signal_this_button_clicked(int,int)), this, SLOT(slot_cell_button_clicked(int,int)));
            new_button->show();
            cell_vector.emplace_back(new_button);
        }
        cell_2dvector.emplace_back(cell_vector);
    }
}

void GameBoard::slot_advance_one_step() {

    // first check the new state of each cell
    std::vector<std::vector<bool>> alive_status_2dvector = {};
    for (int i = 0; i < grid_size; ++i) {
        std::vector<bool> alive_status_vector = {};
        for (int j = 0; j < grid_size; ++j) {
            alive_status_vector.emplace_back(will_still_be_alive(i, j));
        }
        alive_status_2dvector.emplace_back(alive_status_vector);
    }

    // then update each cell accordingly

    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            cell_2dvector[i][j]->set_alive_status(alive_status_2dvector[i][j]);
        }
    }

}

void GameBoard::slot_start_auto_advance() {
    auto_advance_started = true;
}

void GameBoard::slot_cell_button_clicked(int column, int row) {
    if (auto_advance_started) {
        cell_2dvector[column][row]->toggle(); // reverses the button check status if the auto advance is on and does nothing
        return;
    }
    cell_2dvector[column][row]->toggle_alive_status(); // if the auto advance is not on, will toggle the button status from alive to dead and vice versa
}

void GameBoard::close_all_buttons() {
    for (int i = 0; i < grid_size; ++i) {
        for (int j = 0; j < grid_size; ++j) {
            cell_2dvector[i][j]->close();
        }
    }
}

bool GameBoard::will_still_be_alive(const int column, const int row) const { //checks whether a cell will alive in the next iteration
    int neighbours = 0;
    for (int i = column - 1; i <= column + 1; ++i) {
        if (i < 0 || i >= grid_size)
            continue;
        for (int j = row - 1; j <= row + 1; ++j) {
            if (j < 0 || j >= grid_size)
                continue;

            if (i == column && j == row)
                continue;

            if (cell_2dvector[i][j]->is_alive)
                ++neighbours;
        }
    }
    if (neighbours <= 1 || neighbours >= 4) // death by solitude or overpopulation
        return false;

    if (cell_2dvector[column][row]->is_alive) // Live cells remain alive
        return true;

    if (neighbours == 3) // Dead cell come back to life by regrowth
        return true;

    return false;
}
