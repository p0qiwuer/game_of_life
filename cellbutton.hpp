#ifndef CELLBUTTON_HPP
#define CELLBUTTON_HPP

#include <QPushButton>

class CellButton : public QPushButton
{
    Q_OBJECT
public:
    CellButton(QWidget* parent, const int new_column, const int new_row);
    void toggle_alive_status();
    void set_alive_status(bool status); // Necessary since the checked state of the button needs to be updated as well

    bool is_alive;
    const int column, row;

public slots:
    void slot_button_clicked();

signals:
    void signal_this_button_clicked(int this_column, int this_row); // Necessary in order to specify to the game board which button was pushed
};

#endif // CELLBUTTON_HPP
