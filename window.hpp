#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "gameboard.hpp"
#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QLineEdit>

// Main window of the program, contains most of the user interface and handles most inputs

class Window : public QWidget {
    Q_OBJECT

public:
    Window(QWidget* parent = nullptr);

public slots: // Qt specifier, means that Qt signals can be connected to these functions
    void slot_initialise_board();
    void slot_start_button_clicked(bool checked);
    void slot_next_frame_button_clicked();
    void slot_cell_button_clicked();

signals: // Qt specifier for a signal, they don't need implementation since they are only used to connect to slots
    void start_auto_advance_signal();

private:
    QPushButton* start_button;
    QPushButton* next_frame_button;
    QLineEdit* grid_size_line_edit;
    GameBoard* game_board;

};
#endif // WINDOW_HPP
