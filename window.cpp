#include "window.hpp"
#include <QLabel>
#include <QIntValidator>
#include <iostream> // temp

Window::Window(QWidget *parent)
    : QWidget(parent) {
    // Setup the window and set all the graphical objects and their styles:

    constexpr int fixed_window_width = 800;
    constexpr int fixed_window_height = 600;

    setFixedSize(fixed_window_width, fixed_window_height);
    start_button = new QPushButton("Start", this);
    start_button->setGeometry(30, 500, 80, 30);
    start_button->setCheckable(true);
    next_frame_button = new QPushButton("Next frame", this);
    next_frame_button->setGeometry(30, 550, 80, 30);

    grid_size_line_edit = new QLineEdit(this); //Line edit to set the grid size
    constexpr int min_grid_size = 3;
    constexpr int max_grid_size = 60;
    QIntValidator* grid_size_validator = new QIntValidator(min_grid_size, max_grid_size, this); // Ensures that the input is registered of if it is an integer between 3 and 200
    grid_size_line_edit->setValidator(grid_size_validator);
    QLabel* grid_size_label = new QLabel("Grid size\n(3-60):", this);
    grid_size_label->setGeometry(30, 395, 80, 30);
    grid_size_line_edit->setGeometry(30, 430, 80, 30);
    grid_size_label->setAlignment(Qt::AlignCenter);

    timer_interval_edit = new QLineEdit(this); // Line edit to set the time interval for auto advance
    QIntValidator* timer_interval_validator = new QIntValidator(1, 1000000000, this);
    timer_interval_edit->setValidator(timer_interval_validator);
    QLabel* timer_interval_label = new QLabel("Time interval for\nauto advance\n(1-1000000000 ms)", this);
    timer_interval_label->setGeometry(10, 290, 120, 60);
    timer_interval_edit->setGeometry(30, 350, 80, 30);
    timer_interval_label->setAlignment(Qt::AlignCenter);
    constexpr int default_timer_interval = 500; // default interval 500 ms
    timer_interval_edit->setText(QString::number(default_timer_interval));

    // Set the default grid size to 10 x 10 and initialise the game board
    constexpr int default_grid_size = 10;
    grid_size_line_edit->setText(QString::number(default_grid_size));
    constexpr QRect default_game_board_geometry(200, 30, 540, 540);
    game_board = new GameBoard(this, default_grid_size, default_game_board_geometry, default_timer_interval);

    // Connect user interaction signals to relevant Qt slots using the Qt library macros
    connect(grid_size_line_edit, SIGNAL(returnPressed()), this, SLOT(slot_initialise_board())); // When the grid size is chosen and enter is pressed, the board will be reinitialised if the range mentioned above is satisfied
    connect(timer_interval_edit, SIGNAL(returnPressed()), this, SLOT(slot_change_timer_interval())); // When the timer interval is chosen and enter is pressed, the timer interval will be changed, if the range is correct
    connect(start_button, SIGNAL(clicked(bool)), this, SLOT(slot_start_button_clicked(bool)));
    connect(this, SIGNAL(signal_start_auto_advance()), game_board, SLOT(slot_start_auto_advance()));
    connect(this, SIGNAL(signal_stop_auto_advance()), game_board, SLOT(slot_stop_auto_advance()));
    connect(next_frame_button, SIGNAL(clicked()), this, SLOT(slot_next_frame_button_clicked()));
}

void Window::slot_initialise_board() {
    if (game_board->auto_advance_started) // if auto advance is started, the grid size cannot be changed
        return;

    grid_size_line_edit->selectAll(); // Highlight the text to signal the user that the number has been accepted
    int grid_size = grid_size_line_edit->text().toInt(); // The QIntValidator object ensures this is always possible
    game_board->initialise(grid_size);
}

void Window::slot_change_timer_interval() {
    if (game_board->auto_advance_started) // if auto advance is started, the time interval cannot be changed either
        return;

    timer_interval_edit->selectAll();
    game_board->timer_interval = timer_interval_edit->text().toInt();

}

void Window::slot_start_button_clicked(bool checked) {
    if (checked) { // checked is true -> the start button was not previously pressed down
        emit signal_start_auto_advance();
        start_button->setText("Pause");
    } else {
        emit signal_stop_auto_advance();
        start_button->setText("Start");
    }
}

void Window::slot_next_frame_button_clicked() {
    if (game_board->auto_advance_started) // if auto advance is started, the next frame button will do nothing
        return;

    game_board->slot_advance_one_step();
}
