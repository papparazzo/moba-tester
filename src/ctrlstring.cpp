#include "ctrlstring.h"

#include <string>

CtrlString::CtrlString() {
    m_VBox.pack_start(m_HBox_String, Gtk::PACK_SHRINK);
    m_HBox_String.pack_start(m_Label_String, Gtk::PACK_SHRINK);
    m_HBox_String.pack_end(m_Entry_String, Gtk::PACK_SHRINK);
}

CtrlString::~CtrlString() {
}

void CtrlString::init(const std::string &caption, Gtk::ScrolledWindow &container) {
    m_Entry_String.set_text("");
    m_Label_String.set_label(caption);
    container.add(m_VBox);
    container.show_all_children();
}

long CtrlString::get_integer() const {
    return std::stol(m_Entry_String.get_text());
}

float CtrlString::get_float() const {
    return std::stof(m_Entry_String.get_text());
}

std::string CtrlString::get_text() const {
    return m_Entry_String.get_text();
}
