#ifndef GAMEBOARD_HPP
#define GAMEBOARD_HPP

#include "cellbutton.hpp"
#include <QFrame>
#include <vector>

// The main board of the action, will deal with game logic and advancement

class GameBoard : public QFrame {
    Q_OBJECT
public:
    GameBoard(QWidget* parent, const int default_grid_size, QRect default_geometry);
    void initialise(int new_grid_size); // Separate function to set a different grid without the need to delete the GameBoard object

    bool auto_advance_started;

public slots: // Qt specifier, means that Qt signals can be connected to these functions
    void slot_advance_one_step();
    void slot_start_auto_advance();
    void slot_cell_button_clicked(int column, int row);

private:
    void close_all_buttons();
    bool will_still_be_alive(const int column, const int row) const;

    std::vector<std::vector<CellButton*>> cell_2dvector;
    int grid_size;
};

#endif // GAMEBOARD_HPP
