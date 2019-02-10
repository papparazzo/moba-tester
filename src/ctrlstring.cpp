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

moba::JsonIntPtr CtrlString::get_jsonInt() const {
    return moba::toJsonNumberPtr(std::stol(m_Entry_String.get_text()));
}

moba::JsonFloatPtr CtrlString::get_jsonFloat() const {
    return moba::toJsonNumberPtr(std::stof(m_Entry_String.get_text()));
}

moba::JsonStringPtr CtrlString::get_jsonText() const {
    return moba::toJsonStringPtr(std::string(m_Entry_String.get_text()));
}