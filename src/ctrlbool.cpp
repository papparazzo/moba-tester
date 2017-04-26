#include "ctrlbool.h"

#include <string>

CtrlBool::CtrlBool() {
    m_VBox.pack_start(m_HBox_Bool, Gtk::PACK_SHRINK);
    m_HBox_Bool.pack_start(m_Check_Bool, Gtk::PACK_SHRINK);
}

CtrlBool::~CtrlBool() {
}

moba::JsonBoolPtr CtrlBool::get_value() const {
    return moba::toJsonBoolPtr(m_Check_Bool.get_active());
}

void CtrlBool::init(const std::string &caption, Gtk::ScrolledWindow &container) {
    m_Check_Bool.set_label(caption);
    m_Check_Bool.set_active(false);
    container.add(m_VBox);
    container.show_all_children();
}
