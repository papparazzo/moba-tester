#include "ctrlstring.h"

#include <string>

CtrlString::CtrlString() {
    m_VBox.append(m_HBox_String);
    m_HBox_String.append(m_Label_String);
    m_HBox_String.append(m_Entry_String);
}

CtrlString::~CtrlString() {
}

void CtrlString::init(const std::string &caption, Gtk::ScrolledWindow &container) {
    m_Entry_String.set_text("");
    m_Label_String.set_label(caption);
    container.set_child(m_VBox);
  //  container.show_all_children();
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
