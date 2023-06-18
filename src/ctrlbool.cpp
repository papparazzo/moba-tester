#include "ctrlbool.h"

#include <string>

CtrlBool::CtrlBool() {
    m_VBox.append(m_HBox_Bool);
    m_HBox_Bool.append(m_Check_Bool);
}

CtrlBool::~CtrlBool() {
}

bool CtrlBool::get_value() const {
    return m_Check_Bool.get_active();
}

void CtrlBool::init(const std::string &caption, Gtk::ScrolledWindow &container) {
    m_Check_Bool.set_label(caption);
    m_Check_Bool.set_active(false);
    container.set_child(m_VBox);
  //  container.show_all_children();
}
