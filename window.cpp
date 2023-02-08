#include "window.hpp"
#include <QLabel>
#include <QIntValidator>
#include <iostream> // temp

Window::Window(QWidget *parent)
    : QWidget(parent) {
    // Setup the window and set all the graphical objects and their styles:
    setFixedSize(800, 600);
    start_button = new QPushButton("Start", this);
    start_button->setGeometry(30, 500, 80, 30);
    start_button->setCheckable(true);
    next_frame_button = new QPushButton("Next frame", this);
    next_frame_button->setGeometry(30, 550, 80, 30);
    QLabel* grid_size_label = new QLabel("Grid size\n(3-60):", this);
    grid_size_label->setGeometry(30, 395, 80, 30);

    grid_size_line_edit = new QLineEdit(this); //Line edit to set the grid size
    constexpr int min_grid_size = 3;
    constexpr int max_grid_size = 60;
    QIntValidator* validator = new QIntValidator(min_grid_size, max_grid_size, this); // Ensures that the input is registered of if it is an integer between 3 and 200
    grid_size_line_edit->setValidator(validator);
    grid_size_line_edit->setGeometry(30, 430, 80, 30);
    grid_size_label->setAlignment(Qt::AlignCenter);

    // Set the default grid size to 10 x 10 and initialise the game board
    constexpr int default_grid_size = 10;
    grid_size_line_edit->setText(QString::number(default_grid_size));
    constexpr QRect default_game_board_geometry(200, 30, 540, 540);
    game_board = new GameBoard(this, default_grid_size, default_game_board_geometry);

    // Connect user interaction signals to relevant Qt slots using the Qt library macros
    connect(grid_size_line_edit, SIGNAL(returnPressed()), this, SLOT(slot_initialise_board())); // When the grid size is chosen and enter is clicked, the board will be reinitialised if the range mentioned above is satisfied
    connect(start_button, SIGNAL(clicked(bool)), this, SLOT(slot_start_button_clicked(bool)));
    connect(this, SIGNAL(start_auto_advance_signal()), game_board, SLOT(slot_start_auto_advance()));
    connect(next_frame_button, SIGNAL(clicked()), this, SLOT(slot_next_frame_button_clicked()));
}

void Window::slot_initialise_board() {
    grid_size_line_edit->selectAll(); // Highlight the text to signal the user that the number has been accepted
    int grid_size = grid_size_line_edit->text().toInt();
    game_board->initialise(grid_size);
}

void Window::slot_start_button_clicked(bool checked) {
    if (checked) { // checked is true -> the start button was not previously pressed down
        emit start_auto_advance_signal();
        start_button->setText("Pause");
    } else {
        start_button->setText("Start");
    }
}

void Window::slot_next_frame_button_clicked() {
    if (game_board->auto_advance_started) // if auto advance is started, the next frame button will do nothing
        return;

    game_board->slot_advance_one_step();
}

void Window::slot_cell_button_clicked() {}
