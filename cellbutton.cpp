#include "cellbutton.hpp"
#include <iostream> // temp

CellButton::CellButton(QWidget* parent, const int new_column, const int new_row)
    : QPushButton(parent), is_alive(false), column(new_column), row(new_row) {

    setCheckable(true);
    connect(this, SIGNAL(clicked()), this, SLOT(slot_button_clicked()));

}

void CellButton::slot_button_clicked() {
    emit signal_this_button_clicked(column, row);
}

void CellButton::toggle_alive_status() {
    set_alive_status(!is_alive);
}
void CellButton::set_alive_status(bool status) {
    is_alive = status;
    setChecked(status);
}
